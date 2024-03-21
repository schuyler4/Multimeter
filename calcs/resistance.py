# This calculation looks at how the ADC code varies with different
# resistance measurements
import matplotlib.pyplot as plt

MAX_RESISTANCE = 40000
BIAS_VOLTAGE = 1.2
REF_VOLTAGE = 2.5
ADC_REF_VOLTAGE = 2.4
ADC_BITS = 23
R_SERIES = 10e3
LSB = ADC_REF_VOLTAGE/2**ADC_BITS  
MAX_CODE = int(ADC_REF_VOLTAGE/LSB)

def get_adc_code(resistance):
    I = (REF_VOLTAGE - BIAS_VOLTAGE)/(R_SERIES + resistance)
    v_measure = I*resistance
    return int(v_measure/LSB)

resistances = []
adc_codes = []

for resistance in range(0, MAX_RESISTANCE+1):
    resistances.append(resistance);
    adc_codes.append(get_adc_code(resistance)/MAX_CODE)

plt.plot(resistances, adc_codes)
plt.xlabel('Resistance (ohms)')
plt.ylabel('ADC Code Ratio')
plt.title('Ratio Method Curve')
plt.show()
