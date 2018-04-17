/*
 *  File: track.ino
 *
 *  Description: Obtain data from Arduino peripheries, track 
 *              sun with the SunPos algorithm, and rotate 
 *              the stepper motor to tilt the solar panel.
 *
 *  Authors: Justin Pham and Ian Bingham
 */

#include <math.h>
#include "sunpos.h"

// solar panel codes
#define CONTROL 0
#define TILTER 1

// motor step resolution (degrees)
#define S1 1.8
#define S2 0.9
#define S4 0.45
#define S8 0.225
#define S16 0.1125
#define S32 0.05625

// range of motion of panel
#define MOTOR_RANGE 4000.0
#define PANEL_RANGE 40.0
#define GEAR_RATIO 100.0
#define START_POSITION 160.0

// specify motor step size
const double stepSize = S1;

double currDegrees = 0.0;
double currAzi;
double prevAzi;
double angleLeftover;

String inputString;         // a string to hold incoming data
boolean stringComplete;  // whether the string is complete

// intermediate GPS values
String timee, latitude, longitude, date;
int latDir, longDir;
boolean gpsValid;

// motor driver pin definitions
// note: 1.8 degrees per full step
const int stepPin = 12;
const int dirPin = 13;
const int reset = 30;
const int sleep = 32;
const int MS1 = 22;
const int MS2 = 24;
const int MS3 = 26;

//voltage sensor
const int voltagePinC = A6;
const int voltagePinT = A7;
float voutC = 0.0;
float voutT = 0.0;
float vinC = 0.0; // final result
float vinT = 0.0; // final result

//sunpos structures
cTime c_time;
cLocation c_location;
cSunCoordinates c_suncoordinates;

void setup() {
    // initialize serial:
    Serial.begin(9600);
    Serial1.begin(9600);
    inputString.reserve(200); // reserve 200 bytes for the inputString:
    
    // sets motor driver outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(reset, OUTPUT);
    pinMode(sleep, OUTPUT);
    digitalWrite(dirPin, HIGH);
    digitalWrite(reset, HIGH);
    digitalWrite(sleep, LOW);

    pinMode(voltagePinC, INPUT);
    pinMode(voltagePinT, INPUT);
    
    pinMode(MS1, OUTPUT);
    pinMode(MS2, OUTPUT);
    pinMode(MS3, OUTPUT);
    
    if (stepSize == S1) {
        digitalWrite(MS1, LOW);
        digitalWrite(MS2, LOW);
        digitalWrite(MS3, LOW);
    }
    else if (stepSize == S2) {
        digitalWrite(MS1, HIGH);
        digitalWrite(MS2, LOW);
        digitalWrite(MS3, LOW);
    }
    else if (stepSize == S4) {
        digitalWrite(MS1, LOW);
        digitalWrite(MS2, HIGH);
        digitalWrite(MS3, LOW);
    }
    else if (stepSize == S8) {
        digitalWrite(MS1, HIGH);
        digitalWrite(MS2, HIGH);
        digitalWrite(MS3, LOW);
    }
    else if (stepSize == S16) {
        digitalWrite(MS1, LOW);
        digitalWrite(MS2, LOW);
        digitalWrite(MS3, HIGH);
    }
    else if (stepSize == S32) {
        digitalWrite(MS1, HIGH);
        digitalWrite(MS2, HIGH);
        digitalWrite(MS3, HIGH);
    }
    
    currAzi = 0.0;
    prevAzi = 0.0;
    angleLeftover = 0.0;
    
    gpsValid = true;
    inputString = "";
    stringComplete = false;
}

void loop() {
    currAzi = c_suncoordinates.dAzimuth - START_POSITION;
    if (prevAzi == 0 && currAzi > 0)
        prevAzi = currAzi;

    if(abs(currAzi - prevAzi) < 5)
    {
        double motorDiff = (currAzi - prevAzi) + angleLeftover;

        if (gpsValid && motorDiff > 0.9 && currAzi > 0 && currDegrees < PANEL_RANGE)
        {
            angleLeftover = motorDiff - 0.9;
            rotate(motorDiff * GEAR_RATIO);
            prevAzi = currAzi;
        }
    }
    refresh_voltage();
    serial_output();
    }

