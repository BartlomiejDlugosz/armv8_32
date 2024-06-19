from functions import *
y = genxy('num_cars.txt')
print(y)
x = np.linspace(0, len(y), len(y), endpoint=True)
print(x)

initalisePlot("Cars on road over time",
              x, y, '$t$ / s', "num cars")

savePlot("result.txt")
