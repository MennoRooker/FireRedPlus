# Day/Night Cycle Plan (Playtime-based, no RTC)

## Goal
Implement day/night-dependent evolution conditions for the remaining blocked evolutions (sections 2 and 3 in [evolutions.md](/home/mennorooker/projects/Pokémon/pokefirered/evolutions.md)) by adding a simple **playtime-driven** time-of-day system (no hardware RTC required).

---

## What pokeemerald-expansion does (reference model)

### 1) Time-of-day source and buckets
- Time buckets are defined in [constants/rtc.h](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/include/constants/rtc.h) (`TIME_MORNING`, `TIME_DAY`, `TIME_EVENING`, `TIME_NIGHT`), with generation-specific hour windows.
- `GetTimeOfDay()` in [rtc.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/rtc.c) calls `UpdateTimeOfDay()`.
- `UpdateTimeOfDay()` in [overworld.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/overworld.c) classifies the current hour into a bucket.

### 2) Evolution condition integration
- Evolution condition checking is centralized in [pokemon.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/pokemon.c).
- `IF_TIME` / `IF_NOT_TIME` are data-driven conditions interpreted there.
- Species data uses these conditions directly (e.g. Gligar/Sneasel/Budew/Chingling/Happiny/Linoone-Galar/etc. in `src/data/pokemon/species_info/*_families.h`).

### 3) Sirfetch’d critical-hit condition
- `IF_CRITICAL_HITS_GE` is evaluated in [pokemon.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/pokemon.c).
- Per-party critical-hit counters are:
  - incremented when a player mon lands a crit in [battle_util.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/battle_util.c)
  - reset at battle start in [battle_main.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/battle_main.c)

---

## FireRedPlus current state

- Time-based evolution methods/conditions exist in constants/data, but logic is intentionally disabled:
  - `EVO_FRIENDSHIP_DAY` and `EVO_FRIENDSHIP_NIGHT` are commented in [pokemon.c](/home/mennorooker/projects/Pokémon/pokefirered/src/pokemon.c)
  - `EVO_CONDITION_TIME_OF_DAY` currently returns `FALSE` in [pokemon.c](/home/mennorooker/projects/Pokémon/pokefirered/src/pokemon.c)
- Several species already point to time-of-day conditions in [evolution.h](/home/mennorooker/projects/Pokémon/pokefirered/src/data/pokemon/evolution.h) (e.g. Alolan Rattata, Gligar, Sneasel), but cannot evolve yet because condition evaluation is stubbed.
- `PlayTimeCounter_Update()` already updates save playtime every second in [play_time.c](/home/mennorooker/projects/Pokémon/pokefirered/src/play_time.c).
- FRLG RTC script commands are stubbed in [scrcmd.c](/home/mennorooker/projects/Pokémon/pokefirered/src/scrcmd.c), confirming no live RTC path.

---

## Proposed implementation: playtime-based day/night

## Design decision
Use a **virtual clock derived from playtime** (deterministic, save-compatible, no RTC).

Recommended v1 scope:
- support only `DAY`/`NIGHT` (enough for current blocked evolutions)
- no overworld tint/lighting changes yet (evolution logic first)

## Core rule
- Compute total played minutes from save:
  - `totalMinutes = playTimeHours * 60 + playTimeMinutes`
- Map into a repeating cycle:
  - `phase = totalMinutes % DAY_NIGHT_CYCLE_MINUTES`
- Classify:
  - `DAY` if `phase` in day window
  - `NIGHT` otherwise

Suggested defaults (tunable constants):
- `DAY_NIGHT_CYCLE_MINUTES = 60` (full cycle every 1 real play hour)
- `DAY` window: first 30 minutes
- `NIGHT` window: next 30 minutes

This keeps things simple and predictable while avoiding RTC.

---

## Planned code changes

### A) Add a small day/night helper module
- New files:
  - `include/day_night.h`
  - `src/day_night.c`
- API:
  - `u8 GetDayNightTimeOfDay(void);` (`EVO_TIME_DAY` or `EVO_TIME_NIGHT`)
  - `bool8 IsNightTime(void);`
  - `bool8 IsDayTime(void);`

### B) Wire evolution logic to helper
- Update [pokemon.c](/home/mennorooker/projects/Pokémon/pokefirered/src/pokemon.c):
  - implement `EVO_CONDITION_TIME_OF_DAY`
  - re-enable `EVO_FRIENDSHIP_DAY` and `EVO_FRIENDSHIP_NIGHT` using the helper (instead of RTC comments)

### C) Complete section 2 evolutions
- Already data-wired; will start working once time condition is live:
  - Alolan Rattata -> Alolan Raticate
  - Gligar -> Gliscor
  - Sneasel -> Weavile
- Add Sirfetch’d critical-hit tracker support:
  - add/reset per-party crit counters in battle init path
  - increment when player lands critical hit
  - evaluate `EVO_CONDITION_CRITICAL_HITS_GE` in evolution condition switch

