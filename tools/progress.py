import argparse
import json
import git
from pathlib import Path
import math

parser = argparse.ArgumentParser(description="Computes current progress throughout the whole project.")
parser.add_argument("format", nargs="?", default="text", choices=["text", "csv", "shield-json"])
args = parser.parse_args()


def ReadAllLines(fileName: Path) -> list[str]:
    with open(fileName) as f:
        return f.readlines()


def GetFiles(path: Path, ext) -> list[Path]:
    files: list[Path] = []
    for file in path.rglob(f'*{ext}'):
        if file.is_file() and file.suffix == ext:
            files.append(file)
    return files


def GetSizeOfASMSourceFile(asmSrc: Path) -> int:
    size: int = 0
    asmLines: list[str] = ReadAllLines(asmSrc)
    
    for asmLine in asmLines:
        if asmLine.startswith("\t"):
            asmLineStripped = asmLine.strip()
            if not (asmLineStripped.startswith("AREA") or 
                    asmLineStripped.startswith("EXPORT") or 
                    asmLineStripped.startswith("END") or 
                    asmLineStripped.startswith("DCD") or 
                    asmLineStripped.startswith("DCB")):
                size += 4
            elif asmLineStripped.startswith("DCD "):
                size += 4
            elif asmLineStripped.startswith("DCB "):
                size += len(asmLineStripped.split(','))

    size = 4 * math.ceil(size / 4.0)
    return size


def GetAllNonMatchingASMFunctionSourceFiles(nonmatchFiles: list[Path]) -> list[Path]:
    nonmatchASMFunctionSourceFiles: list[Path] = list()
    
    for nonmatchFile in nonmatchFiles:
        with open(nonmatchFile, 'r') as nonmatch:
            nonmatchLines: list[str] = nonmatch.readlines()
            for line in nonmatchLines:
                if line.startswith("\tGET") and line.count("disassembly\\asm") > 0:
                    nonmatchASMFunctionSourceFiles.append(Path(line.split(' ')[-1].strip()))
    
    return nonmatchASMFunctionSourceFiles


def GetTotalNonMatchSize() -> int:
    nonmatchFiles: list[Path] = GetFiles(Path("build/nonmatch"), ".s")
    nonMatchingASMFunctionSourceFiles: list[Path] = GetAllNonMatchingASMFunctionSourceFiles(nonmatchFiles)

    size: int = 0
    for nonMatchFunction in nonMatchingASMFunctionSourceFiles:
        size += GetSizeOfASMSourceFile(nonMatchFunction)
    return size

def progressBar(percentage: float, width: int = 30) -> str:
    """Returns a more accurate progress bar string based on percentage using fractional blocks."""
    # Calculate the total block units (each block has 8 sub-units)
    total_units = percentage * width * 8 / 100
    full_units = int(total_units)  # total filled sub-units
    full_blocks = full_units // 8  # complete blocks
    partial_unit = full_units % 8   # remaining sub-unit
    
    # Unicode characters for fractional blocks; index 0 is empty.
    blocks = ["", "▏", "▎", "▍", "▌", "▋", "▊", "▉"]
    
    bar = "█" * full_blocks  # full blocks
    if partial_unit > 0:
        bar += blocks[partial_unit]
    
    # Pad the bar to the desired width
    bar = bar.ljust(width, " ")
    return f"[{bar}] {percentage:.4f}%"


nonMatchingASMSize: int = GetTotalNonMatchSize()

totalSize: int = 0x3C97B8
srcSize: int = totalSize - nonMatchingASMSize

srcPct: float = 100.0 * srcSize / totalSize

bytesPerHeartPiece: float = totalSize / 80.0

if args.format == 'csv':
    version = 1
    git_object = git.Repo().head.object
    timestamp = str(git_object.committed_date)
    git_hash = git_object.hexsha
    csv_list = [str(version), timestamp, git_hash, str(srcSize), str(nonMatchingASMSize)]
    print(",".join(csv_list))
elif args.format == 'shield-json':
    # https://shields.io/endpoint
    print(json.dumps({
        "schemaVersion": 1,
        "label": "Progress",
        "message": f"{srcPct:.6g}% ({srcSize}/{totalSize} bytes)",
        "color": 'blue',
    }))
elif args.format == 'text':
    print(f"{totalSize} total bytes of decompilable code\n")
    print(f"{srcSize} bytes decompiled in src {srcPct}%\n")
    print(progressBar(srcPct))
    print("------------------------------------\n")

    heartPieces = int(srcSize / bytesPerHeartPiece)
    rupees = int(((srcSize % bytesPerHeartPiece) * 100) / bytesPerHeartPiece)

    if (rupees > 0):
        pass
        #print(f"You have {heartPieces}/80 heart pieces and {rupees} rupee{"s" if rupees != 1 else ""}.\n")
    else:
        print(f"You have {heartPieces}/80 heart pieces.\n")
else:
    print(f"Unknown format argument: {args.format}")
