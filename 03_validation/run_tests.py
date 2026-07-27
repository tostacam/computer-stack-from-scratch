from pathlib import Path
import subprocess
import json

ROOT = Path(__file__).resolve().parent.parent
ASSEMBLER = ROOT / "01_software" / "asm" / "assembler.py"
SIM       = ROOT / "01_software" / "sim" / "tools" / "run_cpu"
RTL       = ROOT / "02_hardware" / "rtl" / "verilator_cpu"

PROGRAMS  = ROOT / "03_validation" / "programs"
HEX_FILES = ROOT / "03_validation" / "hex_files"
RESULTS   = ROOT / "03_validation" / "results"

def assemble(base):
  subprocess.run([
    "python3",
    ASSEMBLER,
    PROGRAMS / f"{base}.s",
    HEX_FILES / f"{base}.hex"
  ])

def run_sim(base):
  subprocess.run([
    SIM,
    HEX_FILES / f"{base}.hex",
    RESULTS / f"{base}.sim.json"
  ]) 

def run_rtl(base):
  pass

def compare(base):
  expected = json.load(open(PROGRAMS/f"{base}.expected.json"))
  sim      = json.load(open(RESULTS/f"{base}.sim.json"))

  test_passed = True

  # PC
  if "pc" in expected:
    if sim["pc"] != expected["pc"]: 
      test_passed = False

  # Registers
  for reg, val in expected.get("registers", {}).items():
    if sim["registers"][reg] != val:
      test_passed = False

  # Memory
  for addr, value, in expected.get("memory", {}).items():
    if sim["memory"][addr] != val:
      test_passed = False

  return test_passed

def main():
  # creating folders
  HEX_FILES.mkdir(parents=True, exist_ok=True)
  RESULTS.mkdir(parents=True, exist_ok=True)

  tests_total  = 0
  tests_passed = 0

  # running all tests
  for test in PROGRAMS.glob("*.s"):
    base = test.stem

    assemble(base)
    run_sim(base)
    #run_rtl(base)
    
    passed = compare(base)
    tests_total += 1

    if passed:
      tests_passed += 1
      print(f"PASS {base}")
    else:
      print(f"FAIL {base}")

  percentage = 100 * tests_passed / tests_total
  print(f"Summary: {tests_passed}/{tests_total} tests passed ({percentage:.1f}%)")

if __name__ == "__main__":
  main()
