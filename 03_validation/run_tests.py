from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent.parent
ASSEMBLER = ROOT / "01_software" / "asm" / "assembler.py"
SIM       = ROOT / "01_software" / "sim" / "run_cpu.c"
RTL       = ROOT / "02_hardware" / "rtl" / "verilator_cpu"
TESTS     = ROOT / "03_validation" / "programs"
RESULTS   = ROOT / "03_validation" / "results"

def assemble(base):
  subprocess.run([
    "python3",
    ASSEMBLER,
    TESTS / f"{base}.s",
    TESTS / f"{base}.hex"
  ])

def run_sim(base):
  subprocess.run([
    SIM,
    TESTS / f"{base}.hex",
    TESTS / f"{base}.sim.json"
  ]) 

def run_rtl(base):
  pass

def compare():
  pass
  #expected = json.load(open(TESTS/"add.expected.json"))

def main():
  for test in TESTS.glob("*.s"):
    base = test.stem

    assemble(base)
    run_sim(base)
    #run_rtl(base)
    #compare(base)

if __name__ == "__main__":
  main()
