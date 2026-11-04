#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

#include "global.h"

u8 GetDayNightTimeOfDay(void);
bool8 IsNightTime(void);
bool8 IsDayTime(void);
void UpdateDayNightVisuals(void);
void ApplyDayNightTintToMapPalettes(void);
void ApplyDayNightTintToObjPaletteSlot(u8 paletteSlot);

#endif // GUARD_DAY_NIGHT_H
