import argparse
import re
from pathlib import Path
from typing import List, Optional, Tuple


ENTRY_RE = re.compile(r'(?m)^(?P<indent>\s*)"(?P<char>[^"\r\n])"\s*,')
HEX_RE = re.compile(r"0[xX][0-9A-Fa-f]{2}")


def read_text(path: Path, encoding: Optional[str]) -> Tuple[str, str]:
    data = path.read_bytes()

    if encoding:
        return data.decode(encoding), encoding

    for enc in ("utf-8-sig", "gb18030", "gbk"):
        try:
            return data.decode(enc), enc
        except UnicodeDecodeError:
            pass

    raise UnicodeDecodeError("unknown", data, 0, 1, "cannot decode input")


def find_bitmap_end(text: str, start: int) -> Optional[int]:
    count = 0

    for match in HEX_RE.finditer(text, start):
        count += 1
        if count == 32:
            pos = match.end()
            while pos < len(text) and text[pos] in " \t":
                pos += 1
            return pos

    return None


def convert_entries(text: str) -> Tuple[str, int]:
    edits: List[Tuple[int, int, str]] = []

    for match in ENTRY_RE.finditer(text):
        char = match.group("char")

        try:
            gbk = char.encode("gbk")
        except UnicodeEncodeError:
            continue

        if len(gbk) != 2:
            continue

        bitmap_end = find_bitmap_end(text, match.end())
        if bitmap_end is None:
            continue

        indent = match.group("indent")
        prefix = f"{indent}{{{{0x{gbk[0]:02X},0x{gbk[1]:02X}}}, {{"
        edits.append((match.start(), match.end(), prefix))

        if bitmap_end < len(text) and text[bitmap_end] == ",":
            edits.append((bitmap_end, bitmap_end, "}}"))
        else:
            edits.append((bitmap_end, bitmap_end, "}}"))

    if not edits:
        return text, 0

    for start, end, replacement in sorted(edits, reverse=True):
        text = text[:start] + replacement + text[end:]

    return text, len(edits) // 2


def default_output_path(input_path: Path) -> Path:
    return input_path.with_name(f"{input_path.stem}_converted{input_path.suffix}")


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            'Convert font entries like "皮",0x.. into '
            "{{0xC6,0xA4}, {0x..}}, so Keil does not depend on Chinese source encoding."
        )
    )
    parser.add_argument("input", type=Path, help="Input txt/h file exported by the font tool.")
    parser.add_argument("-o", "--output", type=Path, help="Output file. Defaults to *_converted.txt/h.")
    parser.add_argument("--in-place", action="store_true", help="Overwrite the input file.")
    parser.add_argument("--input-encoding", help="Input encoding, for example utf-8, gbk, gb18030.")
    parser.add_argument("--output-encoding", default="gbk", help="Output encoding. Defaults to gbk.")
    args = parser.parse_args()

    if args.in_place and args.output:
        parser.error("--in-place and --output cannot be used together")

    input_path = args.input
    output_path = input_path if args.in_place else (args.output or default_output_path(input_path))

    text, used_encoding = read_text(input_path, args.input_encoding)
    converted, count = convert_entries(text)

    with output_path.open("w", encoding=args.output_encoding, newline="") as file:
        file.write(converted)

    print(f"input: {input_path}")
    print(f"detected input encoding: {used_encoding}")
    print(f"output: {output_path}")
    print(f"converted entries: {count}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
