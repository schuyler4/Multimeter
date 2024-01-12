import random

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

EXPECTED_CAPACITANCE = 0.1e-6
SERIES_RESISTOR = 1.43e6
VOLTAGE_SOURCE = 1
RUN_COUNT = 100

class Capacitance_Finder:

    def __init__(self, voltage_points, series_resistor, voltage_source, time_delta):
        self.voltage_points = voltage_points
        self.series_resistor = series_resistor
        self.voltage_source = voltage_source
        self.time_delta = time_delta


    def order_voltage_points(self):
        v1, v2 = self.voltage_points
        if(v1 > v2):
            return (v1, v2)
        else: 
            return (v2, v1) 


    '''
    v(t) = Vs*(1-e^(-t/T))
    v(t)/Vs = 1-e^(-t/T) 
    -v(t)/Vs + 1 = e^(-t/T) 
    ln(-v(t)/Vs + 1) = -t/T
    -T*ln(((-v(t)/Vs) + 1) = t
    -T*ln((-v2/Vs) + 1) + T*ln((-v1/Vs) + 1) = t2 - t1
    '''
    def __call__(self):
        v2, v1 = self.order_voltage_points()    
        Vs = self.voltage_source
        dt = self.time_delta
        Rs = self.series_resistor
        return dt/((np.log((-v2/Vs)+1) - np.log((-v1/Vs)+1))*-1*Rs)
 

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
    voltage_points = (voltage[random_point1], voltage[random_point2])

    cap_finder = Capacitance_Finder(voltage_points, SERIES_RESISTOR, VOLTAGE_SOURCE, time_delta)
    capacitances.append(cap_finder())


plt.scatter(time[random_point1], voltage_points[0], color='red')
plt.scatter(time[random_point2], voltage_points[1], color='red')
plt.plot(time, voltage)

capacitances = np.array(capacitances)
print(np.var(capacitances)/EXPECTED_CAPACITANCE)
plt.show()