### D) Start section 3 data wiring
- Add missing evolution entries in [evolution.h](/home/mennorooker/projects/Pokémon/pokefirered/src/data/pokemon/evolution.h) for:
  - Linoone (Galar) -> Obstagoon (night)
  - Budew -> Roselia (friendship + day)
  - Chingling -> Chimecho (friendship + night)
  - Happiny -> Chansey (Oval Stone + day)
  - Sneasel (Hisui) -> Sneasler (day + Razor Claw)
- Leave non-time tracker/spec-conditions as follow-up phase:
  - Ursaring -> Ursaluna
  - Stantler -> Wyrdeer
  - Primeape -> Annihilape
  - Bisharp -> Kingambit

---

## Compatibility and risk notes

- **No save-structure change required** for v1 (uses existing playtime fields).
- Known edge case: playtime caps at `999:59`; at cap, virtual time stops changing (acceptable for initial implementation, can be solved later with a dedicated uncapped counter if needed).
- Keeping v1 to evolution logic avoids broad overworld-side regressions.

---

## Validation plan

1. Build and ensure no compile errors.
2. Evolution checks:
   - force/test day phase -> verify day-only evolutions pass and night-only fail
   - force/test night phase -> inverse behavior
3. Battle condition check:
   - land 3 crits with Galarian Farfetch’d in one battle
   - confirm Sirfetch’d evolution triggers post-battle
4. Regression checks:
   - non-time evolutions unchanged
   - item/trade/friendship baseline behavior unchanged

---

## Implementation order (recommended)

1. Add day/night helper module.
2. Enable `EVO_CONDITION_TIME_OF_DAY` + friendship day/night logic.
3. Add/verify section 2 time-based evolutions.
4. Implement Sirfetch’d crit tracker.
5. Add section 3 time-based entries (Linoone-Galar/Budew/Chingling/Happiny/Sneasel-Hisui).
6. Separate follow-up for complex tracker/region conditions.

---

## Finalized follow-up integrations (outside core cycle logic)

These are not required to *create* the day/night clock, but should be implemented to make behavior consistent:

1. **Map expansion “morning” behavior to `DAY` in FireRedPlus**
   - Treat any mechanics that check morning as day in this project.
   - Example: move-healing behavior such as Morning Sun-style time checks should use `DAY` instead of requiring a separate morning bucket.
   - Similar simplification can be applied to any morning/evening bonus paths (map both to `DAY` where appropriate).

2. **Implement encounter-time routing (day/night encounter content)**
   - Add/port logic that determines which encounter set is active at day vs night.
   - This is a separate system from the clock itself, but important so day/night has visible gameplay impact beyond evolutions.
   - Keep this as a dedicated follow-up workstream after the evolution/time-condition core is merged.

3. **Later phase: Add NDS-like overworld shading (DNS-style palette tinting)**
   - Source reference: `pokeemerald-expansion` DNS pipeline:
     - Time blend presets and bucket transitions in [overworld.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/overworld.c)
     - Palette mixing math in [palette.c](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/src/palette.c)
     - Config gate in [overworld.h config](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/include/config/overworld.h)
     - Usage notes and pitfalls in [docs/tutorials/dns.md](/home/mennorooker/projects/Pokémon/pokeemerald-expansion/docs/tutorials/dns.md)
   - Recommended FireRedPlus adaptation:
     1. Add a feature toggle (like `OW_ENABLE_DNS`) so visual tinting can be enabled/disabled without touching evolution logic.
     2. Keep a strict `DAY`/`NIGHT` model (no morning/evening buckets): treat all expansion morning/evening visuals as day-side behavior.
     3. Add a `MapHasNaturalLight`-style gate so tinting applies only to outdoor map types (town/city/route/ocean-route equivalents), not indoor/cave maps.
     4. Implement a lightweight `gTimeBlend` state (`startBlend`, `endBlend`, `weight`) and update only on time boundary changes or periodic checks.
     5. Apply tint only to map/object palettes (not UI palettes), matching expansion behavior (`PALETTES_MAP | PALETTES_OBJECTS` filtering).
   - Optional quality pass (from expansion DNS):
     - Alternate nighttime tileset palettes via `swapPalettes`, using night slot mapping `night_pal = (day_pal + 9) % 16`.
     - Per-color light blending using `.pla` files for windows/lamps/signs.
     - Light object events (`OBJ_EVENT_GFX_LIGHT_SPRITE`) for lamp glow effects.
   - Script/engine integration notes:
     - Use the outside-safe fade path equivalent to `fadescreenswapbuffers` where alpha blending and DNS tint overlap.
     - Recheck weather and popup alpha-blend interactions after enabling DNS-style tinting.
   - Validation targets for this phase:
     - Outdoor transitions: day <-> night tint appears and clears correctly.
     - Indoor/cave maps remain untinted.
     - No palette corruption in map transitions, weather changes, or object palette updates.
