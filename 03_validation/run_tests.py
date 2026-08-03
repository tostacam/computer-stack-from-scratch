from pathlib import Path
import subprocess
import json

ROOT = Path(__file__).resolve().parent.parent
ASSEMBLER = ROOT / "01_software" / "asm" / "assembler.py"
SIM       = ROOT / "01_software" / "sim" / "tools" / "run_cpu"
RTL       = ROOT / "02_hardware" / "rtl" / "obj_dir" / "Vcpu"

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
  subprocess.run([
    RTL,
    f"+ROM={HEX_FILES}/{base}.hex",
    RESULTS / f"{base}.rtl.json"
  ])

def compare(expected_file, result_file):
  expected = json.load(open(expected_file))
  result   = json.load(open(result_file))

  # PC
  if "pc" in expected:
    if result["pc"] != expected["pc"]: 
      return False

  # Registers
  for reg, val in expected.get("registers", {}).items():
    if result["registers"][reg] != val:
      return False

  # Memory
  for addr, value, in expected.get("memory", {}).items():
    if result["memory"][addr] != val:
      return False

  return True

def main():
  # creating folders
  HEX_FILES.mkdir(parents=True, exist_ok=True)
  RESULTS.mkdir(parents=True, exist_ok=True)

  tests_total  = 0
  tests_passed = 0

  # running all tests
  for test in PROGRAMS.glob("*.s"):
    base = test.stem

    # asm
    assemble(base)

    # sim
    run_sim(base)
    sim_pass = compare(
      PROGRAMS / f"{base}.expected.json", 
      RESULTS / f"{base}.sim.json")

    tests_total += 1
    if sim_pass:
      tests_passed += 1
      print(f"PASS [SIM] {base}")
    else:
      print(f"FAIL [SIM] {base}")

    # rtl
    run_rtl(base)
    rtl_pass = compare(
      PROGRAMS / f"{base}.expected.json", 
      RESULTS / f"{base}.rtl.json")
    
    tests_total += 1
    if rtl_pass:
      tests_passed += 1
      print(f"PASS [RTL] {base}")
    else:
      print(f"FAIL [RTL] {base}")
  
  percentage = 100 * tests_passed / tests_total
  print(f"Summary: {tests_passed}/{tests_total} tests passed ({percentage:.1f}%)")

if __name__ == "__main__":
  main()
