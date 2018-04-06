import serial, os
import json
import datetime
from dateutil import tz
import csv

def writeToFile(list, filename):
    file = open(filename, 'a')
	for l in list:
		file.write(l)
		file.write("\n");

def writePowerAndTime(powerList, timeList, filename):
    with open(filename, 'a') as csvfile:
        file = csv.writer(csvfile)
        for power, time in zip(powerList, timeList):
            temp = [power, '', time]
            file.writerow(temp)

def test(list):
    for meh in list:
        print(meh)
    print("------------------------")
	
def main():
	controlList = {}
	tilterList = {}
    # control = []
    # tilter = []
    # c_power = []
    # c_time = []
    # t_power = []
    # t_time = []
    ser = serial.Serial(baudrate = 9600, port = 'COM3')

    timer = 0
    flag = 0
	
	prevHour = 0
	currMin = datetime.datetime.now().strftime("%M")
	nextMin = datetime.datetime.now().strftime("%M") + 1
	currHour = datetime.datetime.now().strftime("%H")

	cAccumulator = 0
	tAccumulator = 0
	
	cSamples = 0
	tSamples = 0

    while True:
		c_filename = "ControlTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
		t_filename = "TilterTest_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
		# cPower_filename ="ControlPower_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
		# tPower_filename ="TilterPower_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
		# cTime_filename ="ControlTime_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
		# tTime_filename ="TilterTime_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
        output = ser.readline().strip()

        outputList = output.split(' ')

        #panelDict = {"hour": -1, "minute": -1, "voltage": -1}

        # for element in outputList:
            # itemList = element.split(' ')
            # if len(itemList) == 2:
                # if (itemList[0] == 0 or itemList[0] == 1)):
                    # flag = 1
                # elif (flag == 1):
                    # panelDict[itemList[0]] = itemList[1]
        # flag = 0
		currHour = datetime.datetime.now().strftime("%H")
		currMin = datetime.datetime.now().strftime("%M")
		
		if (currMin < nextMin and len(outputList == 2):
			if (outputList[0] == 0):
				cAccumulator = cAccumulator + outputList[1]
				cSamples = cSamples + 1
			elif (outputList[1] == 1):
				tAccumulator = tAccumulator + outputList[1]
				tSamples = tSamples + 1
		elif (currMin == nextMin and len(outputList == 2):
			if (outputList[0] == 0):
				controlList.append(prevHour)
				controlList.append(currMin - 1)
				controlList.append(cAccumulator / cSamples)
			elif (outputList[1] == 1):
				tilterList.append(prevHour)
				tilterList.append(currMin - 1)
				tilterList.append(tAccumulator / tSamples)

		nextMin = currMin + 1
		prevHour = currHour

        #if (panelDict["valid"] == 0):
        #panelDict["hour"] = datetime.datetime.now().strftime("%H")
        #panelDict["minute"] = datetime.datetime.now().strftime("%M")
        #panelDict["second"] = datetime.datetime.now().strftime("%S")

        # panelDict["hour"] = float(panelDict["hour"]) - 8
        # if (panelDict["hour"] < 0):
                # panelDict["hour"] = float(panelDict["hour"]) + 24
        #if (panelDict['voltage'] >= 0):
        #    volt = float(panelDict['voltage'])
        #    curr = float(panelDict['voltage']) / 6
            #powerDict['power'] = volt * curr
            #powerDict['time'] = float(panelDict['hour']) + float(panelDict['minute']) / 60 + float(panelDict['second']) / 3600
            #time = float(panelDict['hour']) + float(panelDict['minute']) / 60 + float(panelDict['second']) / 3600
            #power = volt * curr
            #plt.scatter

        #print(panelDict)

        # if (outputList[0] == "panel:CONTROL"):
            # control.append(panelDict)
            # c_power.append(power)
            # c_time.append(time)
        # elif (outputList[0] == "panel:TILTER"):
            # tilter.append(panelDict)
            # t_power.append(power)
            # t_time.append(time)
        #print(c_power)
            # writeToFile(control, c_filename)
            # writeToFile(tilter, t_filename)
            # writeToFile(c_time, cTime_filename)
            # writeToFile(c_power, cPower_filename)
            # writeToFile(t_time, tTime_filename)
            # writeToFile(t_power, tPower_filename)
            # writePowerAndTime(c_power, c_time, "c_file.csv")
            # writePowerAndTime(t_power, t_time, "t_file.csv")
            # control = []
            # tilter = []
		writeToFile(controlList, c_filename)
		writeToFile(tilterList, t_filename)
		
		controlList = {}
		tilterList = {}
		
		tAccumulator = 0
		tSamples = 0
		
		cAccumulator = 0
		cSamples = 0

if __name__ == "__main__":
    main()

# C:\Python27\python C:\Users\Nicholas\Desktop\solarAlgorithm\Solar-Algorithm\track\parse.py
