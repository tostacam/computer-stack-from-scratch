from pathlib import Path
import subprocess
import json

# executables
ROOT = Path(__file__).resolve().parent.parent
ASSEMBLER = ROOT / "01_software" / "asm" / "assembler.py"
SIM       = ROOT / "01_software" / "sim" / "tools" / "run_sim_cpu"
RTL       = ROOT / "02_hardware" / "rtl" / "obj_dir" / "Vcpu_system"
SOC       = ROOT / "02_hardware" / "rtl" / "obj_dir" / "Vsoc"

# folders
CPU_PROGS = ROOT / "03_validation" / "cpu_programs"
HEX_FILES = ROOT / "03_validation" / "hex_files"
RESULTS   = ROOT / "03_validation" / "results"
SOC_PROGS = ROOT / "03_validation" / "soc_programs"

MASK_64BIT = 0xFFFFFFFFFFFFFFFF

RED   = "\033[31m"
GREEN = "\033[32m"
PINK  = "\033[35m"
CYAN  = "\033[36m"
RESET = "\033[0m"

def assemble(base, folder):
  subprocess.run([
    "python3",
    ASSEMBLER,
    folder / f"{base}.s",
    HEX_FILES / f"{base}.hex"
  ])

def run_sim(base):
  subprocess.run([
    SIM,
    HEX_FILES / f"{base}.hex",
    RESULTS / f"{base}.sim.json"
  ]) 

def run_rtl(base, component):
  num_instr = sum(1 for line in open(f"{HEX_FILES}/{base}.hex") if line.strip())

  subprocess.run([
    component,
    f"+ROM={HEX_FILES}/{base}.hex",
    f"+CYCLES={num_instr}",
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
    if (result["registers"][reg] & MASK_64BIT) != (val & MASK_64BIT):
      return False

  # Memory
  for addr, val, in expected.get("memory", {}).items():
    if result["memory"][addr] != val:
      return False

  return True

def test_status(passed):
  if passed:
    return f"{GREEN}✓ PASS{RESET}"
  return f"{RED}✗ FAIL{RESET}"


def validate_cpu():
  # creating folders
  HEX_FILES.mkdir(parents=True, exist_ok=True)
  RESULTS.mkdir(parents=True, exist_ok=True)

  tests_total  = 0
  tests_passed = 0

  print("\n" + "-" * 36)
  print("CPU".center(36))
  print("-" * 36)
  print(f"{'Program':<12} {CYAN}{'SIM':<10} {PINK}{'RTL':<10}{RESET}")
  print("-" * 36)

  # running all tests
  for test in CPU_PROGS.glob("*.s"):
    base = test.stem

    # asm
    assemble(base, CPU_PROGS)

    # sim
    run_sim(base)
    sim_pass = compare(
      CPU_PROGS / f"{base}.expected.json", 
      RESULTS / f"{base}.sim.json")
    tests_total  += 1
    if sim_pass:
      tests_passed += 1

    # rtl - cpu
    run_rtl(base, RTL)
    rtl_pass = compare(
      CPU_PROGS / f"{base}.expected.json", 
      RESULTS / f"{base}.rtl.json")
    tests_total  += 1
    if rtl_pass:
      tests_passed += 1

    print(f"{base:<10} {test_status(sim_pass)}     {test_status(rtl_pass)}")
  
  percentage = 100 * tests_passed / tests_total
  color = GREEN if tests_passed == tests_total else RED
  print("-" * 36)
  print(f"{color}Summary: {tests_passed}/{tests_total} tests passed ({percentage:.1f}%){RESET}")
  print("-" * 36 + "\n")

def validate_soc():
  tests_total  = 0
  tests_passed = 0

  print("-" * 36)
  print("SOC".center(36))
  print("-" * 36)

  # running all tests
  for test in SOC_PROGS.glob("*.s"):
    base = test.stem

    # asm
    assemble(base, SOC_PROGS)

    # rtl - soc
    run_rtl(base, SOC)
    soc_pass = compare(
      SOC_PROGS / f"{base}.expected.json",
      RESULTS / f"{base}.rtl.json")
    tests_total += 1
    if soc_pass:
      tests_passed += 1

    print(f"{base:<12}      {test_status(soc_pass)}")

  percentage = 100 * tests_passed / tests_total
  color = GREEN if tests_passed == tests_total else RED
  print("-" * 36)
  print(f"{color}Summary: {tests_passed}/{tests_total} tests passed ({percentage:.1f}%){RESET}")
  print("-" * 36 + "\n")

def main():
  validate_cpu()
  validate_soc()

if __name__ == "__main__":
  main()
