import matplotlib.pyplot as plt
import numpy as np
from math import fabs

percent_error = lambda ref, measured: fabs(((ref - measured)/measured)*100)

data = open('voltages.txt', 'r').read()
meter = []
reference = []
difference = []
percent_errors = []
zero_cleared_values = []
for i, line in enumerate(data.split('\n')):
    if(i != 0 and line != ''):
        seperated_value = line.split(',')
        meter_value = float(seperated_value[0])
        reference_value = float(seperated_value[1])
        meter.append(meter_value) 
        reference.append(reference_value)
        difference.append(reference_value - meter_value)
        if(reference_value != 0):
            print(reference_value, percent_error(reference_value, meter_value))
            zero_cleared_values.append(meter_value)
            percent_errors.append(percent_error(reference_value, meter_value))

DEGREE = 12

meter = np.array(meter)
difference = np.array(difference)
error_poly = np.polyfit(meter, difference, DEGREE)
error_fn = np.poly1d(error_poly) 

vv = np.linspace(-60, 60, num=1000)
dd = error_fn(vv)

plt.plot(zero_cleared_values, percent_errors)
plt.show()
