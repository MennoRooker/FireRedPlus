#!/usr/bin/env python3
"""
Import species data from pokeemerald-expansion gen_n_families files into
this project's src/data/pokemon/species_info.h stubs.

Phase 1 default scope: Gen 4 National Dex entries (387-493).
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_SOURCE_ROOT = ROOT.parent / "pokeemerald-expansion"
DEFAULT_SOURCE_FILES = (
    "src/data/pokemon/species_info/gen_1_families.h",
    "src/data/pokemon/species_info/gen_2_families.h",
    "src/data/pokemon/species_info/gen_3_families.h",
    "src/data/pokemon/species_info/gen_4_families.h",
    "src/data/pokemon/species_info/gen_5_families.h",
    "src/data/pokemon/species_info/gen_6_families.h",
    "src/data/pokemon/species_info/gen_7_families.h",
    "src/data/pokemon/species_info/gen_8_families.h",
    "src/data/pokemon/species_info/gen_9_families.h",
)
DEFAULT_SPECIES_HEADER = ROOT / "include/constants/species.h"
DEFAULT_SPECIES_INFO = ROOT / "src/data/pokemon/species_info.h"

DEFAULTS = {
    "evYield_HP": "0",
    "evYield_Attack": "0",
    "evYield_Defense": "0",
    "evYield_SpAttack": "0",
    "evYield_SpDefense": "0",
    "evYield_Speed": "0",
    "itemCommon": "ITEM_NONE",
    "itemRare": "ITEM_NONE",
    "friendship": "70",
    "safariZoneFleeRate": "0",
    "noFlip": "FALSE",
}

REQUIRED_FIELDS = (
    "baseHP",
    "baseAttack",
    "baseDefense",
    "baseSpAttack",
    "baseSpDefense",
    "baseSpeed",
    "types",
    "catchRate",
    "expYield",
    "genderRatio",
    "eggCycles",
    "growthRate",
    "eggGroups",
    "abilities",
    "bodyColor",
)

FIELD_ORDER = (
    "baseHP",
    "baseAttack",
    "baseDefense",
    "baseSpAttack",
    "baseSpDefense",
    "baseSpeed",
    "types",
    "catchRate",
    "expYield",
    "evYield_HP",
    "evYield_Attack",
    "evYield_Defense",
    "evYield_SpAttack",
    "evYield_SpDefense",
    "evYield_Speed",
    "itemCommon",
    "itemRare",
    "genderRatio",
    "eggCycles",
    "friendship",
    "growthRate",
    "eggGroups",
    "abilities",
    "safariZoneFleeRate",
    "bodyColor",
    "noFlip",
)

ASSIGNMENT_RE = re.compile(r"^\s*\.(\w+)\s*=\s*(.+),\s*$")
BLOCK_START_RE = re.compile(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=\s*\{")
BLOCK_MACRO_RE = re.compile(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=\s*([A-Z0-9_]+)(?:\(([^)]*)\))?,\s*$")
SPECIES_DEFINE_RE = re.compile(r"^\s*#define\s+(SPECIES_[A-Z0-9_]+)\s+(\d+)\b")
SPECIES_ALIAS_RE = re.compile(r"^\s*#define\s+(SPECIES_[A-Z0-9_]+)\s+(SPECIES_[A-Z0-9_]+)\b")
MACRO_DEFINE_RE = re.compile(r"^\s*#define\s+([A-Z0-9_]+)\s+(.+?)\s*$")
SPECIAL_ALIAS_FALLBACKS = {
    "SPECIES_MOTHIM": ("SPECIES_MOTHIM_PLANT",),
    "SPECIES_ARCEUS": ("SPECIES_ARCEUS_NORMAL",),
}
SPECIAL_VALUE_MAP = {
    "RALTS_FAMILY_EGG_GROUPS": "{EGG_GROUP_AMORPHOUS, EGG_GROUP_HUMAN_LIKE}",
}


@dataclass
class SpeciesBlock:
    species: str
    fields: dict[str, str]
    source_file: str


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
    parser.add_argument("--start-dex", type=int, default=387, help="National dex start (inclusive)")
    parser.add_argument("--end-dex", type=int, default=667, help="National dex end (inclusive)")
    parser.add_argument("--write", action="store_true", help="Apply replacements in species_info.h")
    return parser.parse_args()


def extract_numeric_macros(text: str) -> dict[str, str]:
    macros: dict[str, str] = {}
    for line in text.splitlines():
        match = MACRO_DEFINE_RE.match(line)
        if match is None:
            continue
        name, raw_value = match.group(1), match.group(2).strip()
        if "(" in name:  # Skip function-like macros.
            continue
        if raw_value.endswith("\\"):  # Multi-line macro, handled elsewhere.
            continue
        macros[name] = raw_value
    return macros


def load_known_constants(path: Path, prefix: str) -> set[str]:
    if not path.exists():
        return set()
    text = path.read_text(encoding="utf-8")
    pattern = re.compile(rf"^\s*#define\s+({re.escape(prefix)}[A-Z0-9_]+)\b", re.MULTILINE)
    return set(pattern.findall(text))


def sanitize_constant(token: str, prefix: str, known: set[str], fallback: str) -> str:
    token = token.strip()
    token = SPECIAL_VALUE_MAP.get(token, token)
    if not token.startswith(prefix):
        return token
    if token in known:
        return token
    return fallback


def simplify_conditional(value: str, macro_values: dict[str, str]) -> str:
    value = value.strip()
    value = SPECIAL_VALUE_MAP.get(value, value)
    if value in macro_values:
        return macro_values[value]
    if "?" in value and ":" in value:
        head = value.find("?")
        cond = value[:head].strip()
        rest = value[head + 1 :]
        true_branch, false_branch = rest.rsplit(":", 1)

        # Prefer updated typing branches so TYPE_FAIRY resolves when present.
        if "P_UPDATED_TYPES" in cond and ">=" in cond and "GEN_6" in cond:
            return simplify_conditional(true_branch.strip(), macro_values)

        return simplify_conditional(false_branch.strip(), macro_values)
    return value


def normalize_value(
    field: str,
    value: str,
    macro_values: dict[str, str],
    known_abilities: set[str],
    known_items: set[str],
    known_types: set[str],
    known_egg_groups: set[str],
) -> str:
    value = value.strip()
    value = value.replace("STANDARD_FRIENDSHIP", "70")
    value = value.replace("EGG_GROUP_NO_EGGS_DISCOVERED", "EGG_GROUP_UNDISCOVERED")
    value = simplify_conditional(value, macro_values)

    if field == "types":
        m = re.fullmatch(r"MON_TYPES\(([^)]*)\)", value)
        if not m:
            return value
        args = [part.strip() for part in m.group(1).split(",") if part.strip()]
        args = [sanitize_constant(arg, "TYPE_", known_types, "TYPE_NORMAL") for arg in args]
        if len(args) == 1:
            return f"{{{args[0]}, {args[0]}}}"
        return f"{{{args[0]}, {args[1]}}}"

    if field == "eggGroups":
        m = re.fullmatch(r"MON_EGG_GROUPS\(([^)]*)\)", value)
        if not m:
            return value
        args = [part.strip() for part in m.group(1).split(",") if part.strip()]
        args = [sanitize_constant(arg, "EGG_GROUP_", known_egg_groups, "EGG_GROUP_UNDISCOVERED") for arg in args]
        if len(args) == 1:
            return f"{{{args[0]}, {args[0]}}}"
        return f"{{{args[0]}, {args[1]}}}"

    if field == "abilities":
        m = re.fullmatch(r"\{\s*([^}]*)\s*\}", value)
        if not m:
            return value
        abilities = [part.strip() for part in m.group(1).split(",") if part.strip()]
        if not abilities:
            return "{ABILITY_NONE, ABILITY_NONE}"
        first = sanitize_constant(abilities[0], "ABILITY_", known_abilities, "ABILITY_NONE")
        second = sanitize_constant(abilities[1] if len(abilities) > 1 else "ABILITY_NONE", "ABILITY_", known_abilities, "ABILITY_NONE")
        hidden = sanitize_constant(abilities[2] if len(abilities) > 2 else "ABILITY_NONE", "ABILITY_", known_abilities, "ABILITY_NONE")
        mapped_second = hidden if second == "ABILITY_NONE" and hidden != "ABILITY_NONE" else second
        return f"{{{first}, {mapped_second}}}"

    if field in ("itemCommon", "itemRare"):
        return sanitize_constant(value, "ITEM_", known_items, "ITEM_NONE")

    if field == "expYield" and value.isdigit():
        return str(min(int(value), 255))

    if field == "types":
        if value.startswith("{") and value.endswith("}"):
            inner = [part.strip() for part in value[1:-1].split(",") if part.strip()]
            if len(inner) == 1:
                inner.append(inner[0])
            if len(inner) >= 2:
                t1 = sanitize_constant(inner[0], "TYPE_", known_types, "TYPE_NORMAL")
                t2 = sanitize_constant(inner[1], "TYPE_", known_types, "TYPE_NORMAL")
                return f"{{{t1}, {t2}}}"

    if field == "eggGroups":
        if value.startswith("{") and value.endswith("}"):
            inner = [part.strip() for part in value[1:-1].split(",") if part.strip()]
            if len(inner) == 1:
                inner.append(inner[0])
            if len(inner) >= 2:
                g1 = sanitize_constant(inner[0], "EGG_GROUP_", known_egg_groups, "EGG_GROUP_UNDISCOVERED")
                g2 = sanitize_constant(inner[1], "EGG_GROUP_", known_egg_groups, "EGG_GROUP_UNDISCOVERED")
                return f"{{{g1}, {g2}}}"

    return value


def parse_macro_templates(text: str) -> dict[str, tuple[list[str], dict[str, str]]]:
    templates: dict[str, tuple[list[str], dict[str, str]]] = {}
    lines = text.splitlines()
    i = 0
    while i < len(lines):
        line = lines[i]
        func_match = re.match(r"^\s*#define\s+([A-Z0-9_]+)\(([^)]*)\)\s*\\\s*$", line)
        obj_match = re.match(r"^\s*#define\s+([A-Z0-9_]+)\s*\\\s*$", line)
        if not func_match and not obj_match:
            i += 1
            continue

        if func_match:
            macro_name = func_match.group(1)
            params = [p.strip() for p in func_match.group(2).split(",") if p.strip()]
        else:
            macro_name = obj_match.group(1)  # type: ignore[union-attr]
            params = []

        i += 1
        body_fields: dict[str, str] = {}
        while i < len(lines):
            body_line = lines[i].rstrip()
            if not body_line.endswith("\\"):
                break
            stripped = body_line[:-1].rstrip()
            assign_match = ASSIGNMENT_RE.match(stripped)
            if assign_match:
                body_fields[assign_match.group(1)] = assign_match.group(2).strip()
            i += 1
        templates[macro_name] = (params, body_fields)
    return templates


def apply_macro_template(
    template: tuple[list[str], dict[str, str]],
    args: list[str],
    macro_values: dict[str, str],
    known_abilities: set[str],
    known_items: set[str],
    known_types: set[str],
    known_egg_groups: set[str],
) -> dict[str, str]:
    params, fields = template
    substitutions = {param: args[idx] for idx, param in enumerate(params) if idx < len(args)}
    out: dict[str, str] = {}
    for field, value in fields.items():
        substituted = value
        for key, replacement in substitutions.items():
            substituted = re.sub(rf"\b{re.escape(key)}\b", replacement, substituted)
        out[field] = normalize_value(
            field,
            substituted,
            macro_values,
            known_abilities,
            known_items,
            known_types,
            known_egg_groups,
        )
    return out


def extract_blocks(
    text: str,
    source_file: str,
    macro_values: dict[str, str],
    known_abilities: set[str],
    known_items: set[str],
    known_types: set[str],
    known_egg_groups: set[str],
) -> list[SpeciesBlock]:
    blocks: list[SpeciesBlock] = []
    macro_templates = parse_macro_templates(text)
    i = 0
    while i < len(text):
        m = BLOCK_START_RE.search(text, i)
        if not m:
            break

        species = m.group(1)
        brace_start = text.find("{", m.end() - 1)
        if brace_start == -1:
            i = m.end()
            continue

        depth = 0
        j = brace_start
        while j < len(text):
            char = text[j]
            if char == "{":
                depth += 1
            elif char == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        if j >= len(text):
            break

        body = text[brace_start + 1:j]
        fields: dict[str, str] = {}
        for line in body.splitlines():
            assign_match = ASSIGNMENT_RE.match(line)
            if not assign_match:
                continue
            field = assign_match.group(1)
            if field not in FIELD_ORDER:
                continue
            if field in fields:
                continue
            fields[field] = normalize_value(
                field,
                assign_match.group(2),
                macro_values,
                known_abilities,
                known_items,
                known_types,
                known_egg_groups,
            )

        blocks.append(SpeciesBlock(species=species, fields=fields, source_file=source_file))
        i = j + 1

    for line in text.splitlines():
        m = BLOCK_MACRO_RE.match(line.strip())
        if not m:
            continue
        species, macro_name, macro_args = m.group(1), m.group(2), (m.group(3) or "")
        template = macro_templates.get(macro_name)
        if template is None:
            continue
        args = [arg.strip() for arg in macro_args.split(",")] if macro_args else []
        fields = apply_macro_template(
            template,
            args,
            macro_values,
            known_abilities,
            known_items,
            known_types,
            known_egg_groups,
        )
        blocks.append(SpeciesBlock(species=species, fields=fields, source_file=source_file))
    return blocks


def load_species_numbers_and_aliases(path: Path) -> tuple[dict[str, int], dict[str, str]]:
    text = path.read_text(encoding="utf-8")
    numbers = {name: int(num) for name, num in re.findall(SPECIES_DEFINE_RE.pattern, text, re.MULTILINE)}
    aliases = {alias: target for alias, target in re.findall(SPECIES_ALIAS_RE.pattern, text, re.MULTILINE)}
    return numbers, aliases


def build_entry(species: str, number: int, fields: dict[str, str]) -> str:
    values = dict(DEFAULTS)
    values.update(fields)
    lines = [f"    [{species} /* {number} */] =", "    {"]
    for field in FIELD_ORDER:
        lines.append(f"        .{field} = {values[field]},")
    lines.extend(["    },"])
    return "\n".join(lines)


def main() -> int:
    args = parse_args()

    source_paths = [args.source_root / rel_path for rel_path in args.source_files]
    for source_path in source_paths:
        if not source_path.exists():
            print(f"ERROR: Source file not found: {source_path}", file=sys.stderr)
            return 1
    if not args.species_header.exists():
        print(f"ERROR: Species header not found: {args.species_header}", file=sys.stderr)
        return 1
    if not args.species_info.exists():
        print(f"ERROR: Species info file not found: {args.species_info}", file=sys.stderr)
        return 1

    known_abilities = load_known_constants(ROOT / "include/constants/abilities.h", "ABILITY_")
    known_items = load_known_constants(ROOT / "include/constants/items.h", "ITEM_")
    known_types = load_known_constants(ROOT / "include/constants/pokemon.h", "TYPE_")
    known_egg_groups = load_known_constants(ROOT / "include/constants/pokemon.h", "EGG_GROUP_")

    species_numbers, species_aliases = load_species_numbers_and_aliases(args.species_header)

    parsed_by_species: dict[str, SpeciesBlock] = {}
    for source_path in source_paths:
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

    in_scope_species = [
        species
        for species, number in species_numbers.items()
        if args.start_dex <= number <= args.end_dex
    ]
    in_scope_species.sort(key=lambda s: species_numbers[s])

    alias_by_target: dict[str, list[str]] = {}
    for alias, target in species_aliases.items():
        alias_by_target.setdefault(target, []).append(alias)

    missing_required: list[str] = []
    replacements: dict[str, str] = {}
    for species in in_scope_species:
        block = parsed_by_species.get(species)
        if block is None:
            for alias in alias_by_target.get(species, []):
                alias_block = parsed_by_species.get(alias)
                if alias_block is not None:
                    block = alias_block
                    break
        if block is None:
            for fallback_alias in SPECIAL_ALIAS_FALLBACKS.get(species, ()):
                alias_block = parsed_by_species.get(fallback_alias)
                if alias_block is not None:
                    block = alias_block
                    break
        if block is None:
            continue
        if any(req not in block.fields and req not in DEFAULTS for req in REQUIRED_FIELDS):
            missing_required.append(species)
            continue
        replacements[species] = build_entry(species, species_numbers[species], block.fields)

    species_info_text = args.species_info.read_text(encoding="utf-8")
    stub_line_re = re.compile(r"^(\s*)\[(SPECIES_[A-Z0-9_]+)\s*/\*\s*(\d+)\s*\*/\]\s*=\s*\{0\},\s*$", re.MULTILINE)

    replaced: list[str] = []

    def replace_stub(match: re.Match[str]) -> str:
        species = match.group(2)
        replacement = replacements.get(species)
        if replacement is None:
            return match.group(0)
        replaced.append(species)
        return replacement

    new_text = stub_line_re.sub(replace_stub, species_info_text)

    print(f"Source files parsed: {len(source_paths)}")
    print(f"Species in requested dex range: {len(in_scope_species)}")
    print(f"Species parsed with usable data: {len(replacements)}")
    print(f"Stubs replaced in destination: {len(replaced)}")
    if missing_required:
        print(f"Species skipped due to missing required fields: {len(missing_required)}")
        for species in missing_required[:20]:
            print(f"  - {species}")
        if len(missing_required) > 20:
            print(f"  ... and {len(missing_required) - 20} more")

    unresolved = [s for s in in_scope_species if s not in replacements]
    if unresolved:
        print(f"Species in scope without parsed entries: {len(unresolved)}")
        for species in unresolved[:20]:
            print(f"  - {species}")
        if len(unresolved) > 20:
            print(f"  ... and {len(unresolved) - 20} more")

    if not args.write:
        print("Dry-run only. Re-run with --write to apply changes.")
        return 0

    if new_text == species_info_text:
        print("No changes to write.")
        return 0

    args.species_info.write_text(new_text, encoding="utf-8")
    print(f"Wrote updated species data to: {args.species_info}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
