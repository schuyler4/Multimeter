percent_error = lambda ref, measured: (abs(ref - measured)/measured)*100

for index, line in enumerate(open('voltages.txt').read().split('\n')):
    if(index != 0 and line != ''):
        voltage_setting = float(line.split(',')[1])
        error = round(percent_error(voltage_setting, float(line.split(',')[0])),2) 
        print(voltage_setting, error) 
