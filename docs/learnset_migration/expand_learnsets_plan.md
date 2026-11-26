# Learnset expansion migration plan (preserve local edits)

## What is already saved

- Full vanilla-vs-current learnset diff has been moved here for safekeeping:
  - `docs/learnset_migration/learnsets_vanilla_diff.txt`

## Key findings from pokeemerald-expansion

- Level-up learnsets are generation-selectable and split by file:
  - `src/data/pokemon/level_up_learnsets/gen_5.h`
  - `src/data/pokemon/level_up_learnsets/gen_6.h`
- TMHM/tutor learnsets are generated there via helpers (not maintained as static hand-edited source in-repo by default).

## Recommended source generation (amended)

- Use **Gen 6** as the required level-up source baseline.
- Include tutor coverage from:
  - this project’s current tutor move set, and
  - expansion tutor-related move availability.
- Normalize move constant spellings directly in this project:
  - use `MOVE_FEINT_ATTACK` (rename from `MOVE_FAINT_ATTACK`)
  - use `MOVE_VISE_GRIP` (rename from `MOVE_VICE_GRIP`)

## Strategy to preserve your year of edits

Use a **3-way species-level merge**, not a raw line-based copy.

### A) Freeze your current customizations as an overlay

1. Keep this as baseline evidence:
   - `docs/learnset_migration/learnsets_vanilla_diff.txt`
2. Also create machine-usable patches (same idea, one patch per file) between:
   - `../pokefirered-vanilla/pokefirered/src/data/pokemon/*.h`
   - `src/data/pokemon/*.h`
3. Extract a species list you changed in each file (level-up, tmhm, tutor, pointers).

### B) Build an imported base from expansion (without your edits yet)

1. Create temporary generated files for this repo:
   - `src/data/pokemon/level_up_learnsets.imported.h`
   - `src/data/pokemon/tmhm_learnsets.imported.h`
   - `src/data/pokemon/tutor_learnsets.imported.h`
2. For level-up:
   - Parse expansion `gen_6.h`
   - Keep only species present in your project
   - Convert expansion format
     - from `struct LevelUpMove` entries
     - to FireRed format `LEVEL_UP_MOVE(level, MOVE_XXX)` + `LEVEL_UP_END`
3. For TMHM/tutor:
   - Build sets from expansion teachable source data
   - Merge tutor move availability from both projects (union source, then filtered for supported moves/constants)
   - Intersect with this project’s available machines/tutors and existing move constants
   - Reflect HM09 (Defog) and HM10 (Rock Climb) in TMHM learnsets during generation/merge
   - Apply the project-wide rename normalization above before/while merging learnsets.

### C) Re-apply your local edits on top (the preservation step)

Policy:

- If species was customized by you in the last year, **keep your current species entry** (or merge manually if you want both).
- If species was untouched by you, take imported Gen 6 data.

Apply this policy separately for:

- `level_up_learnsets.h`
- `tmhm_learnsets.h`
- `tutor_learnsets.h`
- `level_up_learnset_pointers.h` (only adjust if needed by species table changes)

### D) Validate and iterate

1. Build once after import-only.
2. Build again after custom overlay re-application.
3. Resolve unknown moves/macros by:
   - rename map
   - or dropping unsupported moves explicitly (never silently)
4. Spot-check customized species from your diff file to confirm preservation.

### E) HM09/HM10 propagation (amended)

1. Treat HM09 Defog and HM10 Rock Climb as first-class machine moves in migration scripts/merge logic.
2. Ensure `tmhm_learnsets.h` entries are regenerated/merged with HM09/HM10 considered.
3. Validate that species expected to learn these HMs have explicit TMHM bits set.

### F) Future additional TM expansion section (amended)

When you later add more TMs in `include/constants/items.h`:

1. Add TM constants/items first.
2. Extend TM bit mapping logic/macros if machine range expands.
3. Re-run the TMHM merge pipeline so new TMs can be assigned from:
   - imported expansion teachable data,
   - your project’s custom overrides.
4. Keep this policy for conflicts:
   - custom species edits win,
   - imported data fills untouched species.
5. Re-validate with build + species spot-checks on newly added TMs.

## Practical execution sequence (safe and reviewable)

1. Keep working on the current branch (no branch switch for this migration).
2. Commit 1: add imported base (no custom overlay)
3. Commit 2: reapply your custom overlay
4. Commit 3: cleanup/manual conflict resolutions
5. Final review with:
   - per-file diff
   - species-level changed list
   - compile success

## Why this avoids losing progress

- Your current edits are preserved as an explicit overlay sourced from vanilla-vs-current history.
- Import and customization are separated into independent commits, so regressions are easy to detect and revert.
- Species-level merge avoids destructive line-hunk collisions from reordered/expanded datasets.
