#include "global.h"
#include "day_night.h"
#include "battle_transition.h"
#include "overworld.h"
#include "palette.h"
#include "fieldmap.h"
#include "constants/pokemon.h"
#include "constants/quest_log.h"
#include "constants/rgb.h"

#define DAY_NIGHT_CYCLE_MINUTES     2
#define DAY_NIGHT_DAY_MINUTES       1

#define DAY_NIGHT_BG_PALETTES           0x00001FFF
// Bits for only the secondary tileset's BG palette slots (7-12).
#define DAY_NIGHT_SECONDARY_BG_PALETTES 0x00001F80
#define DAY_NIGHT_MAX_BLEND         5
#define DAY_NIGHT_TRANSITION_STEPS  10
#define DAY_NIGHT_STEP_FRAMES       15
#define DAY_NIGHT_NIGHT_COLOR       RGB(8, 10, 20)
#define OBJ_PALETTE_COUNT           16

static bool8 sVisualStateInitialized;
static u8 sCurrentTransitionStep;
static u8 sTargetTransitionStep;
static u8 sTransitionFrameCounter;
static u8 sLastAppliedBgStep;
static u8 sLastAppliedObjStep;
// TRUE from DayNight_BeginWarpTransition to DayNight_EndWarpTransition (any
// warp regardless of destination).  Prevents ReapplyAllObjTint from running
// in the gap between fade-out complete and new map fading in, which would
// re-tint OBJ faded while old sprites are still in OAM (bug #2 flash).
static bool8 sWarpInProgress;
// TRUE only when the warp destination is outdoors.  Gates the mid-game BG
// reload so it does not fire while the new map's unfaded palette is already
// correctly tinted from map-load time.
static bool8 sWarpTransitionSuppressed;

// Raw-colour backup for every OBJ palette slot, written at PatchObjectPalette
// time (outdoor maps only).  Required because ApplyDayNightTintToObjPaletteSlot
// overwrites gPlttBufferUnfaded, so we store the ROM originals here and use
// them in ReapplyAllObjTint for day<->night and step-level transitions.
static bool8 sObjSlotHasBackup[OBJ_PALETTE_COUNT];
static u16   sObjPaletteBackup[OBJ_PALETTE_COUNT][16];

static u8 GetDayNightBlendForStep(u8 step)
{
    if (step >= DAY_NIGHT_TRANSITION_STEPS)
        return DAY_NIGHT_MAX_BLEND;

    return (step * DAY_NIGHT_MAX_BLEND + (DAY_NIGHT_TRANSITION_STEPS / 2)) / DAY_NIGHT_TRANSITION_STEPS;
}

static void EnsureVisualStateInitialized(void)
{
    if (!sVisualStateInitialized)
    {
        u32 totalMinutes = gSaveBlock2Ptr->playTimeHours * 60 + gSaveBlock2Ptr->playTimeMinutes;
        u8 timeOfDay = ((totalMinutes % DAY_NIGHT_CYCLE_MINUTES) < DAY_NIGHT_DAY_MINUTES)
                         ? EVO_TIME_DAY : EVO_TIME_NIGHT;
        sTargetTransitionStep = (timeOfDay == EVO_TIME_NIGHT) ? DAY_NIGHT_TRANSITION_STEPS : 0;
        sCurrentTransitionStep = sTargetTransitionStep;
        sVisualStateInitialized = TRUE;
    }
}

static bool8 TryGetBattleNightBlendCoeff(u8 *blendCoeff)
{
    // Called from battle startup paths where gMain.inBattle is not yet set.
    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return FALSE;

    EnsureVisualStateInitialized();

    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return FALSE;

    *blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    return (*blendCoeff != 0);
}

