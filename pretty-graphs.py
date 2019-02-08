
import matplotlib.pyplot as plt
from random import uniform

from ctypes import cdll, c_int32, c_int16
import os

filter_obj = cdll.LoadLibrary(os.path.abspath("test_filter.so"))

window = 50
buf = (c_int32 * window)()

filter_obj.start(buf, c_int16(window)); 

y = [x**(.5) for x in range(100)]
dydt = [y[i] - y[i-1] for i in range(1, len(y))]

ny = [.1 * y[i] * uniform(0, 1) + y[i] for i in range(0, len(y))]
dnydt = [ny[i] - ny[i-1] for i in range(len(ny))]

fy = []
for v in ny:
    filter_obj.push(c_int32(int(100*v)))
    fy.append(filter_obj.read()/100)

fy = fy[2:]
dnydt = dnydt[1:]

plt.figure(1)

plt.plot(y)
plt.plot(dydt)
plt.title("x^(1/2) and its derivative")

plt.figure(2)

plt.plot(ny)
plt.plot(dnydt)
plt.title("x^(1/2) + 10% uniform noise, and its derivative");

plt.figure(3)

plt.plot(ny)
plt.plot(fy)
plt.title("x^(1/2) + 10% uniform noise, and its filtered derivative")

plt.show()

