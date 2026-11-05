#include "global.h"
#include "day_night.h"
#include "battle_transition.h"
#include "overworld.h"
#include "palette.h"
#include "fieldmap.h"
#include "constants/pokemon.h"
#include "constants/quest_log.h"
#include "constants/rgb.h"

#define DAY_NIGHT_CYCLE_MINUTES 2
#define DAY_NIGHT_DAY_MINUTES   1

#define DAY_NIGHT_BG_PALETTES       0x00001FFF
#define DAY_NIGHT_PALETTE_MASK      (DAY_NIGHT_BG_PALETTES | PALETTES_OBJECTS)
#define DAY_NIGHT_MAX_BLEND         5
#define DAY_NIGHT_TRANSITION_STEPS  5
#define DAY_NIGHT_STEP_FRAMES       30
#define DAY_NIGHT_NIGHT_COLOR       RGB(8, 10, 20)

static bool8 sVisualStateInitialized;
static u8 sCurrentTransitionStep;
static u8 sTargetTransitionStep;
static u8 sTransitionFrameCounter;
static u8 sLastAppliedBgStep;
static bool8 sWarpTransitionSuppressed;
static bool8 sPendingWarpReapply;

static u8 GetDayNightBlendForStep(u8 step)
{
    if (step >= DAY_NIGHT_TRANSITION_STEPS)
        return DAY_NIGHT_MAX_BLEND;

    return (step * DAY_NIGHT_MAX_BLEND + (DAY_NIGHT_TRANSITION_STEPS / 2)) / DAY_NIGHT_TRANSITION_STEPS;
}

// Writes the night blend into BOTH gPlttBufferUnfaded and gPlttBufferFaded.
// Used for BG palettes only: by writing to unfaded, UpdateNormalPaletteFade
// naturally interpolates from/to the tinted base during any warp transition.
static void TintBgPaletteEntries(u16 paletteOffset, u16 count, u8 blendCoeff, bool8 writeFaded)
{
    s32 nr = (s32)GET_R(DAY_NIGHT_NIGHT_COLOR);
    s32 ng = (s32)GET_G(DAY_NIGHT_NIGHT_COLOR);
    s32 nb = (s32)GET_B(DAY_NIGHT_NIGHT_COLOR);
    u16 i;

    for (i = 0; i < count; i++)
    {
        u16 index = paletteOffset + i;
        struct PlttData *src = (struct PlttData *)&gPlttBufferUnfaded[index];
        s32 r = src->r + (((s32)(nr - src->r) * blendCoeff) >> 4);
        s32 g = src->g + (((s32)(ng - src->g) * blendCoeff) >> 4);
        s32 b = src->b + (((s32)(nb - src->b) * blendCoeff) >> 4);
        u16 color = (u16)(r | (g << 5) | (b << 10));

        if (index == 0)
            continue;

        gPlttBufferUnfaded[index] = color;
        if (writeFaded)
            gPlttBufferFaded[index] = color;
    }
}

void DayNight_BeginWarpTransition(bool8 destinationIsOutdoors)
{
    sWarpTransitionSuppressed = destinationIsOutdoors;
    if (!destinationIsOutdoors)
        sPendingWarpReapply = FALSE;
}

