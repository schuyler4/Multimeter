import csv

digikey_part_numbers = []
quantities = []

with open('Multimeter_BOM.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    header = next(reader)
    part_number_index = None
    quantity_index = None
    for i, label in enumerate(header):
        if('Digikey' in label):
            part_number_index = i
        elif('Qty' in label):
            quantity_index = i
    for row in reader:
       for i, value in enumerate(row):
            if(i == part_number_index):
                digikey_part_numbers.append(value)
            elif(i == quantity_index):
                quantities.append(value)

with open('Multimeter_BOM_digikey.csv', 'w') as csvfile:
    writer = csv.writer(csvfile, delimiter=',')
    for i, part_number in enumerate(digikey_part_numbers):
        writer.writerow([quantities[i], digikey_part_numbers[i]])
    
