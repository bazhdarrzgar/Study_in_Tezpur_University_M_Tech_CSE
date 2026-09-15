#!/usr/bin/env python3
"""
Presentation to PDF Converter (PPTX / PPT / PPSX to PDF)
--------------------------------------------------------
Features:
  - Supports single file, multiple files, single folder, or multiple folders.
  - Recursively finds and converts presentation files (.pptx, .ppt, .ppsx, .odp).
  - Can be run from the command line with arguments or interactively.
  - Can be imported as a Python library in other scripts.
  - Option to remove original presentation files after successful conversion.
  - Verifies generated PDF exists and has non-zero size before deleting original.
"""

import sys
import os
import shutil
import subprocess
import argparse
from pathlib import Path
from typing import List, Union, Optional

SUPPORTED_EXTENSIONS = {".pptx", ".ppt", ".ppsx", ".odp"}


def find_libreoffice_binary() -> Optional[str]:
    """Locate the LibreOffice / soffice executable on the system."""
    candidates = [
        shutil.which("libreoffice"),
        shutil.which("soffice"),
        "/snap/bin/libreoffice",
        "/usr/bin/libreoffice",
        "/usr/bin/soffice",
        "/usr/local/bin/libreoffice",
        "/usr/local/bin/soffice",
    ]
    for candidate in candidates:
        if candidate and os.path.isfile(candidate) and os.access(candidate, os.X_OK):
            return candidate
    return None


def convert_single_file(
    file_path: Union[str, Path],
    output_dir: Optional[Union[str, Path]] = None,
    remove_original: bool = False,
    libreoffice_bin: Optional[str] = None,
) -> bool:
    """
    Convert a single presentation file (.pptx, .ppt, .ppsx, .odp) to PDF.
    
    :param file_path: Path to the presentation file.
    :param output_dir: Directory where the PDF will be saved (defaults to same folder as input file).
    :param remove_original: If True, delete original presentation file after successful PDF creation.
    :param libreoffice_bin: Custom path to libreoffice executable.
    :return: True if conversion succeeded, False otherwise.
    """
    src = Path(file_path).resolve()
    if not src.is_file():
        print(f"[-] Error: File does not exist: {src}")
        return False

    if src.suffix.lower() not in SUPPORTED_EXTENSIONS:
        print(f"[-] Skipped: Unsupported extension '{src.suffix}' for file: {src.name}")
        return False

    lo_bin = libreoffice_bin or find_libreoffice_binary()
    if not lo_bin:
        print("[-] Error: LibreOffice executable not found! Please ensure LibreOffice is installed.")
        return False

    target_dir = Path(output_dir).resolve() if output_dir else src.parent
    target_dir.mkdir(parents=True, exist_ok=True)
    target_pdf = target_dir / f"{src.stem}.pdf"

    print(f"[*] Converting: {src.name} -> {target_pdf.name}...")

    cmd = [
        lo_bin,
        "--headless",
        "--convert-to",
        "pdf",
        "--outdir",
        str(target_dir),
        str(src),
    ]

    try:
        result = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=180,
        )
        if result.returncode != 0:
            print(f"[-] LibreOffice conversion failed for {src.name}:")
            print(result.stderr.strip() or result.stdout.strip())
            return False

        # Verify output PDF
        if target_pdf.exists() and target_pdf.stat().st_size > 0:
            print(f"[+] Successfully converted: {target_pdf.name} ({target_pdf.stat().st_size:,} bytes)")
            if remove_original:
                try:
                    src.unlink()
                    print(f"[x] Removed original file: {src.name}")
                except Exception as e:
                    print(f"[!] Warning: Could not remove original file {src.name}: {e}")
            return True
        else:
            print(f"[-] Error: Output PDF was not created or is empty: {target_pdf}")
            return False

    except subprocess.TimeoutExpired:
        print(f"[-] Error: Conversion timed out for {src.name}")
        return False
    except Exception as e:
        print(f"[-] Error during conversion of {src.name}: {e}")
        return False


def collect_presentation_files(paths: List[Union[str, Path]], recursive: bool = True) -> List[Path]:
    """Collect all supported presentation files from given list of files and/or directories."""
    files_to_convert: List[Path] = []
    seen = set()

    for p_str in paths:
        p = Path(p_str).resolve()
        if not p.exists():
            print(f"[!] Warning: Path does not exist: {p}")
            continue

        if p.is_file():
            if p.suffix.lower() in SUPPORTED_EXTENSIONS and p not in seen:
                files_to_convert.append(p)
                seen.add(p)
        elif p.is_dir():
            pattern = "**/*" if recursive else "*"
            for item in p.glob(pattern):
                if item.is_file() and item.suffix.lower() in SUPPORTED_EXTENSIONS and item not in seen:
                    files_to_convert.append(item)
                    seen.add(item)

    return sorted(files_to_convert)


