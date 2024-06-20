import json
import math
import numpy as np
import scipy.optimize
import matplotlib.pyplot as plt

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
    plt.plot(xs, ys, '-', label="fitted", linewidth=0.5)
    plt.grid()
    plt.title(title)
    # 72 dpi is default for screens, doesn't matter much for a pdf
    # however, higher values will increase file size without much benefit

def savePlot(title, resolution=300):
    plt.savefig(f'{title}.pdf', dpi=resolution)
    plt.clf()
    plt.close()
