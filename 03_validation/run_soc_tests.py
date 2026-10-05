from common import assemble, run_sim, run_rtl, test_status, compare_cpu
from common import ROOT, ASSEMBLER, SIM, RTL, SOC
from common import SOC_PROGS, HEX_FILES, RESULTS 
from common import RED, GREEN, PINK, CYAN, RESET
from pathlib import Path
import subprocess
import json

def validate_soc():
  tests_total  = 0
  tests_passed = 0

  print("\n" + "-" * 36)
  print("SOC".center(36))
  print("-" * 36)

  # running all tests
  for test in SOC_PROGS.glob("*.s"):
    base = test.stem

    # asm
    assemble(base, SOC_PROGS)

    # rtl - soc
    run_rtl(base, SOC)
    soc_pass = compare_cpu(
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
  validate_soc()

if __name__ == "__main__":
  main()
