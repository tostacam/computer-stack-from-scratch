import subprocess
import sys

def run_gnu_assembler(filename, gnu_prefix):
  subprocess.run([
    "riscv64-unknown-elf-as",
    filename,
    "-o",
    f"{gnu_prefix}.o"
  ], check = True)

  result = subprocess.run([
    "riscv64-unknown-elf-objdump",
    "-d",
    f"{gnu_prefix}.o"
  ], capture_output = True, text = True, check = True)

  with open(f"{gnu_prefix}.hex", "w") as file:
    for line in result.stdout.splitlines():
      parts = line.split()
      # 0: 003100b3 add x1,x2,x3
      if len(parts) >= 2 and parts[0].endswith(":") and parts[0][:-1].isalnum():
        file.write(parts[1] + "\n")

def run_own_assembler(filename, own_prefix):
  subprocess.run([
    "python3",
    "assembler.py",
    filename,
    f"{own_prefix}.hex"
  ], check = True)

def compare_files(file1, file2):
  with open(file1) as f:
    expected = f.readlines()

  with open(f"{file2}.hex") as f:
    actual = f.readlines()

  if expected == actual:
    print("PASS")
    return
  else:
    print("FAIL")

  for i, (e, a) in enumerate(zip(expected, actual), start = 1):
    if e != a:
      print(f"line {i}")
      print(f" GNU : {e.strip()}")
      print(f" GNU : {e.strip()}")
      return

  if len(expected) != len(actual):
    print("FAIL: files have different lengths")

def main():
  if len(sys.argv) != 2:
    print(f"Need input/output files: python3 {sys.argv[0]} <input.s>")
    sys.exit(1)

  filename = sys.argv[1]

  gnu_prefix = "tests/assembler_file_gnu"
  own_prefix = "tests/assembler_file_own"

  run_gnu_assembler(filename, gnu_prefix)
  run_own_assembler(filename, own_prefix)

  compare_files(f"{gnu_prefix}.hex", own_prefix)

if __name__ == "__main__":
  main()
