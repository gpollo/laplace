#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

from matplotlib import rc
from matplotlib.font_manager import FontProperties

# configuration nécessaire pour exporter en LaTeX
mpl.rcParams['text.usetex'] = True
mpl.rcParams['font.family'] = 'serif'
mpl.rcParams['font.sans-serif'] = 'cm'
mpl.rcParams['text.latex.unicode'] = True

mpl.rcParams["font.size"] = 11
mpl.rcParams["axes.labelsize"] = 11
mpl.rcParams["legend.fontsize"] = 11

mpl.rcParams["xtick.labelsize"] = 11
mpl.rcParams["ytick.labelsize"] = 11

mpl.rcParams["xtick.direction"] = 'in'
mpl.rcParams["ytick.direction"] = 'in'

mpl.rcParams["axes.linewidth"] = 0.5
mpl.rcParams["lines.linewidth"] = 0.5
mpl.rcParams["lines.markeredgewidth"] = 0
mpl.rcParams["legend.fancybox"] = 'False'
mpl.rcParams["ytick.major.width"] = 0.5
mpl.rcParams["xtick.major.width"] = 0.5

mpl.rcParams["legend.edgecolor"] = 'black'
mpl.rcParams["legend.fancybox"] = 'False'

mpl.rcParams['text.latex.preamble'] = [
    '\\usepackage{siunitx}'
]

if __name__ == "__main__":
    if len(sys.argv) < 2:
        exit()

    data = np.genfromtxt(sys.argv[1], delimiter=',')

    levels = 150*np.power(np.sin(np.linspace(3*np.pi/2,np.pi/2,50)),3)
#    levels = np.array([-150,-50,-25,-5,-1.0/1000000,0,1.0/1000000,5,25,50,150])
#    levels = np.array([0])
#    levels = np.linspace(-150,150,20)

    plt.imshow(data, cmap=mpl.cm.gist_rainbow, interpolation='gaussian')
    plt.contour(data,levels)

    if any('--show' in arg for arg in sys.argv):
        plt.show()
    else:
        plt.savefig('figure.png', dpi=1000)


