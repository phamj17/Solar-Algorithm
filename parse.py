# C:\Python27\python C:\Users\Nicholas\Desktop\solarAlgorithm\Solar-Algorithm\parse.py

import serial, os
import datetime
import csv

def write_txt(list, filename):
    file = open(filename, 'a')
    for l in list:
        file.write(str(l))
        file.write(" ")
    file.write("\n");

def write_csv(list, filename):
    with open(filename, 'a') as csvfile:
        file = csv.writer(csvfile)
        file.writerow(list)
    
def main():
    controlList = []
    tilterList = []
    
    ser = serial.Serial(baudrate = 9600, port = 'COM3')

    timer = 0
    flag = 0
    
    currHour = int(datetime.datetime.now().strftime("%H"))
    prevHour = currHour
    currMin = int(datetime.datetime.now().strftime("%M"))
    nextMin = int(datetime.datetime.now().strftime("%M")) + 1

    cAccumulator = 0.0
    tAccumulator = 0.0
    
    cSamples = 0
    tSamples = 0

    while True:
        cTxtFile = "ControlTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
        tTxtFile = "TilterTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
        cCsvFile = "ControlTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".csv"
        tCsvFile = "TilterTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".csv"
        
        output = ser.readline().strip()
        outputList = output.split(' ')

        currHour = int(datetime.datetime.now().strftime("%H"))
        currMin = int(datetime.datetime.now().strftime("%M"))
        
        if (len(outputList) == 2):
            try:
                panel = int(outputList[0])
                voltage = float(outputList[1])
            except ValueError:
                break
                
            if (currMin < nextMin):
                if (panel == 0):
                    cAccumulator = cAccumulator + voltage
                    cSamples = cSamples + 1
                elif (panel == 1):
                    tAccumulator = tAccumulator + voltage
                    tSamples = tSamples + 1
            elif (currMin == nextMin):
                controlList.append(prevHour)
                controlList.append(currMin - 1)
                controlList.append(cAccumulator / cSamples)
                
                tilterList.append(prevHour)
                tilterList.append(currMin - 1)
                tilterList.append(tAccumulator / tSamples)
                
                write_txt(controlList, cTxtFile)
                write_txt(tilterList, tTxtFile)
                
                write_csv(controlList, cCsvFile)
                write_csv(tilterList, tCsvFile)
                
                controlList = []
                tilterList = []
                
                tAccumulator = 0
                tSamples = 0
                
                cAccumulator = 0
                cSamples = 0
                flag = 0
        
        nextMin = currMin + 1
        prevHour = currHour

if __name__ == "__main__":
    main()
