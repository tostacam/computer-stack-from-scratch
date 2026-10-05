from pathlib import Path
import subprocess
import json

# executables
ROOT = Path(__file__).resolve().parent.parent
ASSEMBLER = ROOT / "01_software" / "asm" / "assembler.py"
CPU_SIM   = ROOT / "01_software" / "sim" / "tools" / "run_sim_cpu"
CPU_RTL   = ROOT / "02_hardware" / "rtl" / "obj_dir" / "Vcpu_system"
SOC_RTL   = ROOT / "02_hardware" / "rtl" / "obj_dir" / "Vsoc"

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

def run_cpu_sim(base):
  subprocess.run([
    CPU_SIM,
    HEX_FILES / f"{base}.hex",
    RESULTS / f"{base}.sim.json"
  ]) 

def run_cpu_rtl(base):
  num_instr = sum(1 for line in open(f"{HEX_FILES}/{base}.hex") if line.strip())

  subprocess.run([
    CPU_RTL,
    f"+ROM={HEX_FILES}/{base}.hex",
    RESULTS / f"{base}.rtl.json"
  ])

def run_soc_rtl(base):
  num_instr = sum(1 for line in open(f"{HEX_FILES}/{base}.hex") if line.strip())

  subprocess.run([
    SOC_RTL,
    f"+ROM={HEX_FILES}/{base}.hex",
    RESULTS / f"{base}.rtl.json"
  ])

def compare_cpu(expected_file, result_file):
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
