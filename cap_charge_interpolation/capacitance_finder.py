import numpy as np

class Capacitance_Finder:

    def __init__(self, voltage_points, series_resistor, voltage_source, time_delta):
        self.voltage_points = voltage_points
        self.series_resistor = series_resistor
        self.voltage_source = voltage_source
        self.time_delta = time_delta

    def _order_voltage_points(self):
        sorted_voltage_points = sorted(self.voltage_points)
        return (sorted_voltage_points[1], sorted_voltage_points[0]) 
        
    '''
    v(t) = Vs*(1-e^(-t/T))
    v(t)/Vs = 1-e^(-t/T) 
    -v(t)/Vs + 1 = e^(-t/T) 
    ln(-v(t)/Vs + 1) = -t/T
    -T*ln(((-v(t)/Vs) + 1) = t
    -T*ln((-v2/Vs) + 1) + T*ln((-v1/Vs) + 1) = t2 - t1
    '''
    def __call__(self):
        v2, v1 = self._order_voltage_points()    
        Vs = self.voltage_source
        dt = self.time_delta
        Rs = self.series_resistor
        return dt/((np.log((-v2/Vs)+1) - np.log((-v1/Vs)+1))*-1*Rs)
 


