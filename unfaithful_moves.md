# Gen 4+ moves not currently expansion-faithful

This table lists Gen 4+ moves that currently do not behave fully like pokeemerald-expansion in this branch.

| Move | Non-faithful area | What is missing |
|---|---|---|
| `MOVE_ROOST` | Missing script command plumbing | Needs `setroost` command path support. |
| `MOVE_GRAVITY` | Missing script command plumbing | Needs `setgravity` command path support. |
| `MOVE_MIRACLE_EYE` | Missing volatile-status plumbing | Expansion uses volatile checks/setters (`jumpifvolatile`, `setvolatile`). |
| `MOVE_HEALING_WISH` | Missing switch-in/event plumbing | Needs commands used by expansion healing-wish flow (`storehealingwish`, switch-in event hooks, related helpers). |
| `MOVE_TAILWIND` | Missing side-condition plumbing | Needs `settailwind` support and related target loop helpers. |
| `MOVE_ACUPRESSURE` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_EMBARGO` | Missing script command plumbing | Needs `setembargo` support. |
| `MOVE_PSYCHO_SHIFT` | Missing status-transfer plumbing | Needs `trypsychoshift` + related safeguard/synchronize handling. |
| `MOVE_HEAL_BLOCK` | Missing volatile-status plumbing | Needs heal-block volatile timer path. |
| `MOVE_POWER_TRICK` | Missing stat-swap plumbing | Needs `powertrick` command behavior. |
| `MOVE_GASTRO_ACID` | Missing ability-suppression plumbing | Needs dedicated gastro-acid/ability update flow. |
| `MOVE_LUCKY_CHANT` | Missing side-condition plumbing | Needs `setluckychant` support. |
| `MOVE_POWER_SWAP` | Missing stat-stage swap plumbing | Needs `swapstatstages` behavior. |
| `MOVE_GUARD_SWAP` | Missing stat-stage swap plumbing | Needs `swapstatstages` behavior. |
| `MOVE_WORRY_SEED` | Missing ability-overwrite plumbing | Needs full overwrite-ability flow (`tryoverwriteability` + ability refresh hooks). |
| `MOVE_TOXIC_SPIKES` | Missing hazard command plumbing | Needs `settoxicspikes` support. |
| `MOVE_HEART_SWAP` | Missing stat-stage swap plumbing | Needs `swapstatstages` behavior. |
| `MOVE_AQUA_RING` | Missing volatile-status plumbing | Needs aqua-ring volatile set/check flow. |
| `MOVE_MAGNET_RISE` | Missing volatile-status plumbing | Needs magnet-rise volatile set/check flow. |
| `MOVE_DEFOG` | Missing defog command plumbing | Needs `trydefog` path (hazard/screen clearing behavior). |
| `MOVE_TRICK_ROOM` | Missing room command plumbing | Needs `setroom`/room-state handling for turn-order inversion. |
| `MOVE_CAPTIVATE` | Missing move-specific effect flow | Still routed through generic behavior; expansion uses dedicated stat-change logic. |
| `MOVE_STEALTH_ROCK` | Missing hazard command plumbing | Needs `setstealthrock` support. |
| `MOVE_LUNAR_DANCE` | Missing switch-in/event plumbing | Same missing infrastructure as Healing Wish path. |
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

