#!/usr/bin/env python3
"""
Add species numbers inside bracket comments in src/data/pokemon/species_info.h.

Example:
    [SPECIES_BULBASAUR] =
becomes:
    [SPECIES_BULBASAUR /* 1 */] =

Run from repo root:
    python3 scripts/print_species_numbers.py
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SPECIES_H = ROOT / "include/constants/species.h"
SPECIES_INFO_H = ROOT / "src/data/pokemon/species_info.h"


def load_species_numbers(path: Path) -> dict[str, int]:
    text = path.read_text(encoding="utf-8")
    # e.g. #define SPECIES_BULBASAUR 1
    pairs = re.findall(r"^\s*#define\s+(SPECIES_[A-Z0-9_]+)\s+(\d+)\b", text, re.MULTILINE)
    return {name: int(num) for name, num in pairs}


def annotate_species_info(path: Path, species_numbers: dict[str, int]) -> tuple[int, list[str]]:
    text = path.read_text(encoding="utf-8")
    unknown: list[str] = []
    changed = 0

    # Match lines like:
    # [SPECIES_X] =
    # [SPECIES_X /* 123 */] =
    pattern = re.compile(r"\[(SPECIES_[A-Z0-9_]+)(?:\s*/\*\s*\d+\s*\*/)?\](\s*=)")

    def repl(match: re.Match[str]) -> str:
        nonlocal changed
        species = match.group(1)
        suffix = match.group(2)

        if species not in species_numbers:
            unknown.append(species)
            return match.group(0)

        changed += 1
        return f"[{species} /* {species_numbers[species]} */]{suffix}"

    new_text = pattern.sub(repl, text)

    if new_text != text:
        path.write_text(new_text, encoding="utf-8")

    return changed, sorted(set(unknown))


def main() -> int:
    if not SPECIES_H.exists():
        print(f"ERROR: Not found: {SPECIES_H}", file=sys.stderr)
        return 1
    if not SPECIES_INFO_H.exists():
        print(f"ERROR: Not found: {SPECIES_INFO_H}", file=sys.stderr)
        return 1

    species_numbers = load_species_numbers(SPECIES_H)
    changed, unknown = annotate_species_info(SPECIES_INFO_H, species_numbers)

    print(f"Annotated entries: {changed}")
    print(f"Species defined in {SPECIES_H}: {len(species_numbers)}")
    if unknown:
        print("WARNING: Missing species numbers for:")
        for s in unknown:
            print(f"  - {s}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())