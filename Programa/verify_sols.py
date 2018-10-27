#!/usr/bin/env python


import numpy as np
import pandas as pd
import os, sys

if __name__ == '__main__':
    sol_dir = os.path.join('.', 'tests', 'solutions')
    out_dir = os.path.join('.', 'output')

    for f in out_dir:
        if '.txt' not in f:
            continue

        out_f = os.path.join(out_dir, f)
        sol_f = os.path.join(sol_dir, f)

        with open(out_f, 'r') as f:
            n_steps_out = int(f.read())

        with open(sol_f, 'r') as f:
            n_steps_sol = int(f.read())

        if n_steps_out != n_steps_sol:
            print("Output step count does not match solution: {} {}".format(n_steps_out, n_steps_sol))

    print("All good!")