void DayNight_EndWarpTransition(void)
{
    if (sWarpTransitionSuppressed)
        sPendingWarpReapply = TRUE;
    sWarpTransitionSuppressed = FALSE;
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

// Called immediately after LoadMapTilesetPalettes from any map-load path.
// Also initialises sCurrentTransitionStep on first call so the correct
// tint level is applied even on the very first map load.
static void ApplyDayNightTintToMapPalettesInternal(bool8 writeFaded)
{
    u32 selectedPalettes;
    u16 paletteOffset;
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;

    // Snap to current time-of-day on first call so the tint is correct
    // from the moment the game starts rather than waiting for the first
    // UpdateDayNightVisuals tick.
    if (!sVisualStateInitialized)
    {
        u32 totalMinutes = gSaveBlock2Ptr->playTimeHours * 60 + gSaveBlock2Ptr->playTimeMinutes;
        u8 timeOfDay = ((totalMinutes % DAY_NIGHT_CYCLE_MINUTES) < DAY_NIGHT_DAY_MINUTES)
                         ? EVO_TIME_DAY : EVO_TIME_NIGHT;
        sTargetTransitionStep = (timeOfDay == EVO_TIME_NIGHT) ? DAY_NIGHT_TRANSITION_STEPS : 0;
        sCurrentTransitionStep = sTargetTransitionStep;
        sVisualStateInitialized = TRUE;
    }

    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;

    // Always record the step that was current when this outdoor map was loaded,
    // even if blendCoeff is zero (daytime). UpdateDayNightVisuals compares
    // sCurrentTransitionStep against this value to detect when a mid-game
    // step change needs to be baked in.
    sLastAppliedBgStep = sCurrentTransitionStep;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    selectedPalettes = DAY_NIGHT_BG_PALETTES;
    paletteOffset = 0;
    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
            TintBgPaletteEntries(paletteOffset, 16, blendCoeff, writeFaded);
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

void ApplyDayNightTintToMapPalettes(void)
{
    ApplyDayNightTintToMapPalettesInternal(!sWarpTransitionSuppressed && !gPaletteFade.active);
}

void ApplyDayNightTintToMapPalettesForMapLoad(void)
{
    ApplyDayNightTintToMapPalettesInternal(FALSE);
}

// Called after PatchObjectPalette to prime the faded OBJ palette slot for
// the current night level.  gPlttBufferUnfaded is intentionally left raw so
// the per-frame BlendPalettes in UpdateDayNightVisuals always reads the
// correct base colour when re-applying or changing the blend level.
void ApplyDayNightTintToObjPaletteSlot(u8 paletteSlot)
{
    u16 base = OBJ_PLTT_ID(paletteSlot);
    s32 nr, ng, nb;
    u8 blendCoeff;
    u16 i;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;
    if (!sVisualStateInitialized)
        return;
    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;
    if (sWarpTransitionSuppressed || gPaletteFade.active)
        return;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    nr = (s32)GET_R(DAY_NIGHT_NIGHT_COLOR);
    ng = (s32)GET_G(DAY_NIGHT_NIGHT_COLOR);
    nb = (s32)GET_B(DAY_NIGHT_NIGHT_COLOR);

    for (i = 0; i < 16; i++)
    {
        u16 index = base + i;
        struct PlttData *src = (struct PlttData *)&gPlttBufferUnfaded[index];
        s32 r = src->r + (((s32)(nr - src->r) * blendCoeff) >> 4);
        s32 g = src->g + (((s32)(ng - src->g) * blendCoeff) >> 4);
        s32 b = src->b + (((s32)(nb - src->b) * blendCoeff) >> 4);
        gPlttBufferFaded[index] = (u16)(r | (g << 5) | (b << 10));
        // gPlttBufferUnfaded[index] is left unchanged (keeps raw ROM colour)
    }
}

void UpdateDayNightVisuals(void)
{
    u8 timeOfDay;
    u8 mapType;
    bool8 isOutdoors;
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
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

    if (sWarpTransitionSuppressed)
        return;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);

    if (sPendingWarpReapply && !gPaletteFade.active)
    {
        sPendingWarpReapply = FALSE;
        LoadMapTilesetPalettes(gMapHeader.mapLayout);
        ApplyDayNightTintToMapPalettesInternal(TRUE);
        if (!IsWildBattleTransitionIntroActive())
            BlendPalettes(PALETTES_OBJECTS, blendCoeff, DAY_NIGHT_NIGHT_COLOR);
        return;
    }

    // BG palettes: reload from ROM and re-bake whenever the applied step
    // lags behind the current step.  Using sLastAppliedBgStep (rather than
    // comparing with the previous frame's step) ensures that a step change
    // deferred by an active fade is still applied once the fade clears.
    if (sCurrentTransitionStep != sLastAppliedBgStep && !gPaletteFade.active)
    {
        LoadMapTilesetPalettes(gMapHeader.mapLayout);
        ApplyDayNightTintToMapPalettes();
    }

    // OBJ palettes: re-tint every frame so the player, NPCs, grass walk
    // animations, and other field-effect sprites all follow the cycle.
    // OBJ gPlttBufferUnfaded is kept raw (by ApplyDayNightTintToObjPaletteSlot),
    // so BlendPalettes always reads the correct base colour.
    // blendCoeff==0 during the day restores raw (day) colours, which handles
    // the night-to-day transition correctly.
    // Skip during active fades: the palette fade controls gPlttBufferFaded
    // directly; the tint is re-applied the first frame after the fade clears.
    // Also skip while the wild encounter intro is driving its own BlendPalettes
    // pulse so daytime intro visuals match vanilla behavior.
    if (!gPaletteFade.active && !IsWildBattleTransitionIntroActive())
        BlendPalettes(PALETTES_OBJECTS, blendCoeff, DAY_NIGHT_NIGHT_COLOR);
}
