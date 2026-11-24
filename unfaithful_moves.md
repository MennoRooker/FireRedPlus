# Gen 4+ moves not currently expansion-faithful

This table lists Gen 4+ moves that currently do not behave fully like pokeemerald-expansion in this branch.

## Implemented but with unfaithful/incomplete end-of-turn effects

| Move | Limitation | What is missing |
|---|---|---|
| `MOVE_ROOST` | STATUS3_ROOST bit set, but Flying-type suppression not yet hooked in type calc | Type-calc code must check STATUS3_ROOST and skip the user's Flying type for that turn. |
| `MOVE_GRAVITY` | STATUS_FIELD_GRAVITY bit set and gGravityTimer tracks duration, but grounding mechanics not enforced | Damage-calc / accuracy / switching code must check STATUS_FIELD_GRAVITY; timer must decrement each turn. |
| `MOVE_MIRACLE_EYE` | STATUS3_MIRACLE_EYE bit set, but Psychic-vs-Dark immunity bypass not yet hooked in type calc | Type-calc must check STATUS3_MIRACLE_EYE and allow Psychic to hit Dark types. |
| `MOVE_HEALING_WISH` / `MOVE_LUNAR_DANCE` | Attacker faints and switches in a new party member, but the switch-in HP restore / status cure does not trigger | Switch-in effects must check gBattleStruct->field_8C for pending heal and restore the new mon's HP/status. |
| `MOVE_TAILWIND` | SIDE_STATUS_TAILWIND bit set and gTailwindTimers tracks duration, but speed doubling not yet applied | Speed calculation must double speed for Pokémon on the tailwind side; timer must decrement each turn. |
| `MOVE_EMBARGO` | STATUS3_EMBARGO bit set and gEmbargoTimers tracks duration, but item-use blocking not yet enforced | Item-use and held-item activation checks must check STATUS3_EMBARGO; timer must decrement each turn. |
| `MOVE_HEAL_BLOCK` | STATUS3_HEAL_BLOCK bit set, but healing-move blocking not yet enforced | Heal-move usage checks must check STATUS3_HEAL_BLOCK; no timer (lasts until switch-out). |
| `MOVE_POWER_TRICK` | Atk/Def values swapped and STATUS3_POWER_TRICK toggled, but no end-of-battle / switch-out restoration | On switch-out, Atk and Def must be restored if STATUS3_POWER_TRICK is set. |
| `MOVE_GASTRO_ACID` | STATUS3_GASTRO_ACID bit set, but ability-suppression not yet hooked in ability checks | All ability-activation checks must consult STATUS3_GASTRO_ACID and skip the ability if set. |
| `MOVE_LUCKY_CHANT` | SIDE_STATUS_LUCKY_CHANT bit set and gLuckyChantTimers tracks duration, but crit blocking not yet hooked | Critical-hit calculation must check SIDE_STATUS_LUCKY_CHANT on the target's side; timer must decrement. |
| `MOVE_AQUA_RING` | STATUS3_AQUA_RING bit set, but per-turn HP restore not yet added to end-of-turn effects | End-of-turn processing must check STATUS3_AQUA_RING and restore 1/16 HP. |
| `MOVE_MAGNET_RISE` | STATUS3_MAGNET_RISE bit set and gMagnetRiseTimers tracks duration, but Ground-type immunity not yet hooked | Type-calc must check STATUS3_MAGNET_RISE and make the user immune to Ground; timer must decrement. |
| `MOVE_DEFOG` | Clears SIDE_STATUS hazard bits on the opponent's side; also lowers Evasion | Defog does not clear screens (Reflect / Light Screen) or the user's side hazards. |
| `MOVE_TRICK_ROOM` | STATUS_FIELD_TRICK_ROOM toggled and gTrickRoomTimer tracks duration | Turn-order sorting must check STATUS_FIELD_TRICK_ROOM and invert speed comparisons; timer must decrement. |
| `MOVE_STEALTH_ROCK` | SIDE_STATUS_STEALTH_ROCK bit set, but switch-in Rock-type damage not yet applied | Switch-in code must check SIDE_STATUS_STEALTH_ROCK and deal type-effectiveness-based damage. |
| `MOVE_TOXIC_SPIKES` | SIDE_STATUS_TOXIC_SPIKES bit set and gToxicSpikesCount tracks layers, but switch-in poisoning not yet applied | Switch-in code must check SIDE_STATUS_TOXIC_SPIKES and apply poison/bad-poison accordingly. |
| `MOVE_TELEKINESIS` | STATUS3_TELEKINESIS bit set, but always-hit / Ground immunity not yet hooked | Accuracy check must bypass misses; type-calc must check STATUS3_TELEKINESIS for Ground immunity. |
| `MOVE_CAPTIVATE` | Lowers Sp. Atk by 2 on the target; gender check runs in C via VARIOUS | No additional limitations; faithful as-implemented. |
| `MOVE_PSYCHO_SHIFT` | Transfers status to target and cures the user; Safeguard checked | No status-gained message is printed (uses animation only); minor UI unfaithfulness. |
| `MOVE_ACUPRESSURE` | Randomly raises one stat by 2; stat chosen via VARIOUS | No graphical stat-change animation. |
| `MOVE_POWER_SWAP` / `MOVE_GUARD_SWAP` / `MOVE_HEART_SWAP` | Stat stages swapped via VARIOUS | Faithful behavior; no known omissions. |
| `MOVE_GUARD_SPLIT` / `MOVE_POWER_SPLIT` | Base stats averaged between user and target via VARIOUS | No string message ("shared its guard/power" text missing). |
| `MOVE_WORRY_SEED` / `MOVE_SIMPLE_BEAM` | Target's ability overwritten via VARIOUS; ability-activation side-effects not triggered | Ability switch-in effects (e.g. Intimidate from new ability) are not fired. |
| `MOVE_ENTRAINMENT` | Same as Worry Seed/Simple Beam path | Same omissions. |
| `MOVE_SOAK` | Target's types changed to Water-only via VARIOUS | Faithful behavior; no known omissions. |
| `MOVE_REFLECT_TYPE` | User copies the target's types via VARIOUS | Faithful behavior; no known omissions. |
| `MOVE_TOPSY_TURVY` | Inverts all of the target's stat stages via VARIOUS | Faithful behavior; no known omissions. |

