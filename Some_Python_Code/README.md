# Presentation to PDF Converter (`ppt_to_pdf.py`)

A flexible, high-fidelity Python utility to convert PowerPoint presentations (`.pptx`, `.ppt`, `.ppsx`, `.odp`) to PDF on Linux/Ubuntu, with options for single file, multiple files, or entire directory trees, plus optional automatic cleanup of source files.

---

## Features
- **Flexible Inputs**:
  - Convert a single presentation file.
  - Convert multiple presentation files in one command.
  - Convert an entire directory (recursively searches all subdirectories).
- **Supported File Formats**:
  - `.pptx` (PowerPoint 2007+)
  - `.ppt` (PowerPoint 97-2003)
  - `.ppsx` (PowerPoint Slide Show)
  - `.odp` (OpenDocument Presentation)
- **Automatic Original Removal**:
  - Pass `--remove-original` (or `--remove-pptx` / `-r`) to automatically delete the original presentation files **after** confirming the PDF was successfully created and verified.
- **Interactive Mode**:
  - If executed without arguments (`python3 ppt_to_pdf.py`), it prompts you step-by-step for the file/folder path and cleanup preference.
- **Python Module Import**:
  - Importable into any Python program or workflow.

---

## How to Run

### 1. Interactive Mode
Simply run the script with no arguments:
```bash
python3 converters/ppt_to_pdf.py
```
You will be prompted:
1. Enter file path or folder path: `/path/to/folder`
2. Delete original presentation files after successful conversion? (y/N): `y`

---

### 2. Single File
Convert one file and keep the original:
```bash
python3 converters/ppt_to_pdf.py /path/to/lecture.pptx
```

Convert one file and remove the original `.pptx`:
```bash
python3 converters/ppt_to_pdf.py /path/to/lecture.pptx --remove-original
```

---

### 3. Multiple Files
Pass as many files as you like:
```bash
python3 converters/ppt_to_pdf.py slide1.pptx slide2.ppt slide3.ppsx --remove-original
```

---

### 4. Entire Folder (Recursive)
Convert all presentation files located anywhere inside a folder and its subfolders:
```bash
python3 converters/ppt_to_pdf.py /home/swyanswartz/Music/STUDY/RM --remove-original
```

Or specify with `-i` / `--input`:
```bash
python3 converters/ppt_to_pdf.py -i /home/swyanswartz/Music/STUDY/RM -r
```

---

### 5. Custom Output Directory
By default, the PDF is placed in the same folder as each source presentation. If you want all generated PDFs placed in a specific folder:
```bash
python3 converters/ppt_to_pdf.py /home/swyanswartz/Music/STUDY/RM -o /home/swyanswartz/Music/STUDY/PDFs
```

---

### 6. Use as a Python Library
You can import the converter into your own scripts:

```python
from converters.ppt_to_pdf import convert_single_file, convert_all

# Convert a single file:
convert_single_file("slides.pptx", remove_original=True)

# Convert an entire directory or list of paths:
convert_all(
    paths=["/home/swyanswartz/Music/STUDY/RM"],
    remove_original=True,
    recursive=True
)
```