// Tint one range of BG palette entries.
// Reads raw colours from gPlttBufferUnfaded (which must still hold ROM values),
// writes the tinted result to BOTH buffers.
//
// Writing to gPlttBufferFaded here is intentional for two reasons:
//  - During map-load paths VBlank is cleared (FieldClearVBlankHBlankCallbacks),
//    so the write is never seen as a flash; FadeInFromBlack then zeroes faded
//    again before the callback is restored.
//  - During mid-game step changes (called with bufferTransferDisabled=TRUE),
//    both buffers are updated atomically before the next VBlank transfer,
//    preventing the one-frame raw-palette flash (bug #3).
//  - After returning from battle (no FadeInFromBlack), faded is immediately
//    correct so sprites appear tinted on the first visible frame.
static void TintBgPaletteEntries(u16 paletteOffset, u16 count, u8 blendCoeff)
{
    s32 nr = (s32)GET_R(DAY_NIGHT_NIGHT_COLOR);
    s32 ng = (s32)GET_G(DAY_NIGHT_NIGHT_COLOR);
    s32 nb = (s32)GET_B(DAY_NIGHT_NIGHT_COLOR);
    u16 i;

    for (i = 0; i < count; i++)
    {
        u16 index = paletteOffset + i;
        struct PlttData *src;
        s32 r, g, b;
        u16 color;

        if (index == 0)
            continue;
        src = (struct PlttData *)&gPlttBufferUnfaded[index];
        r = src->r + (((s32)(nr - src->r) * blendCoeff) >> 4);
        g = src->g + (((s32)(ng - src->g) * blendCoeff) >> 4);
        b = src->b + (((s32)(nb - src->b) * blendCoeff) >> 4);
        color = (u16)(r | (g << 5) | (b << 10));
        gPlttBufferUnfaded[index] = color;
        gPlttBufferFaded[index] = color;
    }
}

// Tint one OBJ palette slot from its raw backup into BOTH palette buffers.
// At blendCoeff == 0 this restores the unmodified day colours.
static void TintObjSlotFromBackup(u8 slot, u8 blendCoeff)
{
    u16 base = OBJ_PLTT_ID(slot);
    s32 nr = (s32)GET_R(DAY_NIGHT_NIGHT_COLOR);
    s32 ng = (s32)GET_G(DAY_NIGHT_NIGHT_COLOR);
    s32 nb = (s32)GET_B(DAY_NIGHT_NIGHT_COLOR);
    u16 i;

    for (i = 0; i < 16; i++)
    {
        u16 raw = sObjPaletteBackup[slot][i];
        struct PlttData *src = (struct PlttData *)&raw;
        s32 r = src->r + (((s32)(nr - src->r) * blendCoeff) >> 4);
        s32 g = src->g + (((s32)(ng - src->g) * blendCoeff) >> 4);
        s32 b = src->b + (((s32)(nb - src->b) * blendCoeff) >> 4);
        u16 color = (u16)(r | (g << 5) | (b << 10));
        gPlttBufferUnfaded[base + i] = color;
        gPlttBufferFaded[base + i] = color;
    }
}

// Re-tint every OBJ palette slot that has a backup at the given coefficient.
static void ReapplyAllObjTint(u8 blendCoeff)
{
    u8 slot;
    for (slot = 0; slot < OBJ_PALETTE_COUNT; slot++)
    {
        if (sObjSlotHasBackup[slot])
            TintObjSlotFromBackup(slot, blendCoeff);
    }
    sLastAppliedObjStep = sCurrentTransitionStep;
}

void DayNight_BeginWarpTransition(bool8 destinationIsOutdoors)
{
    EnsureVisualStateInitialized();
    sTargetTransitionStep = sCurrentTransitionStep;
    sTransitionFrameCounter = 0;
    sLastAppliedBgStep = sCurrentTransitionStep;
    sLastAppliedObjStep = sCurrentTransitionStep;
    sWarpInProgress = TRUE;
    sWarpTransitionSuppressed = destinationIsOutdoors;
}

void DayNight_EndWarpTransition(void)
{
    sWarpTransitionSuppressed = FALSE;
    sWarpInProgress = FALSE;
}

u8 GetDayNightTimeOfDay(void)
{
    u32 totalMinutes = gSaveBlock2Ptr->playTimeHours * 60 + gSaveBlock2Ptr->playTimeMinutes;
    u32 phase = totalMinutes % DAY_NIGHT_CYCLE_MINUTES;

    if (phase < DAY_NIGHT_DAY_MINUTES)
        return EVO_TIME_DAY;

    return EVO_TIME_NIGHT;
}

