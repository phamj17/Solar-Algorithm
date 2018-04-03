#include <math.h>
#include "sunpos.h"

// motor step resolution (degrees)
#define S1 1.8
#define S2 0.9
#define S4 0.45
#define S8 0.225
#define S16 0.1125
#define S32 0.05625

// range of motion of panel
#define MAX_RANGE 10440.0
#define GEAR_RATIO 100.0
//#define TRACK_INTERVAL 1000*60*10

const double stepSize = S1;
double currDegrees = 0.0;

double currAzi;
double prevAzi;
double angleLeftover;

String inputString;         // a string to hold incoming data
boolean stringComplete;  // whether the string is complete
String timee, latitude, longitude, date;
//String concateTime;
int latDir, longDir;
boolean gpsValid;

int timer1, timer2;

// motor driver pin definitions
// note: 1.8 degrees per full step
const int stepPin = 12;
const int dirPin = 13;
const int reset = 30;
const int sleep = 32;
const int MS1 = 22;
const int MS2 = 24;
const int MS3 = 26;

//current sensor
const int currentPinC = A0;
const int currentPinT = A1;
float vpp = 5.0 / 1024;
float sensitivity = 0.066;
float currentC = 0.0;
float currentT = 0.0;

//voltage sensor
const int voltagePinC = A6;
const int voltagePinT = A7;
float voutC = 0.0;
float voutT = 0.0;
float vinC = 0.0;
float vinT = 0.0;
const float voltageR1 = 30000.0;
const float voltageR2 = 7500.0;

//sunpos structures
cTime c_time;
cLocation c_location;
cSunCoordinates c_suncoordinates;

void setup() {
    // initialize serial:
    Serial.begin(9600);
    Serial1.begin(9600);
    inputString.reserve(200); // reserve 200 bytes for the inputString:
    
    timer1 = 0;
    timer2 = 58;
    
    // sets motor driver outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(reset, OUTPUT);
    pinMode(sleep, OUTPUT);
    digitalWrite(dirPin, HIGH);
    digitalWrite(reset, HIGH);
    digitalWrite(sleep, LOW);

    pinMode(currentPinC, INPUT);
    pinMode(currentPinT, INPUT);
    pinMode(voltagePinC, INPUT);
    pinMode(voltagePinT, INPUT);
    
    // MS1, MS2, MS3
    // 000: full step
    // 100: half step
    // 010: 1/4 step
    // 110: 1/8 step
    // 001: 1/16 step
    // 101: 1/32 step
    // 011: 1/32 step
    // 111: 1/32 step
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
    currAzi = c_suncoordinates.dAzimuth - 160; 
    if (prevAzi == 0 && currAzi > 0)
      prevAzi = currAzi;
    //else
    //  return;
      
    if(abs(currAzi - prevAzi) < 5){
      double motor_diff = (currAzi - prevAzi) + angleLeftover;
      //delay(500);
      //Serial.println("Previous Azimuth: " + String(prevAzi));
      //Serial.println("Current Azimuth: " + String(currAzi));
      //Serial.println("Motor Diff: " + String(motor_diff));
      //Serial.println("Curr Degrees: " + String(currDegrees));
      
      //Serial.println("gpsValid: " + String(gpsValid));
      if (gpsValid && motor_diff > 0.9 && currAzi > 0 && currDegrees < 40.0){
        angleLeftover = motor_diff - 0.9;
        //Serial.println("Anlge Leftover: " + String(angleLeftover));
		//digitalWrite(sleep, HIGH);
        rotate(motor_diff * GEAR_RATIO);
        //rotate(180.0);
        prevAzi = currAzi;
      }
      //else if (currDegrees > 40.0 || currAzi > 205.0){
      //  digitalWrite(sleep, LOW);
      //}
      
      // if(timee.compare("000000") == 0){
          // Serial.println(timee);
          // reset_motor();
          // currDegrees = 0.0;
      //}
      // if (gpsValid && (motor_diff > 0.12))
      // {
         // rotate(motor_diff); //3.75 seems to be the minimum
         // Serial.println("we rotatin");
         // prevAzi = currAzi;
      // }
      //delay(1000);
      //if(timer1 < 120){
        //rotate(90.0);    
        //timer1++;
      //}
      //if(timer1 > 120){
        //rotate(-90.0); 
        //timer2--;
        //if(timer2 == 0)
          //delay(100000);
      //}
      //Serial.println(" --- ROTATIONS: " + String(timer1));
      // Serial.println("timer: " + String(timer));
      // Serial.println("currDegrees: " + String(currDegrees));
      // Serial.println("------------------------");
      // if (currDegrees >= MAX_RANGE) {
          // reset_motor();
          // timer = 0;
      // }
      //reset_motor();
    
    //else
      //Serial.println("INVALID: " + String(currAzi - prevAzi));
	}
	refresh_voltage();
	refresh_current();
    serial_output();
}