## Still stubbed / not implemented

| Move | Reason |
|---|---|
| `MOVE_STICKY_WEB` | SIDE_STATUS_STICKY_WEB bit defined; switch-in Speed-drop code not yet written. |
| `MOVE_AFTER_YOU` | Requires turn-order manipulation; no infrastructure. |
| `MOVE_ALLY_SWITCH` | Requires battler-position swap logic. |
| `MOVE_FAIRY_LOCK` | Requires field-wide no-switching condition; no infrastructure. |
| `MOVE_FLOWER_SHIELD` | Multi-target Grass-type Def boost; no VARIOUS handler yet. |
| `MOVE_ROTOTILLER` | Multi-target Grass-type Atk/SpAtk boost; no VARIOUS handler yet. |
| `MOVE_QUASH` | Requires turn-order manipulation; no infrastructure. |
| `MOVE_ELECTRIFY` | Requires dynamic move-type override on the target; no infrastructure. |
| `MOVE_TERRAIN` | Requires field terrain condition; no infrastructure. |
| `MOVE_WONDER_ROOM` / `MOVE_MAGIC_ROOM` | Require field room conditions; no infrastructure. |
| `MOVE_BESTOW` | Requires item-transfer logic; no VARIOUS handler yet. |
| `MOVE_POWDER` | Requires volatile status to block Fire moves and reflect damage. |
| `MOVE_DARK_VOID` | Routes to BattleScript_EffectSleep as a placeholder; multi-target sleep not yet enforced. |
| `MOVE_SMACK_DOWN` | Routes to BattleScript_EffectHit; aerial-target grounding not yet enforced. |
| `MOVE_THIRD_TYPE` | Requires a third type slot or type-override logic; no infrastructure. |
| `MOVE_TRICKS_OR_TREAT` / `MOVE_FORESTS_CURSE` | Same as Third Type. |

## Intentional deviations

| Move | Intentional area | Notes |
|---|---|---|
| `MOVE_QUIVER_DANCE` | Custom implementation | Intentional branch policy override; differs from expansion baseline. |
| `MOVE_BULLDOZE` | Custom effect path | Intentional deviation; uses project-specific effect routing. |
| `MOVE_FIRE_FANG` | Custom statline | Custom 75/100/20 statline kept. |
| `MOVE_THUNDER_FANG` | Custom statline | Custom 75/100/20 statline kept. |
| `MOVE_ICE_FANG` | Custom statline | Custom 75/100/20 statline kept. |
| `MOVE_POISON_FANG` | Custom behavior | Aligned to custom fang-style behavior. |
| `MOVE_STORM_THROW` | Missing metadata | No `alwaysCriticalHit` metadata field in BattleMove struct. |
| `MOVE_FROST_BREATH` | Missing metadata | Same as Storm Throw. |
| `MOVE_CHIP_AWAY` | Missing metadata | No "ignore Defense stage" metadata field. |
| `MOVE_SACRED_SWORD` | Missing metadata | Same as Chip Away. |

