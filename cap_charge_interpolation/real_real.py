'''0.869472, 1.145098,'''
from capacitance_finder import Capacitance_Finder

voltage_points = [0.96, 1.31]
series_resistor = 10000
voltage_source = 1.36
tt = 0.00177

cap_finder = Capacitance_Finder(voltage_points, series_resistor, voltage_source, tt)
print(cap_finder())


