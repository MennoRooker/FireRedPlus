#!/usr/bin/env python3
"""
Import alternate-form species_info entries from pokeemerald-expansion into
this project's src/data/pokemon/species_info.h.

Scope: every species listed in the "Alternate forms" section of
include/constants/species.h (post-Unown forms).
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

from import_species_info_from_expansion import (
    DEFAULT_SOURCE_FILES,
    DEFAULT_SOURCE_ROOT,
    DEFAULT_SPECIES_HEADER,
    DEFAULT_SPECIES_INFO,
    DEFAULTS,
    REQUIRED_FIELDS,
    SpeciesBlock,
    build_entry,
    extract_blocks,
    extract_numeric_macros,
    load_known_constants,
    load_species_numbers_and_aliases,
)

ALT_FORMS_SECTION_RE = re.compile(r"//\s*Alternate forms(?P<body>.*)#endif", re.DOTALL)
FORM_DEFINE_RE = re.compile(r"^\s*#define\s+(SPECIES_[A-Z0-9_]+)\s+\(NUM_SPECIES\s*\+\s*(\d+)\)\s*$", re.MULTILINE)
NUM_SPECIES_ALIAS_RE = re.compile(r"^\s*#define\s+NUM_SPECIES\s+(SPECIES_[A-Z0-9_]+)\s*$", re.MULTILINE)
ENTRY_START_RE = re.compile(r"^\s*\[(SPECIES_[A-Z0-9_]+)\s*/\*\s*\d+\s*\*/\]\s*=\s*\{", re.MULTILINE)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source-root",
        type=Path,
        default=DEFAULT_SOURCE_ROOT,
        help="Path to pokeemerald-expansion root directory",
    )
    parser.add_argument(
        "--source-files",
        nargs="+",
        default=list(DEFAULT_SOURCE_FILES),
        help="Relative files under --source-root to parse",
    )
    parser.add_argument(
        "--species-header",
        type=Path,
        default=DEFAULT_SPECIES_HEADER,
        help="Path to include/constants/species.h in this project",
    )
    parser.add_argument(
        "--species-info",
        type=Path,
        default=DEFAULT_SPECIES_INFO,
        help="Path to src/data/pokemon/species_info.h in this project",
    )
    parser.add_argument("--write", action="store_true", help="Apply changes in species_info.h")
    return parser.parse_args()


def read_target_forms(species_header: Path) -> list[tuple[str, int]]:
    text = species_header.read_text(encoding="utf-8")
    species_numbers, _ = load_species_numbers_and_aliases(species_header)

    num_species_alias_match = NUM_SPECIES_ALIAS_RE.search(text)
    if not num_species_alias_match:
        raise ValueError("Could not resolve NUM_SPECIES alias in species.h")
    num_species_target = num_species_alias_match.group(1)
    if num_species_target not in species_numbers:
        raise ValueError(f"NUM_SPECIES target {num_species_target} has no numeric value")
    num_species_value = species_numbers[num_species_target]

    section_match = ALT_FORMS_SECTION_RE.search(text)
    if not section_match:
        raise ValueError("Could not find Alternate forms section in species.h")

    forms: list[tuple[str, int]] = []
    for species, offset in FORM_DEFINE_RE.findall(section_match.group("body")):
        forms.append((species, num_species_value + int(offset)))
    return forms


def parse_source_blocks(source_root: Path, source_files: list[str]) -> dict[str, SpeciesBlock]:
    known_abilities = load_known_constants(DEFAULT_SPECIES_HEADER.parent / "abilities.h", "ABILITY_")
    known_items = load_known_constants(DEFAULT_SPECIES_HEADER.parent / "items.h", "ITEM_")
    known_types = load_known_constants(DEFAULT_SPECIES_HEADER.parent / "pokemon.h", "TYPE_")
    known_egg_groups = load_known_constants(DEFAULT_SPECIES_HEADER.parent / "pokemon.h", "EGG_GROUP_")

    parsed_by_species: dict[str, SpeciesBlock] = {}
    for rel_path in source_files:
        source_path = source_root / rel_path
        if not source_path.exists():
            raise FileNotFoundError(f"Source file not found: {source_path}")
        text = source_path.read_text(encoding="utf-8")
        file_macros = extract_numeric_macros(text)
        global_macros = dict(file_macros)
        for block in extract_blocks(
            text,
            str(source_path),
            global_macros,
            known_abilities,
            known_items,
            known_types,
            known_egg_groups,
        ):
            parsed_by_species.setdefault(block.species, block)
    return parsed_by_species


def replace_or_insert_entries(species_info_text: str, entries_by_species: dict[str, str], ordered_species: list[str]) -> tuple[str, int, int]:
    replaced = 0
    missing_to_insert: list[str] = []
    out_text = species_info_text

    for species in ordered_species:
        replacement = entries_by_species[species]
        species_re = re.compile(rf"^\s*\[{re.escape(species)}\s*/\*\s*\d+\s*\*/\]\s*=\s*\{{", re.MULTILINE)
        m = species_re.search(out_text)
        if not m:
            missing_to_insert.append(species)
            continue

        brace_start = out_text.find("{", m.start())
        if brace_start == -1:
            missing_to_insert.append(species)
            continue

        depth = 0
        i = brace_start
        while i < len(out_text):
            ch = out_text[i]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    break
            i += 1
        if i >= len(out_text):
            missing_to_insert.append(species)
            continue

        end = i + 1
        while end < len(out_text) and out_text[end] in " \t":
            end += 1
        if end < len(out_text) and out_text[end] == ",":
            end += 1

        out_text = out_text[:m.start()] + replacement + out_text[end:]
        replaced += 1

    if missing_to_insert:
        closing_idx = out_text.rfind("\n};")
        if closing_idx == -1:
            raise ValueError("Could not find closing brace for gSpeciesInfo array")
        additions = "\n" + "\n".join(entries_by_species[s] for s in missing_to_insert) + "\n"
        out_text = out_text[:closing_idx] + additions + out_text[closing_idx:]

    return out_text, replaced, len(missing_to_insert)


def main() -> int:
    args = parse_args()

    if not args.species_header.exists():
        print(f"ERROR: Species header not found: {args.species_header}", file=sys.stderr)
        return 1
    if not args.species_info.exists():
        print(f"ERROR: Species info file not found: {args.species_info}", file=sys.stderr)
        return 1
    if not args.source_root.exists():
        print(f"ERROR: Source root not found: {args.source_root}", file=sys.stderr)
        return 1

    target_forms = read_target_forms(args.species_header)
    parsed_by_species = parse_source_blocks(args.source_root, list(args.source_files))

    replacements: dict[str, str] = {}
    unresolved: list[str] = []
    missing_required: list[str] = []
    for species, number in target_forms:
        block = parsed_by_species.get(species)
        if block is None and species == "SPECIES_CASTFORM_SANDY":
            block = parsed_by_species.get("SPECIES_CASTFORM_NORMAL")
        if block is None:
            unresolved.append(species)
            continue

        if any(req not in block.fields and req not in DEFAULTS for req in REQUIRED_FIELDS):
            missing_required.append(species)
            continue

        fields = dict(block.fields)
        if species == "SPECIES_CASTFORM_SANDY":
            fields["baseHP"] = "80"
            fields["baseAttack"] = "80"
            fields["baseDefense"] = "80"
            fields["baseSpAttack"] = "100"
            fields["baseSpDefense"] = "80"
            fields["baseSpeed"] = "80"
            fields["types"] = "{TYPE_ROCK, TYPE_GROUND}"
        replacements[species] = build_entry(species, number, fields)

    species_info_text = args.species_info.read_text(encoding="utf-8")
    ordered_species = [species for species, _ in target_forms if species in replacements]
    new_text, replaced_count, inserted_count = replace_or_insert_entries(species_info_text, replacements, ordered_species)

    print(f"Target alternate forms in species.h: {len(target_forms)}")
    print(f"Parsed alternate forms with usable data: {len(replacements)}")
    print(f"Existing entries replaced: {replaced_count}")
    print(f"New entries inserted: {inserted_count}")
    if unresolved:
        print(f"Unresolved forms (missing in source): {len(unresolved)}")
        for species in unresolved:
            print(f"  - {species}")
    if missing_required:
        print(f"Skipped forms (missing required fields): {len(missing_required)}")
        for species in missing_required:
            print(f"  - {species}")

    if not args.write:
        print("Dry-run only. Re-run with --write to apply changes.")
        return 0

    if new_text == species_info_text:
        print("No changes to write.")
        return 0

    args.species_info.write_text(new_text, encoding="utf-8")
    print(f"Wrote alternate form species data to: {args.species_info}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