void rotate(double angle)
{
    int dir;
    if (angle >= 0)
    {
        digitalWrite(dirPin,HIGH);
        dir = 1;
    }
    else
    {
        digitalWrite(dirPin,LOW);
        dir = -1;
    }
    double theta = abs(angle);
    int pulses = (int) round(theta / stepSize);
    
    if (currDegrees + (dir * pulses * stepSize) > MOTOR_RANGE)
        pulses = (int) floor((MOTOR_RANGE - currDegrees) / stepSize);
    
    if (pulses != 0)
    {
        digitalWrite(sleep, HIGH);
        delay(1000);
        for(int x = 0; x < pulses; x++) {
            digitalWrite(stepPin,HIGH);
            delayMicroseconds(1000);
            digitalWrite(stepPin,LOW);
            delayMicroseconds(1000);
        }
        digitalWrite(sleep, LOW);
        currDegrees += (dir * pulses * stepSize / GEAR_RATIO);
    }
}

void serialEvent1()
{
    while (Serial1.available())
    {
        char inChar = (char)Serial1.read();
        inputString += inChar;
        if (inChar == '$')
            stringComplete = true;

        if(stringComplete)
        {
            if (parse(inputString))
            {
                refresh_sunpos_data();
                track();
                gpsValid = true;
            }
            else
                gpsValid = false;
            
            inputString = "";
            stringComplete = false;
        }
    }
}

boolean parse(String in)
{
    String type = "GPRMC,";
    if (!in.startsWith(type))
        return false;
    
    int firstComma = 0;
    int secondComma = 0;
    int count = 0;
    String val = "";
    while (in.indexOf(',', firstComma) != -1)
    {
        secondComma = in.indexOf(',', firstComma+1);
        val = in.substring(firstComma+1, secondComma);
        switch (count)
        {
        case 1: //time
            timee = val;
            break;
        case 2: //status (A or V)
            if (val != "A")
                return false;
            break;
        case 3: //latitude
            latitude = val;
            break;
        case 4: //latitude direction
            if (val.equals("N"))
                latDir = 1;
            else if (val.equals("S"))
                latDir = -1;
            else
                return false;
            break;
        case 5: //longitude
            longitude = val;
            break;
        case 6: //longitude direction
            if (val.equals("E"))
                longDir = 1;
            else if (val.equals("W"))
                longDir = -1;
            else
                return false;
            break;
        case 9: //date
            if (val.length() != 6)
                return false;
            date = val;
            break;
        }
        count++;
        firstComma = secondComma;
    }
    count = 0;
    firstComma = 0;
    secondComma = 0;
    return true;
}

double convert_latitude()
{
    int period;
    period = latitude.indexOf('.', 0);
    if (period)
        return latitude.toInt() / 100.00000 * latDir;
    return 0;
}

double convert_longitude()
{
    int period;
    period = longitude.indexOf('.', 0);
    if (period)
        return longitude.toInt() / 100.00000 * longDir;
    return 0;
}

void refresh_sunpos_data(void)
{
    c_time.iYear = ("20" + date.substring(4,6)).toInt(); //assume the 21st century
    c_time.iMonth = (date.substring(2,4)).toInt();
    c_time.iDay = (date.substring(0,2)).toInt();
    c_time.dHours = timee.substring(0,2).toInt();
    c_time.dMinutes = timee.substring(2,4).toInt();
    c_time.dSeconds = timee.substring(4,6).toInt();
    c_location.dLongitude = convert_longitude();
    c_location.dLatitude = convert_latitude();
}

void track(void)
{
    sunpos(c_time,c_location,&c_suncoordinates);
}

void refresh_voltage()
{
    float valueC = analogRead(voltagePinC);
    float valueT = analogRead(voltagePinT);
    voutC = (valueC * 5.0) / 1024.0;
    voutT = (valueT * 5.0) / 1024.0;
    vinC = voutC*4.7;
    vinT = voutT*4.7;
}

void serial_output()
{
    Serial.println(String(CONTROL) + " " + String(vinC));
    Serial.println(String(TILTER) + " " + String(vinT));
}