import random

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from capacitance_finder import Capacitance_Finder

EXPECTED_CAPACITANCE = 0.1e-6
SERIES_RESISTOR = 1.43e6
VOLTAGE_SOURCE = 1
RUN_COUNT = 100

data = pd.read_csv('sim_curve.txt', sep='\t').to_numpy()
time = data[:,0]
voltage = data[:,1]
count = len(time)
capacitances = []

for _ in range(0, RUN_COUNT):
    random_point1 = random.randint(0, count-1)
    random_point2 = random.randint(0, count-1)

    while(random_point1 == random_point2):
        random_point1 = random.randint(0, count-1)
        random_point2 = random.randint(0, count-1)

    time_delta = np.abs(time[random_point1] - time[random_point2])
    voltage_points = [voltage[random_point1], voltage[random_point2]]

    cap_finder = Capacitance_Finder(voltage_points, SERIES_RESISTOR, VOLTAGE_SOURCE, time_delta)
    capacitances.append(cap_finder())


plt.scatter(time[random_point1], voltage_points[0], color='red')
plt.scatter(time[random_point2], voltage_points[1], color='red')
plt.plot(time, voltage)

capacitances = np.array(capacitances)
print(np.var(capacitances)/EXPECTED_CAPACITANCE)
plt.show()
