#!/usr/bin/python3
#
# -*- coding: utf-8 -*-
# vim: ts=4 sw=4 tw=100 et ai si
#
# Author: Niklas Neronin <niklas.neronin@gmail.com>

"""Run benchmark."""

import os
import sys
import argparse
import subprocess
from datetime import datetime

VERSION = "1.0.0"
OWN_NAME = "run_benchmark"
LOG_FILE = "./tests/benchmarklog"

def parse_arguments():
    """Parse input arguments."""

    text = sys.modules[__name__].__doc__
    parser = argparse.ArgumentParser(description=text, prog=OWN_NAME)

    text = "Print version and exit."
    parser.add_argument("--version", action="version", help=text, version=VERSION)

    text = "Commit the benchmark measurements."
    parser.add_argument("--commit", action="store_true", help=text)

    return parser.parse_args()

def file_prepend(filename, data):
    """Prepend new benchmark results to 'LOG_FILE'."""

    with open(filename, 'r+', encoding="utf-8") as fobj:
        content = fobj.read()
        fobj.seek(0, 0)
        fobj.write(data.rstrip('\r\n'))
        fobj.write('\n' + content)

def run_benchmark(cmd):
    """Compile and run benchmark."""

    subprocess.run(cmd, shell=True, check=True)
    output = subprocess.run("./a.out", capture_output=True, shell=True, check=True)
    os.remove("./a.out")
    vals = [float(val) for val in output.stdout.decode("utf-8").split("\n")]
    return vals

def main():
    """Script entry point."""

    args = parse_arguments()

    cmd = "gcc ./tests/benchmark.c -L . -Wl,-rpath=$PWD -Wall -ldm_x86_64_Linux -I include -pthread"
    libdm = run_benchmark(cmd)

    cmd = "gcc ./tests/benchmark.c"
    stdlib = run_benchmark(cmd)

    data = datetime.now().strftime("\n------ %Y-%m-%d ------\n")
    data +=  "        : libdm   : stdlib  : diff\n"
    data += f"malloc  : {libdm[0]:0.5f} : {stdlib[0]:0.5f} : {(libdm[0] - stdlib[0]):.5f}\n"
    data += f"free    : {libdm[1]:0.5f} : {stdlib[1]:0.5f} : {(libdm[1] - stdlib[1]):.5f}\n"
    data += f"realloc : {libdm[2]:0.5f} : {stdlib[2]:0.5f} : {(libdm[2] - stdlib[2]):.5f}\n"

    if args.commit:
        file_prepend(LOG_FILE, data)
        subprocess.run(f"git add {LOG_FILE}", shell=True, check=True)
        subprocess.run("git commit -s -m 'benchmark: update 'benchmarklog''", shell=True,
                       check=True)
    else:
        print(data)

    return 0

if __name__ == "__main__":
    sys.exit(main())