def convert_all(
    paths: List[Union[str, Path]],
    output_dir: Optional[Union[str, Path]] = None,
    remove_original: bool = False,
    recursive: bool = True,
) -> dict:
    """
    Convert presentations across multiple files and/or folders.
    
    :param paths: List of file paths or folder paths.
    :param output_dir: Optional target folder for converted PDFs (if None, each PDF stays with source file).
    :param remove_original: Delete original presentation files after successful conversion.
    :param recursive: If True, recursively search directories.
    :return: Summary dictionary containing total, succeeded, and failed counts.
    """
    lo_bin = find_libreoffice_binary()
    if not lo_bin:
        raise RuntimeError("LibreOffice / soffice executable could not be found on your system.")

    files = collect_presentation_files(paths, recursive=recursive)
    if not files:
        print("[*] No presentation files found matching supported formats (.pptx, .ppt, .ppsx, .odp).")
        return {"total": 0, "succeeded": 0, "failed": 0}

    print(f"\n{'='*60}")
    print(f"Found {len(files)} presentation file(s) to convert.")
    print(f"Remove originals: {'YES' if remove_original else 'NO'}")
    print(f"{'='*60}\n")

    succeeded = 0
    failed = 0

    for idx, f in enumerate(files, 1):
        print(f"[{idx}/{len(files)}]", end=" ")
        ok = convert_single_file(
            file_path=f,
            output_dir=output_dir,
            remove_original=remove_original,
            libreoffice_bin=lo_bin,
        )
        if ok:
            succeeded += 1
        else:
            failed += 1

    print(f"\n{'='*60}")
    print(f"Conversion Summary: Total: {len(files)} | Succeeded: {succeeded} | Failed: {failed}")
    print(f"{'='*60}\n")

    return {"total": len(files), "succeeded": succeeded, "failed": failed}


def interactive_prompt():
    """Prompt the user interactively if no command line arguments were provided."""
    print("=" * 60)
    print("       Presentation to PDF Converter (Interactive Mode)")
    print("=" * 60)
    raw_path = input("Enter file path or folder path (or multiple separated by commas): ").strip()
    if not raw_path:
        print("No input provided. Exiting.")
        sys.exit(0)

    paths = [p.strip().strip("'\"") for p in raw_path.split(",") if p.strip()]

    remove_ans = input("Delete original presentation files after successful conversion? (y/N): ").strip().lower()
    remove_original = remove_ans in ["y", "yes"]

    out_dir_ans = input("Output directory (leave empty to keep alongside originals): ").strip().strip("'\"")
    output_dir = out_dir_ans if out_dir_ans else None

    convert_all(paths=paths, output_dir=output_dir, remove_original=remove_original, recursive=True)


def parse_args():
    parser = argparse.ArgumentParser(
        description="Convert presentation files (.pptx, .ppt, .ppsx, .odp) to PDF.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Convert a single file:
  python ppt_to_pdf.py presentation.pptx

  # Convert a single file and remove the original pptx:
  python ppt_to_pdf.py presentation.pptx --remove-original

  # Convert multiple files:
  python ppt_to_pdf.py slide1.pptx slide2.ppt slide3.ppsx

  # Convert an entire folder (recursively) and save PDFs alongside originals:
  python ppt_to_pdf.py /path/to/folder

  # Convert a folder and delete original presentation files:
  python ppt_to_pdf.py /path/to/folder --remove-original

  # Convert to a specific output folder:
  python ppt_to_pdf.py /path/to/folder -o /path/to/output_pdfs

  # Interactive mode (prompts you for paths):
  python ppt_to_pdf.py
        """,
    )

    parser.add_argument(
        "paths",
        nargs="*",
        help="One or more presentation files or directory paths to convert.",
    )
    parser.add_argument(
        "-i",
        "--input",
        dest="input_path",
        help="Optional explicit input file or folder path (alternative to positional arguments).",
    )
    parser.add_argument(
        "-o",
        "--output-dir",
        dest="output_dir",
        default=None,
        help="Directory to save converted PDF files (defaults to same folder as source file).",
    )
    parser.add_argument(
        "-r",
        "--remove-original",
        "--remove-pptx",
        action="store_true",
        dest="remove_original",
        help="Remove/delete original presentation files (.pptx, .ppt, etc.) after successful conversion.",
    )
    parser.add_argument(
        "--no-recursive",
        action="store_false",
        dest="recursive",
        help="Do not search directories recursively (default searches recursively).",
    )

    return parser.parse_args()


def main():
    args = parse_args()
    input_paths = list(args.paths)
    if args.input_path:
        input_paths.append(args.input_path)

    # If no paths given via CLI, run interactive mode
    if not input_paths:
        interactive_prompt()
        return

    convert_all(
        paths=input_paths,
        output_dir=args.output_dir,
        remove_original=args.remove_original,
        recursive=args.recursive,
    )


if __name__ == "__main__":
    main()
