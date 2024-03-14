from statistics import variance, mean

DATA_FILE_EXTENSION = '.txt'
SAMPLE_SIZE = 100
DECIMAL_PLACES = 4

def percent_error(ref, measured):
    return (abs(ref - measured)/measured)*100

def extract_float(string):
    return float(''.join(list(filter(lambda char: char in '.1234567890', string))))

def coeff_of_var(samples):
    return (100*variance(samples))/mean(samples)

def accuracy_and_precision(filename):
    f = open(filename, 'r')
    string_measured_values = list(filter(lambda line: line != '', f.read().split('\n')))
    measured_values = [extract_float(string) for string in string_measured_values]
    reference_value = measured_values[0]
    sample_values = measured_values[1:SAMPLE_SIZE+1]
    errors = list(map(lambda value: percent_error(reference_value, value), sample_values))
    return round(mean(errors), DECIMAL_PLACES), round(coeff_of_var(sample_values), DECIMAL_PLACES)

def print_results(dataset_filenames):
    for filename in dataset_filenames:
        print(filename.replace(DATA_FILE_EXTENSION, ''))
        accuracy, precision = accuracy_and_precision(filename)
        print(f'Error: {accuracy}, Coeff of Var: {precision}\n')

VOLTAGE_TEST_FILE_NAMES = ('2.5V.txt', '5V.txt', '7.5V.txt', '10V.txt')
RESISTANCE_TEST_FILE_NAMES = ('10ohm.txt', '100ohm.txt', '1000ohm.txt', '10000ohm.txt', '40000ohm.txt')

print_results(VOLTAGE_TEST_FILE_NAMES)
print_results(RESISTANCE_TEST_FILE_NAMES)
