import matplotlib.pyplot as plt
import numpy as np
from math import fabs

percent_error = lambda ref, measured: fabs(((ref - measured)/measured)*100)

VOLTAGE_PERCENT_ERROR = 0.3
RESISTANCE_PERCENT_ERROR = 10
CAPACITANCE_PERCENT_ERROR = 5

get_digits = lambda s:''.join(list(filter(lambda c: c in '.0123456789', list(s)))) 

def parse_data(filename):
    data = open(filename, 'r').read()
    meter = []
    reference = []
    difference = []
    percent_errors = []
    for i, line in enumerate(data.split('\n')):
        if(i != 0 and line != ''):
            seperated_value = line.split(',')
            meter_value = float(get_digits(seperated_value[0]))
            if(len(seperated_value) == 2):
                reference_value = float(get_digits(seperated_value[1]))
            else:
                reference_value = float(get_digits(seperated_value[2]))
            meter.append(meter_value) 
            reference.append(reference_value)
            if(reference_value != 0):
                percent_errors.append(percent_error(reference_value, meter_value))
    return percent_errors
        
for error in parse_data('voltage.txt'): assert error < VOLTAGE_PERCENT_ERROR
print('voltage test PASS')
