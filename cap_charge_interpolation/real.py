import csv

import matplotlib.pyplot as plt 

import numpy as np

from capacitance_finder import Capacitance_Finder

SET_RESISTANCE = 1e6
SOURCE_VOLTAGE = 1
THRESHOLD_COEFFICIANT = 0.1

def find_asymtotic_voltage(tt, vv, show_plot=False):
    FILTER_LENGTH = 100    
    filtered_voltage = np.convolve(np.array(vv), np.ones(FILTER_LENGTH))/FILTER_LENGTH
    source_voltage = np.max(filtered_voltage)
    if(show_plot):
        plt.plot(tt, filtered_voltage[0:len(vv)])
        plt.plot(tt, np.ones(len(tt))*source_voltage)
        plt.show()
        plt.clf()
    return source_voltage


def find_thevenin_resistance(set_resistance, source_voltage, asymtotic_voltage):
    I = (source_voltage - asymtotic_voltage)/set_resistance    
    cap_parallel_resistance = asymtotic_voltage/I
    return 1/((1/cap_parallel_resistance) + (1/SET_RESISTANCE))


class Resampler:
    
    def __init__(self, tt, xx, period, threshold):
        self._tt = tt
        self._xx = xx
        self._period = period
        self._threshold = threshold
        self._new_tt = []
        self._new_xx = []


    def __call__(self):
        elapsed_time = 0
        for i, t in enumerate(self._tt):
            if(i == 0):
                dt = self._tt[i]    
            else:
                dt = self._tt[i] - self._tt[i -1]
            elapsed_time += dt      
            if(elapsed_time >= self._period):
                if(self._xx[i] > self._threshold):
                    self._new_tt.append(t)
                    self._new_xx.append(self._xx[i])
                elapsed_time = 0
        

    @property
    def tt(self):
        return self._new_tt


    @property
    def xx(self):
        return self._new_xx

            
def find_capacitance_from_circuit(voltage, time, sample_period):
    asymtotic_voltage = find_asymtotic_voltage(time, voltage)
    response_resample = Resampler(time, voltage, sample_period, THRESHOLD_COEFFICIANT*asymtotic_voltage)
    response_resample()
    delta_time = response_resample.tt[1] - response_resample.tt[0]
    resistance = find_thevenin_resistance(SET_RESISTANCE, SOURCE_VOLTAGE, asymtotic_voltage)
    cap_finder = Capacitance_Finder(response_resample.xx, resistance, asymtotic_voltage, delta_time) 
    capacitance = cap_finder()
    return (capacitance, response_resample.tt, response_resample.xx)


def read_csv(filename):
    voltages = []
    times = []
    with open(filename, newline='') as csvfile:
        data_reader = csv.reader(csvfile, delimiter=',') 
        for row in data_reader:
            times.append(float(row[0]))    
            voltages.append(float(row[1]))
    return (times, voltages)


time1, voltage1 = read_csv('cap1.csv')
time2, voltage2 = read_csv('cap2.csv')

capacitance1, resampled_time1, resampled_voltage1 = find_capacitance_from_circuit(voltage1, time1, 0.015)
capacitance2, resampled_time2, resampled_voltage2 = find_capacitance_from_circuit(voltage2, time2, 0.1)

print('capacitor 1:', capacitance1)
print('capacitor 2:', capacitance2)

plt.scatter(resampled_time1, resampled_voltage1, color='red', zorder=1)
plt.plot(time1, voltage1, color='blue', zorder=0) 
plt.title('Capacitor1')
plt.xlabel('Time (s)')
plt.ylabel('Voltage (V)')
plt.show()
plt.clf()

plt.scatter(resampled_time2, resampled_voltage2, color='red', zorder=1)
plt.plot(time2, voltage2, color='blue', zorder=0)
plt.title('Capacitor 2')
plt.xlabel('Time (s)')
plt.ylabel('Voltage (V)')
plt.show()
plt.clf()
