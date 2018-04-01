import serial
import matplotlib.pyplot as plt

control = []
tilter = []

c_power = []
t_power = []
    
ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM3'

while True:
    output = ser.readline().strip()
    print(output)
    
    if (output == "FIN"):
        break
        
    outArray = output.split(',')
    panelDict {};
    for (int i = 1; i < len(outArray); i++)
    {
        dictArray = outArray[i].split(':');
        panelDict[dictArray[0]] = dictArray[1]
    }
    panelDict['power'] = float(panelDict['voltage']) * float(panelDict['current'])
    
    if (outArray[0] == "CONTROL")
        control.append(panelDict)
    else if (outArray[0] == "TILTER")
        tilter.append(panelDict)
        
        
# for (int i = 0; i < len(control); i++)
# {
    
# }
# Format
# CONTROL,hour:02,minute:05,second:34,Azimuth:010101,Voltage:5,Current:5