| `MOVE_DARK_VOID` | Missing non-volatile status command plumbing | Needs expansion non-volatile status path (`setnonvolatilestatus` / `trynonvolatilestatus`). |
| `MOVE_GUARD_SPLIT` | Missing stat-average plumbing | Needs `averagestats` behavior. |
| `MOVE_POWER_SPLIT` | Missing stat-average plumbing | Needs `averagestats` behavior. |
| `MOVE_WONDER_ROOM` | Missing room command plumbing | Needs `setroom`-style room-state support for Wonder Room. |
| `MOVE_AUTOTOMIZE` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_TELEKINESIS` | Missing telekinesis command plumbing | Needs `settelekinesis` support. |
| `MOVE_MAGIC_ROOM` | Missing room command plumbing | Needs `setroom`-style room-state support for Magic Room. |
| `MOVE_SOAK` | Missing type-overwrite plumbing | Needs `trysoak` behavior. |
| `MOVE_SIMPLE_BEAM` | Missing ability-overwrite plumbing | Needs full overwrite-ability flow (`tryoverwriteability` + ability refresh hooks). |
| `MOVE_ENTRAINMENT` | Missing ability-transfer plumbing | Needs `tryentrainment` + ability refresh hooks. |
| `MOVE_AFTER_YOU` | Missing turn-order command plumbing | Needs `tryafteryou` support. |
| `MOVE_ALLY_SWITCH` | Missing ally-switch command plumbing | Needs `tryallyswitch` and battler-swap helper support. |
| `MOVE_HEAL_PULSE` | Missing heal-pulse command plumbing | Needs `tryhealpulse` support (+ related volatile checks). |
| `MOVE_QUASH` | Missing turn-order command plumbing | Needs `tryquash` support. |
| `MOVE_REFLECT_TYPE` | Missing type-copy command plumbing | Needs `tryreflecttype` support. |
| `MOVE_BESTOW` | Missing item-transfer command plumbing | Needs `trybestow` + related item/ability helper flow. |
| `MOVE_ROTOTILLER` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_STICKY_WEB` | Missing hazard command plumbing | Needs `setstickyweb` support. |
| `MOVE_TRICK_OR_TREAT` | Missing third-type command plumbing | Needs `trythirdtype` support. |
| `MOVE_FORESTS_CURSE` | Missing third-type command plumbing | Needs `trythirdtype` support. |
| `MOVE_PARTING_SHOT` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change + pivot flow. |
| `MOVE_TOPSY_TURVY` | Missing stat-inversion plumbing | Needs `invertstatstages` support. |
| `MOVE_FLOWER_SHIELD` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_GRASSY_TERRAIN` | Missing terrain command plumbing | Needs `setterrain` support. |
| `MOVE_MISTY_TERRAIN` | Missing terrain command plumbing | Needs `setterrain` support. |
| `MOVE_ELECTRIFY` | Missing electrify command plumbing | Needs `tryelectrify` support. |
| `MOVE_FAIRY_LOCK` | Missing trapping-field command plumbing | Needs `trysetfairylock` support. |
| `MOVE_POWDER` | Missing volatile-status plumbing | Needs powder volatile set/check flow. |
| `MOVE_GEOMANCY` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change/two-turn logic. |
| `MOVE_MAGNETIC_FLUX` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_ELECTRIC_TERRAIN` | Missing terrain command plumbing | Needs `setterrain` support. |
| `MOVE_STORM_THROW` | Metadata mismatch | Expansion marks this as always-critical (`alwaysCriticalHit = TRUE`); this project lacks that direct move metadata path. |
| `MOVE_FROST_BREATH` | Metadata mismatch | Expansion marks this as always-critical (`alwaysCriticalHit = TRUE`); this project lacks that direct move metadata path. |
| `MOVE_CHIP_AWAY` | Metadata mismatch | Expansion ignores target Defense/Evasion stage changes; this project lacks that direct move metadata path. |
| `MOVE_SACRED_SWORD` | Metadata mismatch | Expansion ignores target Defense/Evasion stage changes; this project lacks that direct move metadata path. |
| `MOVE_QUIVER_DANCE` | Intentional custom override | Custom implementation/data differs from expansion baseline (kept by branch policy). |
| `MOVE_BULLDOZE` | Intentional custom override | Uses this project’s custom effect path instead of expansion’s exact model. |
| `MOVE_FIRE_FANG` | Intentional custom override | Kept custom 75/100/20 statline (expansion baseline differs). |
| `MOVE_THUNDER_FANG` | Intentional custom override | Kept custom 75/100/20 statline (expansion baseline differs). |
| `MOVE_ICE_FANG` | Intentional custom override | Kept custom 75/100/20 statline (expansion baseline differs). |
| `MOVE_POISON_FANG` | Intentional custom override | Intentionally aligned to fang-style custom behavior/statline, diverging from expansion model. |

