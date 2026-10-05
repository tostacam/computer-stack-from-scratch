from common import assemble, run_cpu_sim, run_cpu_rtl, test_status, compare_cpu
from common import CPU_PROGS, HEX_FILES, RESULTS 
from common import RED, GREEN, PINK, CYAN, RESET
from pathlib import Path
import subprocess
import json

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

    # cpu sim
    run_cpu_sim(base)
    sim_pass = compare_cpu(
      CPU_PROGS / f"{base}.expected.json", 
      RESULTS / f"{base}.sim.json")
    tests_total  += 1
    if sim_pass:
      tests_passed += 1

    # cpu rtl
    run_cpu_rtl(base)
    rtl_pass = compare_cpu(
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

def main():
  validate_cpu()

if __name__ == "__main__":
  main()
