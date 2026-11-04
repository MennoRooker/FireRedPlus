#include "global.h"
#include "day_night.h"
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

static u8 GetDayNightBlendForStep(u8 step)
{
    if (step >= DAY_NIGHT_TRANSITION_STEPS)
        return DAY_NIGHT_MAX_BLEND;

    return (step * DAY_NIGHT_MAX_BLEND + (DAY_NIGHT_TRANSITION_STEPS / 2)) / DAY_NIGHT_TRANSITION_STEPS;
}

// Applies the night blend in-place to count palette entries in BOTH
// gPlttBufferUnfaded and gPlttBufferFaded, reading the raw colour from
// gPlttBufferUnfaded.  Writing to unfaded is the key: UpdateNormalPaletteFade
// uses unfaded as its blend source, so any fade (in or out) will naturally
// interpolate from/to the tinted colours with no flash or snap.
static void TintPaletteEntries(u16 paletteOffset, u16 count, u8 blendCoeff)
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
        gPlttBufferUnfaded[index] = color;
        gPlttBufferFaded[index] = color;
    }
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
void ApplyDayNightTintToMapPalettes(void)
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

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    selectedPalettes = DAY_NIGHT_BG_PALETTES;
    paletteOffset = 0;
    while (selectedPalettes)
    {
        if (selectedPalettes & 1)
            TintPaletteEntries(paletteOffset, 16, blendCoeff);
        selectedPalettes >>= 1;
        paletteOffset += 16;
    }
}

// Called after PatchObjectPalette to tint one OBJ palette slot.
// Covers the player sprite and all NPCs.
void ApplyDayNightTintToObjPaletteSlot(u8 paletteSlot)
{
    u8 blendCoeff;

    if (gGlobalFieldTintMode != QL_TINT_NONE)
        return;
    if (!sVisualStateInitialized)
        return;
    if (!IsMapTypeOutdoors(GetCurrentMapType()))
        return;

    blendCoeff = GetDayNightBlendForStep(sCurrentTransitionStep);
    if (blendCoeff == 0)
        return;

    TintPaletteEntries(OBJ_PLTT_ID(paletteSlot), 16, blendCoeff);
}

void UpdateDayNightVisuals(void)
{
    u8 timeOfDay;
    u8 mapType;
    bool8 isOutdoors;
    u8 prevStep;

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

    prevStep = sCurrentTransitionStep;

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

    // When the transition step changes mid-game, reload the BG palette from ROM
    // (to recover the raw colours) and re-bake the new tint level into unfaded.
    // Only do this when no fade is active to avoid interfering with transitions.
    if (isOutdoors && sCurrentTransitionStep != prevStep && !gPaletteFade.active)
    {
        LoadMapTilesetPalettes(gMapHeader.mapLayout);
        ApplyDayNightTintToMapPalettes();
    }
}
