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

    # we find the length of the source in the JFET
    l = 1
    while True:
        if math.isnan(data[1][l]):
            break
        l += 1
    l -= 1

    sum = 0
    for i in range(1,l):
        v11 = data[1][1  ]
        v12 = data[2][1  ]
        v21 = data[1][1+1]
        v22 = data[2][1+1]

        sum += ((v12-v11)+(v22-v21))/2
    print(300e-3/(-(1/0.06e-2)*40e-6*sum*1e-3))


