import subprocess

PCB_PATH = '../ecad/Multimeter.kicad_pcb'
SCHEMATIC_PATH = '../ecad/Multimeter.kicad_sch'

def error_check():
    pcb_conditions = ['Found 0 DRC violations', 'Found 0 unconnected pads', 'Found 0 Footprint errors'] 
    schematic_condition = 'ERC messages: 0  Errors 0  Warnings 0'

    subprocess.run(['kicad-cli sch erc' + ' ' + SCHEMATIC_PATH], shell=True)
    report_file = open('Multimeter.rpt', 'r').read()

    if(not schematic_condition in report_file):
        print('SCHEMATIC ERC Error')
        exit()
    print('ERC PASS')
    
    subprocess.run(['kicad-cli pcb drc' + ' ' + PCB_PATH], shell=True)
    report_file = open('Multimeter.rpt', 'r').read()

    for condition in pcb_conditions:
        if(not condition in report_file):
            print('PCB DRC Error') 
            exit()
    print('DRC PASS')


def export():
      

error_check()

