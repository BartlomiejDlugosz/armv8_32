from functions import *
import numpy as np

names = ["Genetic Algorithm Max", "Genetic Algorithm Avg", "Basic", "Basic Plus"]

for i in range(len(names)):
    name = names[i]
    strategy = name + "-road"

    yss = [genxy(file) for file in [ strategy + '0.txt', strategy + '1.txt', strategy + '2.txt', strategy + '3.txt' ]]
    xs = np.linspace(0, len(yss[0]), len(yss[0]), endpoint=False)

    ys_isec = yss[0]
    for i in range(1, len(yss)):
        ys_isec = np.add(ys_isec, yss[i])

    for i in range(len(yss)):
        ys = yss[i]

        initalisePlot(name + ": Cars on road " + str(i) + " over time ",
                      xs, ys, '$t$ / s', "num cars")
        savePlot("result" + str(i) + strategy)


    initalisePlot(name + ": Total cars in intersection over time ",
                  xs, ys_isec, '$t$ / s', "num cars")
    savePlot("total_result" + strategy)

