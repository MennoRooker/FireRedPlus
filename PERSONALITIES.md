# Trainer Pokémon Nature & Ability Overrides

This project adds new trainer party struct variants that let you explicitly set a Pokémon's nature and ability slot per trainer, without changing the existing variants.

## New Party Variants

Use these in `src/data/trainer_parties.h` to define trainer parties:

- No item, default moves: `struct TrainerMonNoItemDefaultMovesNatureAbility`
- Item, default moves: `struct TrainerMonItemDefaultMovesNatureAbility`
- No item, custom moves: `struct TrainerMonNoItemCustomMovesNatureAbility`
- Item, custom moves: `struct TrainerMonItemCustomMovesNatureAbility`

### Fields

Each entry contains:
- `iv`: fixed IV value (or `USE_RANDOM_IVS` semantics as existing)
- `lvl`: level
- `species`: species
- `heldItem`: only for the `Item*` variants
- `moves[MAX_MON_MOVES]`: only for the `*CustomMoves*` variants
- `nature`: desired nature (0..24)
- `abilitySlot`: desired ability slot (0 or 1)

Note: If a species only has one ability, `abilitySlot` is ignored.

## Macros to Attach Parties

In `src/data/trainers.h`, use these macros (analogous to existing ones) to attach your party arrays and set the correct flags:

- `NO_ITEM_DEFAULT_MOVES_NATURE_ABILITY(party)`
- `ITEM_DEFAULT_MOVES_NATURE_ABILITY(party)`
- `NO_ITEM_CUSTOM_MOVES_NATURE_ABILITY(party)`
- `ITEM_CUSTOM_MOVES_NATURE_ABILITY(party)`

These set the `F_TRAINER_PARTY_NATURE_ABILITY` flag in addition to existing flags, and the loader uses that to apply your overrides.

## How It Works

- Ability in Gen 3 is determined by personality parity. The loader composes a personality value that matches both the requested `nature` and `abilitySlot` for the species.
- For species with a single ability, the loader matches only the requested `nature`.
- The composed personality is used to create the Pokémon; held items and custom moves are applied as usual.

## Backward Compatibility

Existing trainer party variants and macros are untouched. If you do not use the new macros/structs, behavior remains identical.

## Example

```
static const struct TrainerMonItemCustomMovesNatureAbility sParty_LeaderExample[] = {
    {
        .iv = 200,
        .lvl = 42,
        .species = SPECIES_PIKACHU,
        .heldItem = ITEM_MAGNET,
        .moves = { MOVE_THUNDERBOLT, MOVE_QUICK_ATTACK, MOVE_IRON_TAIL, MOVE_PROTECT },
        .nature = NATURE_TIMID,
        .abilitySlot = 1,
    },
};

[TRAINER_LEADER_EXAMPLE] = {
    .trainerClass = TRAINER_CLASS_LEADER,
    .encounterMusic_gender = TRAINER_ENCOUNTER_MUSIC_INTENSE,
    .trainerName = _("EXAMPLE"),
    .doubleBattle = FALSE,
    .party = ITEM_CUSTOM_MOVES_NATURE_ABILITY(sParty_LeaderExample),
},
```

## Notes

- `abilitySlot` is 0/1 slot index, not ability ID. This preserves FRLG-accurate behavior.
- Explicit `nature` takes precedence over the original personality seeding (name/gender bias). If you need specific gender outcomes, set them via species constraints or adjust manually.
