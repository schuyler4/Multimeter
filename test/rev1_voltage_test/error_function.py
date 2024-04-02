import matplotlib.pyplot as plt
from math import fabs

def percent_error(ref, measure):
    return 100*fabs((measure - ref)/ref)

data = open('voltages.txt', 'r').read()
meter = []
reference = []
difference = []
percent_errors = []
for i, line in enumerate(data.split('\n')):
    if(i != 0 and line != ''):
        seperated_value = line.split(',')
        meter_value = float(seperated_value[0])
        reference_value = float(seperated_value[1])
        meter.append(meter_value) 
        reference.append(reference_value)
        difference.append(meter_value - reference_value)
        percent_errors.append(percent_error(reference_value, meter_value))

#plt.plot(meter, difference)
plt.plot(meter, percent_errors)
plt.show()
