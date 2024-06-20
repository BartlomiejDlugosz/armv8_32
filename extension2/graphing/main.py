from functions import *

yss = [genxy(file) for file in [ 'road0.txt', 'road1.txt', 'road2.txt', 'road3.txt' ]]
xs = np.linspace(0, len(yss[0]), len(yss[0]), endpoint=False)

ys_isec = yss[0]
for i in range(1, len(yss)):
    for j in range(0, len(yss[i])):
        ys_isec[j] += yss[i][j]

for i in range(len(yss)):
    ys = yss[i]

    initalisePlot("Cars on road" + str(i) + "over time ",
                  xs, ys, '$t$ / s', "num cars")
    savePlot("result" + str(i))


initalisePlot("Total cars in intersection over time ",
              xs, ys_isec, '$t$ / s', "num cars")
savePlot("total_result")

