#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
import math
import numpy as np

if __name__ == "__main__":
    # we need at least an argument to a *.csv file containing the data to plot
    if len(sys.argv) < 2:
        exit()

    data = np.genfromtxt(sys.argv[1], delimiter=',')

    # remove unused lines
    data = np.delete(data, 0, axis=1)
    data = np.delete(data,-1, axis=1)
    data = np.delete(data,-1, axis=1)
    data = np.delete(data, 0, axis=0)
    data = np.delete(data,-1, axis=0)

    sys.stdout.write('\\documentclass[11pt]{article}\n')
    sys.stdout.write('\\usepackage{siunitx}\n')
    sys.stdout.write('\\usepackage{standalone}\n')

    # table header
    sys.stdout.write('\\begin{document}\n')
    sys.stdout.write('\\newcolumntype{Y}{S[table-format=3.2]}\n')
    sys.stdout.write('\\begin{tabular}{')
    for i in data[0]:
        sys.stdout.write('Y')
    sys.stdout.write('}\n')

    # table data
    for j in data:
        for i in j:
            if math.isnan(i):
                sys.stdout.write(' {--}  ')
            else:
                sys.stdout.write('{: 7.2f}'.format(i))
            sys.stdout.write('&')
        sys.stdout.write('\b\\\\\n')

    # table footer
    sys.stdout.write('\\end{tabular}\n')
    sys.stdout.write('\\end{document}\n')
