#!/usr/bin/python3

import subprocess
import os
from os import listdir

PROJECT_NAME = 'Multimeter'
LAYER_LIST = ('B_Cu', 'B_Mask', 'B_Silkscreen', 'Edge_Cuts', 'F_Cu', 'F_Mask', 'F_Silkscreen', 'drl') 

PCB_PATH = f'../ecad/{PROJECT_NAME}.kicad_pcb'
SCHEMATIC_PATH = f'../ecad/{PROJECT_NAME}.kicad_sch'
GERBERS_PATH = './gerbers'

DRILL_FILE = f'./{PROJECT_NAME}.drl'
BOM_FIELDS = 'Reference,Value,${QUANTITY},Digikey,Part #,Price'

def error_check_pass():
    PCB_CONDITIONS = ('Found 0 DRC violations', 'Found 0 unconnected pads', 'Found 0 Footprint errors') 
    schematic_condition = 'ERC messages: 0  Errors 0  Warnings 0'

    subprocess.run([f'kicad-cli sch erc {SCHEMATIC_PATH}'], shell=True)
    report_file = open(f'{PROJECT_NAME}.rpt', 'r').read()

    if(not schematic_condition in report_file):
        print('SCHEMATIC ERC Error')
        return False
    print('ERC PASS')
    
    subprocess.run([f'kicad-cli pcb drc {PCB_PATH}'], shell=True)
    report_file = open('Multimeter.rpt', 'r').read()

    for condition in PCB_CONDITIONS:
        if(not condition in report_file):
            print('PCB DRC Error') 
            return False
    print('DRC PASS')
    return True


def filter_gerbers():
    for file in listdir(GERBERS_PATH):
        layer_found = False
        for layer in LAYER_LIST:
            if(layer in file):
                layer_found = True
        if(not layer_found):
            subprocess.run([f'rm {GERBERS_PATH}/{file}'], shell=True)


def export():
    # Export the Schematic
    subprocess.run([f'kicad-cli sch export pdf -n -t my_theme2 {SCHEMATIC_PATH}'], shell=True)      
    # Create a gerbers directory if it does not exist
    if(not os.path.isdir(GERBERS_PATH)):
        subprocess.run([f'mkdir {GERBERS_PATH}'], shell=True)            
        print('CREATED GERBERS DIRECTORY')
    # Purdge the gerbers directory
    subprocess.run([f'rm {GERBERS_PATH}/*'], shell=True)
    # Export the gerber drill file
    subprocess.run([f'kicad-cli pcb export drill {PCB_PATH}'], shell=True)
    # There seems to be a bug where -o does not work for drill
    subprocess.run([f'mv {DRILL_FILE} {GERBERS_PATH}/{PROJECT_NAME}.drl'], shell=True)
    # Export the rest of the gerbers
    subprocess.run([f'kicad-cli pcb export gerbers --ev -o {GERBERS_PATH} {PCB_PATH}'], shell=True)
    filter_gerbers()
     

if __name__ == '__main__':
    if(error_check_pass()):
        export()
