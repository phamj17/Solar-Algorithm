import serial, os
import json
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import datetime
import csv

control = []
tilter = []
c_power = []
c_time = []
t_power = []
t_time = []
#style.use('fivethirtyeight')

#t_fig = plt.figure()
#c_fig = plt.figure()
#t_graph = t_fig.add_subplot(1,1,1)
#c_graph = c_fig.add_subplot(1,1,1)
#plt.axis([0, 24, 0, 18])
#plt.show()

def writeToFile(dictList, filename):
    file = open(filename, 'a')
    for d in dictList:
        #splitDict = dictList.split()
        file.write(json.dumps(d))
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
    
# def animate():
    # global t_time, t_power, c_time, c_power
    # t_graph.clear()
    # c_graph.clear()
    # t_graph.plot(t_time, t_power)
    # c_graph.plot(c_time, c_power)
    # t_graph.draw()
    # c_graph.draw()

def main():
    global control
    global tilter
    global c_power
    global c_time
    global t_power
    global t_time
    ser = serial.Serial(
    baudrate = 9600,
    port = 'COM3'
    )

    timer = 0
    flag = 0
    c_filename = "ControlData_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
    t_filename = "TilterData_" + datetime.datetime.now().strftime("%y-%m-%d") + ".txt"
 
    while True:
        output = ser.readline().strip()
            
        outputList = output.split(',')

        panelDict = {"hour": -1, "minute": -1, "second": -1, "voltage": -1, "valid": -1}
        #powerDict = {"time": -1, "power": -1}
        power = 0.0
        time = 0.0
        
        #print(outputList)
        for element in outputList:
            itemList = element.split(':')
            if (itemList[0] == 'CONTROL' or itemList[0] == "TILTER"):
                flag = 1
            elif (flag == 1):
                panelDict[itemList[0]] = itemList[1]
        flag = 0
        
        if (panelDict["valid"] == 0):
            panelDict["hour"] = datetime.datetime.now().strftime("%H")
            panelDict["minute"] = datetime.datetime.now().strftime("%M")
            panelDict["second"] = datetime.datetime.now().strftime("%S")
            
        panelDict["hour"] =- 6.0
        if (panelDict["hour"] < 0):
                panelDict["hour"] += 24
        if (panelDict['voltage'] >= 0):
            volt = float(panelDict['voltage'])
            curr = float(panelDict['voltage']) / 6
            #powerDict['power'] = volt * curr
            #powerDict['time'] = float(panelDict['hour']) + float(panelDict['minute']) / 60 + float(panelDict['second']) / 3600
            time = float(panelDict['hour']) + float(panelDict['minute']) / 60 + float(panelDict['second']) / 3600
            power = volt * curr
            #plt.scatter
        
        if (outputList[0] == "CONTROL"):
            control.append(panelDict)
            c_power.append(power)
            c_time.append(time)
        elif (outputList[0] == "TILTER"):
            tilter.append(panelDict)
            t_power.append(power)
            t_time.append(time)
        timer = timer + 1
        if timer >= 2:
            writeToFile(control, c_filename)
            writeToFile(tilter, t_filename)
            writeToFile(c_time, "cTime.txt")
            writeToFile(c_power, "cPower.txt")
            writeToFile(t_time, "tTime.txt")
            writeToFile(t_power, "tPower.txt")
            writePowerAndTime(c_power, c_time, "c_file.csv")
            writePowerAndTime(t_power, t_time, "t_file.csv")
            control = []
            tilter = []
            timer = 0
            #animate()
            #test(c_power)
        #t_ani = animation.FuncAnimation(t_fig, animate, interval=1000)
        #c_ani = animation.FuncAnimation(c_fig, animate, interval=1000)
        
        
if __name__ == "__main__":
    main()


# Format
# CONTROL,hour:02,minute:05,second:34,Azimuth:010101,Voltage:5,Current:5

# C:\Python27\python C:\Users\Nicholas\Desktop\solarAlgorithm\Solar-Algorithm\track\parse.py
