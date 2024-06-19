#from importlib.metadata import metadata
import json
import math
import numpy as np
import scipy.optimize
import matplotlib.pyplot as plt
from numpy import polynomial

# set the font for graphs
#plt.rcParams.update({
#    'text.usetex': True,
#    'font.family': 'Computer Modern Serif',
#    'font.sans-serif': ['Computer Modern Serif']})


def genxy(file, delim=','):
    # takes a .txt file as an argument (of type str).
    # x-values should be on first line, y-values on
    # the second line, separated by delim.
    arr = np.genfromtxt(file, delimiter=delim)
    if np.ndim(arr) == 1:
        return arr
    else:
        return arr[0], arr[1]

def initalisePlot(title, xs, ys, xlabel, ylabel):
    'call before attempting addPlot'
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.xlim(min(xs), max(xs))
    plt.ylim(min(ys), max(ys))
    plt.plot(xs, ys, '-', label="fitted")
    plt.grid()
    plt.title(title)
    # 72 dpi is default for screens, doesn't matter much for a pdf
    # however, higher values will increase file size without much benefit

def savePlot(title, resolution=72):
    plt.savefig(f'{title}.pdf', dpi=resolution)

# TODO: add plot from two points function?
