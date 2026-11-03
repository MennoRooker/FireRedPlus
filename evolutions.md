# Evolution Status Audit

## 1) Species that currently evolve only via trade / trade-held item

- Onix -> Steelix (trade while holding Metal Coat)
- Rhydon -> Rhyperior (trade while holding Protector)
- Scyther -> Scizor (trade while holding Metal Coat)
- Porygon -> Porygon2 (trade while holding Up-Grade)
- Porygon2 -> Porygon-Z (trade while holding Dubious Disc)
- Electabuzz -> Electivire (trade while holding Electirizer)
- Magmar -> Magmortar (trade while holding Magmarizer)
- Dusclops -> Dusknoir (trade while holding Reaper Cloth)
- Clamperl -> Huntail (trade while holding Deep Sea Tooth)
- Clamperl -> Gorebyss (trade while holding Deep Sea Scale)
- Boldore -> Gigalith (trade)
- Graveler (Alola) -> Golem (Alola) (trade)
- Gurdurr -> Conkeldurr (trade)

## 2) Species blocked by not-yet-implemented evolution condition logic

- Rattata (Alola) -> Raticate (Alola) (time-of-day condition)
- Gligar -> Gliscor (time-of-day condition)
- Sneasel -> Weavile (time-of-day condition)
- Farfetch'd (Galar) -> Sirfetch'd (critical-hit tracker condition)

## 3) Species present in `species.h` but omitted from `evolution.h` under deferred-condition scope

- Linoone (Galar) -> Obstagoon (time-of-day condition)
- Budew -> Roselia (friendship + time-of-day condition)
- Chingling -> Chimecho (friendship + time-of-day condition)
- Happiny -> Chansey (Oval Stone + time-of-day condition)
- Sneasel (Hisui) -> Sneasler (time-of-day condition)
- Ursaring -> Ursaluna (region/time-gated special item condition)
- Stantler -> Wyrdeer (used-move-X-times tracker condition)
- Primeape -> Annihilape (used-move-X-times tracker condition)
- Bisharp -> Kingambit (defeat-tracker condition)
