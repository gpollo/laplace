#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
import math
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

import scipy.interpolate
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

def gradient_line_real_one(data,sx,sy,d):
    y,x = np.gradient(data)

    nx = sx
    ny = sy

    lx = []
    ly = []

    while True:
        lx.append(nx)
        ly.append(ny)

        try:
            gx = x[ny][nx]
            gy = y[ny][nx]
        except:
            break

        if math.isnan(gx) or math.isnan(gy):
            break

        dx = gx/(np.sqrt(gx*gx+gy*gy))
        dy = gy/(np.sqrt(gx*gx+gy*gy))

        nx = nx+d*int(np.round(8*dx))
        ny = ny+d*int(np.round(8*dy))

        if nx < 1:
            nx = 1
        if ny < 1:
            ny = 1
        if nx > len(data[0]):
            nx = len(data[0])-1
        if ny > len(data):
            ny = len(data)-1

    return lx,ly

def gradient_line(data,sx,sy):
    x1,y1 = gradient_line_real_one(data,sx,sy,-1)
    x2,y2 = gradient_line_real_one(data,sx,sy, 1)

    x1.reverse()
    y1.reverse()

    x = np.array(x1+x2)
    y = np.array(y1+y2)

    plt.plot(x, y, color='black')

if __name__ == "__main__":
    if len(sys.argv) < 2:
        exit()

    data = np.genfromtxt(sys.argv[1], delimiter=',')
    levels = np.linspace(-150,150,35)

    plt.imshow(data, cmap=mpl.cm.gist_rainbow, interpolation='gaussian')
    plt.contour(data,levels)

    i = 1
    while True:
        if math.isnan(data[1][i]):
            break
        i += 1

#    gradient_line(data,int((1*i)/5),2)
#    gradient_line(data,int((2*i)/5),2)
#    gradient_line(data,int((3*i)/5),2)
#    gradient_line(data,int((4*i)/5),2)

    w = len(data[0])
    h = len(data)

    gradient_line(data, 10,2)
    gradient_line(data,310,2)
    gradient_line(data,int(w/2),int(h/2))
    for offset in [25,50,115]:
        gradient_line(data,int(w/2),int(h/2)-offset)
        gradient_line(data,int(w/2),int(h/2)+offset)


    if any('--show' in arg for arg in sys.argv):
        plt.show()
    else:
        plt.savefig('figure.png', dpi=1000, transparent=True)