void rotate(double angle)
{
    //determine direction of rotation
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
    //Serial.println("pulses: " + String(pulses));
    //Serial.println("theta: " + String(theta));
    //Serial.println("stepSize: " + String(stepSize));
    if (currDegrees + (dir * pulses * stepSize) > MAX_RANGE)
        pulses = (int) floor((MAX_RANGE - currDegrees) / stepSize);
    
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
        //digitalWrite(stepPin,LOW);
        digitalWrite(sleep, LOW);
        currDegrees += (dir * pulses * stepSize / GEAR_RATIO);
        //Serial.println(String(currDegrees));
    }
}

void reset_motor()
{
    rotate(-1 * currDegrees * GEAR_RATIO);
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
            //Serial.println(inputString);
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
    double timezone = -7.0;
    c_time.iYear = ("20" + date.substring(4,6)).toInt();
    c_time.iMonth = (date.substring(2,4)).toInt();
    c_time.iDay = (date.substring(0,2)).toInt();
    c_time.dHours = timee.substring(0,2).toInt();// + timezone;
    c_time.dMinutes = timee.substring(2,4).toInt();
    c_time.dSeconds = timee.substring(4,6).toInt();
    c_location.dLongitude = convert_longitude();
    c_location.dLatitude = convert_latitude();
    
    //concateTime = String(c_time.dHours) + String(c_time.dMinutes) + String(c_time.dSeconds);
    
    // Serial.println("Valid: " + String(gpsValid));
    // Serial.println("Month: " + String(c_time.iMonth));
    // Serial.println("Day: " + String(c_time.iDay));
    // Serial.println("Year: " + String(c_time.iYear));
    // Serial.println("Hours: " + String(c_time.dHours));
    // Serial.println("Minutes: " + String(c_time.dMinutes));
    // Serial.println("Seconds: " + String(c_time.dSeconds));
    // Serial.println("Longitude: " + String(c_location.dLongitude));
    // Serial.println("Latitude: " + String(c_location.dLatitude));
}

void track(void)
{
    sunpos(c_time,c_location,&c_suncoordinates);
}

void refresh_current()
{
    float countsC = analogRead(currentPinC);
    float countsT = analogRead(currentPinT);
    float voltsC = countsC * vpp;
    float voltsT = countsT * vpp;
    currentC = (voltsC - (103.0 * vpp)) / (2 * sensitivity);
    currentT = (voltsT - (103.0 * vpp)) / (2 * sensitivity);
    //Serial.println("---------------------");
    //Serial.println("CurrentC: " + String(currentC));
    //Serial.println("CurrentT: " + String(currentT));
    //Serial.println(countsC);
    //Serial.println(countsT);
    //delay(500);
}

void refresh_voltage()
{
    float valueC = analogRead(voltagePinC);
    float valueT = analogRead(voltagePinT);
    voutC = (valueC * 5.0) / 1024.0;
    voutT = (valueT * 5.0) / 1024.0;
    vinC = voutC*4.7; ;/// (voltageR2/(voltageR1+voltageR2));
    vinT = voutT*4.7;
    timer1++;
    //Serial.println("---------------------");
    //Serial.println("VoltsC: " + String(vinC));
    //Serial.println("VoltsT: " + String(vinT));
    //delay(500);
}

void serial_output()
{
    Serial.println("CONTROL,valid:"
                    +String(gpsValid)+",voltage:"
                    +String(vinC)+",current:"
                    +String(currentC)+",hour:"
                    +String(c_time.dHours)+",minute:"
                    +String(c_time.dMinutes)+",second:"
                    +String(c_time.dSeconds));
    Serial.println("TILTER,valid:"
                    +String(gpsValid)+",voltage:"
                    +String(currentT)+",hour:"
                    +String(c_time.dHours)+",minute:"
                    +String(c_time.dMinutes)+",second:"
                    +String(c_time.dSeconds));
    //delay(500);
}