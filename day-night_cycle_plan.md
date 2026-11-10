# Day/Night Cycle Plan (no RTC)

## Scope and architecture
- **No RTC!** Day-night cycle will be implemented based on playtime.
- All cycle + tint logic lives in:
  - [day_night.h](/home/mennorooker/projects/Pokémon/pokefirered/include/day_night.h)
  - [day_night.c](/home/mennorooker/projects/Pokémon/pokefirered/src/day_night.c)
- Evolution logic should query this module (day/night gameplay state), not RTC APIs.

## Current status
Visual work at the front of implementation:
- Night palette during battles
- Warp palette timing
- Route transition staleness
- Sprite/grass interaction regressions
- Encounter-related palette bleed fixes

## Target gameplay clock
- Platinum-style buckets:
  - Morning: **04:00-10:00**
  - Day: **10:00-18:00**
  - Evening: **18:00-21:00** (visual twilight only; gameplay = day)
  - Night: **21:00-04:00**
- Time speed:
  - **2 real minutes = 1 in-game hour**
  - **48 real minutes = full 24h cycle**
- New save starts at **10:00 day** (offset from raw playtime).

## Phase order
1. **Stabilize visual system first** (already in progress)
   - Keep iterating in [day_night.c](/home/mennorooker/projects/Pokémon/pokefirered/src/day_night.c) for map/object/battle tint correctness.
2. **Finalize gameplay time buckets**
   - Add morning/evening visual states.
   - Keep gameplay checks as day/night (evening treated as day).
3. **Wire remaining evolution checks to day_night module**
   - Ensure all time-gated evolutions read from `GetDayNightTimeOfDay`-backed gameplay state.
4. **Encounter-time routing**
   - Add day/night (and optionally morning/evening visuals) encounter selection behavior.

## What to test now (short-cycle/dev profile)
Use the current short test cycle for fast validation:
- Outdoor tint flips trigger correctly.
- Indoor/cave maps stay untinted.
- No flashes on:
  - warps
  - route transitions
  - battle start/end
- Battle visuals:
  - night tint applies correctly in battle backgrounds/sprites where intended.
- Evolution behavior:
  - night-only evolutions succeed at night and fail during day windows.
- Tracker evolutions still work after visual/timing changes:
  - critical-hit tracker
  - used-move-X-times tracker
  - defeat tracker
  - recoil tracker (Basculegion path)

## What to test when switching to 48-minute target cycle
- Bucket boundaries exactly at:
  - 04:00, 10:00, 18:00, 21:00
- Evening is visual-only (gameplay still day).
- New save starts at virtual 10:00 consistently.
- Save/load preserves cycle position correctly.

## Potential follow-ups
- Add top-right cards when night/morning/day starts
- Sync in-game clocks to real life counterparts
- Add morning/night encounters to OVERVIEW
