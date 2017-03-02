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

# some configurations for exporting the graph to LaTeX/PGF
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

def gradient_line_direction(data,sx,sy,d):
    # compute the gradient of the 2D matrix using numpy
    y,x = np.gradient(data)

    # this is our starting coordinates
    nx = sx
    ny = sy

    # we start our list with nothing
    lx = []
    ly = []

    while True:
        # we stop computing the line if we're outside the matrix
        if nx < 0 or ny < 0 or nx > len(data[0]) or ny > len(data):
            break

        # we append the current point to the list
        lx.append(nx)
        ly.append(ny)

        # we obtain the 4 nearest points
        try:
            x1 = int(np.floor(nx))
            y1 = int(np.floor(ny))

            x2 = int(np.floor(nx))
            y2 = int(np.ceil(ny))

            x3 = int(np.ceil(nx))
            y3 = int(np.ceil(ny))

            x4 = int(np.ceil(nx))
            y4 = int(np.floor(ny))
        except ValueError:
            break

        # we get the gradient at point down-left
        try:
            g1 = [x[y1][x1], y[y1][x1]]
        except:
            g1 = [0,0]

        # we get the gradient at point up-left
        try:
            g2 = [x[y2][x2], y[y2][x2]]
        except:
            g2 = [0,0]

        # we get the gradient at point up-right
        try:
            g3 = [x[y3][x3], y[y3][x3]]
        except:
            g3 = [0,0]

        # we get the gradient at point down-right
        try:
            g4 = [x[y4][x4], y[y4][x4]]
        except:
            g4 = [0,0]

        # compute the distance from the current point to the 4 nearest points
        d1 = np.sqrt((nx-x1)**2+(ny-y1)**2)
        d2 = np.sqrt((nx-x2)**2+(ny-y2)**2)
        d3 = np.sqrt((nx-x3)**2+(ny-y3)**2)
        d4 = np.sqrt((nx-x4)**2+(ny-y4)**2)

        # we sum the x coordinate of the 4 nearest points
        sumx = 0
        sumx += 0 if math.isnan(g1[0]) else d1*g1[0]
        sumx += 0 if math.isnan(g2[0]) else d2*g2[0]
        sumx += 0 if math.isnan(g3[0]) else d3*g3[0]
        sumx += 0 if math.isnan(g4[0]) else d4*g4[0]

        # we sum the y coordinate of the 4 nearetst points
        sumy = 0
        sumy += 0 if math.isnan(g1[1]) else d1*g1[1]
        sumy += 0 if math.isnan(g2[1]) else d2*g2[1]
        sumy += 0 if math.isnan(g3[1]) else d3*g3[1]
        sumy += 0 if math.isnan(g4[1]) else d4*g4[1]

        # compute the weighted arithmetic mean of the 4 nearest gradients
        if sumx == 0 or sumy == 0:
            # if the distance to the 4 nearest points were zeros,
            # we simply use the gradient at the current point
            gx = g1[0]
            gy = g1[1]
        else:
            # TODO is the gradient didn't exist at a particular point,
            #      we shouldn't divide by 4 in the weighted mean
            gx = sumx/4
            gy = sumy/4

        # we normalize the gradient vector
        dx = gx/(np.sqrt(gx*gx+gy*gy))
        dy = gy/(np.sqrt(gx*gx+gy*gy))

        # we step to the next gradient using the unit vector
        nx = nx+d*dx
        ny = ny+d*dy

    return lx,ly

def gradient_line(data,sx,sy):
    # we draw the gradient line in both directions
    x1,y1 = gradient_line_direction(data,sx,sy,-1)
    x2,y2 = gradient_line_direction(data,sx,sy, 1)

    # we reverse the first line in order to merge the two
    x1.reverse()
    y1.reverse()

    # we merge the two gradient lines together
    x = np.array(x1+x2)
    y = np.array(y1+y2)

    # we plot a single line
    plt.plot(x, y, color='black')

if __name__ == "__main__":
    # we need at least an argument to a *.csv file containing the data to plot
    if len(sys.argv) < 2:
        exit()

    # we generate levels for the contour plot
    data = np.genfromtxt(sys.argv[1], delimiter=',')
    levels = np.linspace(-150,150,25)

    # we plot the data and the contour plot
    plt.imshow(data, cmap=mpl.cm.gist_rainbow)
    plt.contour(data,levels)

    # the following is specific to data that you want to analyze

    # we find the length of the source in the JFET
    i = 1
    while True:
        if math.isnan(data[1][i]):
            break
        i += 1

    # we plot 5 gradient lines separated equally
    gradient_line(data, int(1*i/6)               , 2)
    gradient_line(data, int(2*i/6)               , 2)
    gradient_line(data, int(3*i/6)               , 2)
    gradient_line(data, int(4*i/6)               , 2)
    gradient_line(data, int(5*i/6)               , 2)

    # these are some lines that might be useful

#    gradient_line(data,int((1*i)/5),2)
#    gradient_line(data,int((2*i)/5),2)
#    gradient_line(data,int((3*i)/5),2)
#    gradient_line(data,int((4*i)/5),2)

#    w = len(data[0])
#    h = len(data)

#    gradient_line(data,  8,2)
#    gradient_line(data,312,2)
#    gradient_line(data,int(w/2),int(h/2))
#    for offset in [58.5]:
#        gradient_line(data,int(w/2),int(h/2)-offset)
#        gradient_line(data,int(w/2),int(h/2)+offset)

    # we remove the border from the plot
    plt.gca().set_xlim(1,len(data[0])-3  )
    plt.gca().set_ylim(  len(data   )-2,1)

    plt.gca().get_yaxis().set_tick_params(direction='out')
    plt.gca().get_xaxis().set_tick_params(direction='out')

    # we show the plot if specified, else we save it to a file
    if any('--show' in arg for arg in sys.argv):
        plt.show()
    elif any('--pgf' in arg for arg in sys.argv):
        plt.savefig('figure.pgf', transparent=True)
    else:
        plt.savefig('figure.png', dpi=1000, transparent=True)