bool8 IsNightTime(void)
{
    return GetDayNightTimeOfDay() == EVO_TIME_NIGHT;
}

bool8 IsDayTime(void)
{
    return GetDayNightTimeOfDay() == EVO_TIME_DAY;
}

// Core BG-palette tinting.  Called from both map-load paths and from the
// mid-game step-change handler inside UpdateDayNightVisuals.
// Initialises sCurrentTransitionStep on the very first call.
static void ApplyDayNightTintToMapPalettesInternal(void)
{
    u32 selectedPalettes;
    u16 paletteOffset;
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;

    EnsureVisualStateInitialized();

    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;

    sLastAppliedBgStep = sCurrentTransitionStep;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    selectedPalettes = DAY_NIGHT_BG_PALETTES;
    paletteOffset = 0;
    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
            TintBgPaletteEntries(paletteOffset, 16, blendCoeff);
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void ApplyDayNightTintToMapPalettes(void)
{
    ApplyDayNightTintToMapPalettesInternal();
}

void ApplyDayNightTintToMapPalettesForMapLoad(void)
{
    ApplyDayNightTintToMapPalettesInternal();
}

// Called from LoadMapFromCameraTransition immediately after
// LoadSecondaryTilesetPalette writes raw (day) colors into
// gPlttBufferUnfaded[slots 7-12] AND gPlttBufferFaded[slots 7-12].
//
// Unlike the full-map load path, the VBlank callback is ACTIVE during a
// camera-scroll route transition (no fade to black), so the first VBlank
// after LoadSecondaryTilesetPalette would copy the raw day palette to CGRAM
// and cause a visible flash.  This function tints those slots before that
// VBlank fires.
//
// Calling before ApplyWeatherGammaShiftToPal is intentional: that function
// reads gPlttBufferUnfaded (now tinted) and writes the combined
// weather+night result to gPlttBufferFaded, which is the correct behavior.
void ApplyDayNightTintToSecondaryMapPalette(void)
{
    u32 selectedPalettes;
    u16 paletteOffset;
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;
    EnsureVisualStateInitialized();
    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;

    // Mark the BG step as applied for the whole map so UpdateDayNightVisuals
    // does not schedule an unnecessary full-palette reload.
    sLastAppliedBgStep = sCurrentTransitionStep;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    selectedPalettes = DAY_NIGHT_SECONDARY_BG_PALETTES;
    paletteOffset = 0;
    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
            TintBgPaletteEntries(paletteOffset, 16, blendCoeff);
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

// Called by PatchObjectPalette right after loading raw OBJ palette data.
//
// Pokeplatinum timing insight: objects are created (with palettes) while the
// screen is black, before the fade-in starts.  On GBA the equivalent is to
// write the tinted palette to BOTH buffers here, while the VBlank callback is
// cleared (FieldClearVBlankHBlankCallbacks was called at warp start).
//
// Writing to BOTH buffers ensures correctness in every load path:
//  - Warp path: FadeInFromBlack zeroes faded before VBlank is restored, so
//    the intermediate tinted-faded value is never seen; the fade-in then
//    naturally reveals unfaded = tinted.
//  - Battle-return path (wild encounter): there is no FadeInFromBlack, so
//    when VBlank restores it copies faded = tinted to CGRAM immediately
//    (this was the missed case in the previous attempt).
//  - Both paths end with gPlttBufferUnfaded tinted, so normal-palette
//    fade-ins always converge on the correct colour.
void ApplyDayNightTintToObjPaletteSlot(u8 paletteSlot)
{
    u16 base = OBJ_PLTT_ID(paletteSlot);
    u8 blendCoeff;
    u16 i;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;
    EnsureVisualStateInitialized();
    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;

    // Capture the raw ROM colours that LoadPalette just placed in unfaded.
    for (i = 0; i < 16; i++)
        sObjPaletteBackup[paletteSlot][i] = gPlttBufferUnfaded[base + i];
    sObjSlotHasBackup[paletteSlot] = TRUE;
    // Mark this slot as synchronised with the current step so
    // ReapplyAllObjTint only fires when the step actually changes later.
    sLastAppliedObjStep = sCurrentTransitionStep;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return; // Daytime: leave both buffers holding the raw day colours.

    // Write tint to BOTH buffers.
    // VBlank is cleared during map loading so there is no flash risk.
    // FadeInFromBlack (called later in the state machine) will zero faded
    // before the VBlank callback is restored, so the warp path is also safe.
    TintObjSlotFromBackup(paletteSlot, blendCoeff);
}

void ApplyDayNightTintToBattlePaletteRange(u16 paletteOffset, u16 count)
{
    u8 blendCoeff;

    if (!TryGetBattleNightBlendCoeff(&blendCoeff))
        return;

    TintBgPaletteEntries(paletteOffset, count, blendCoeff);
}

void UpdateDayNightVisuals(void)
{
    u8 timeOfDay;
    u8 mapType;
    bool8 isOutdoors;
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;

    // Freeze the cycle at the exact palette we were showing when the warp
    // started.  This prevents a mid-warp step tick from re-tinting objects while
    // old sprites are still on-screen and keeps the palette stable until the
    // fade completes.
    if (sWarpInProgress)
        return;

    timeOfDay = GetDayNightTimeOfDay();
    mapType = GetCurrentMapType();
    isOutdoors = IsMapTypeOutdoors(mapType);

    sTargetTransitionStep = (timeOfDay == EVO_TIME_NIGHT) ? DAY_NIGHT_TRANSITION_STEPS : 0;

    if (!sVisualStateInitialized)
    {
        sCurrentTransitionStep = sTargetTransitionStep;
        sVisualStateInitialized = TRUE;
        return;
    }

    if (sCurrentTransitionStep != sTargetTransitionStep)
    {
        if (++sTransitionFrameCounter >= DAY_NIGHT_STEP_FRAMES)
        {
            sTransitionFrameCounter = 0;

            if (sCurrentTransitionStep < sTargetTransitionStep)
                sCurrentTransitionStep++;
            else
                sCurrentTransitionStep--;
        }
    }
    else
    {
        sTransitionFrameCounter = 0;
    }

    if (!isOutdoors)
        return;

    // Skip BG reload while sWarpTransitionSuppressed: the new map's unfaded
    // palette was already tinted at map-load time by
    // ApplyDayNightTintToMapPalettesForMapLoad.
    if (sWarpTransitionSuppressed)
        return;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);

    // BG: reload from ROM and re-bake when the step changes mid-game.
    // bufferTransferDisabled prevents VBlank from copying the brief raw-
    // palette state (after LoadMapTilesetPalettes and before the tint is
    // applied) to CGRAM, eliminating the one-frame daytime flash (bug #3).
    if (sCurrentTransitionStep != sLastAppliedBgStep && !gPaletteFade.active)
    {
        bool8 prevDisabled = gPaletteFade.bufferTransferDisabled;
        gPaletteFade.bufferTransferDisabled = TRUE;
        LoadMapTilesetPalettes(gMapHeader.mapLayout);
        ApplyDayNightTintToMapPalettesInternal();
        gPaletteFade.bufferTransferDisabled = prevDisabled;
    }

    // OBJ: re-tint from raw backup whenever the step changes.
    // Using the backup (not gPlttBufferUnfaded which is now tinted) means the
    // blend formula always starts from the original ROM colours, keeping
    // day<->night transitions and intermediate step levels correct.
    // Skip during active fades (palette-fade controls gPlttBufferFaded) and
    // during warp transitions (sWarpInProgress keeps OBJ faded black between
    // fade-out complete and new-map fade-in, preventing old-sprite flashes).
    // Also skip during the wild encounter intro BlendPalettes pulse.
    if (!gPaletteFade.active && !sWarpInProgress && !IsWildBattleTransitionIntroActive())
    {
        if (sCurrentTransitionStep != sLastAppliedObjStep)
            ReapplyAllObjTint(blendCoeff);
    }
}
