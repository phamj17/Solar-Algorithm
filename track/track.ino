#include <SoftwareSerial.h>
#include "sunpos.h"

#define LATITUDE 0
#define LONGITUDE 1

SoftwareSerial mySerial(10,11); //RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String timee, latitude, longitude, date;
int lat_dir, long_dir;
boolean flag = false;

int timer;

// motor driver pin definitions
// note: 1.8 degrees per full step
const int stepPin = 13;
const int dirPin = 12;
const int reset = 30;
const int sleep = 32;

//MS1, MS2, MS3
// 000: full step
// 100: half step
// 010: quarter step
// 110: eighth step
// 111: sixteenth step
const int MS1 = 22;
const int MS2 = 24;
const int MS3 = 26;

//spa_data spa;
cTime c_time;
cLocation c_location;
cSunCoordinates c_suncoordinates;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  mySerial.begin(9600);
  inputString.reserve(200); // reserve 200 bytes for the inputString:

  timer = 0;
  
  // sets motor driver outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  
  // setup motor step to FULL STEP
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
}

void loop() {
    
  // motor driver stuff
  if (!flag)
  {
    digitalWrite(stepPin, HIGH);
    flag = true;
  }
  else
  {
    digitalWrite(stepPin, LOW);
    flag = false;
  }
  
  if(mySerial.available()){
    //Serial.write(mySerial.read());
    
    char inChar = (char)mySerial.read();
    inputString += inChar;
    if (inChar == '\n')
      stringComplete = true;
  }

  if(stringComplete){
    //Serial.print(inputString);
    
    Serial.println("\n---------STARTING PARSE-------------");
    if (parse(inputString))
    {
      refresh_sunpos_data();
      track();
    }
    Serial.println("---------FINISHED PARSE-------------\n");
    
    //Serial.println("\n---------STARTING SUNPOS DATA-------------");
    //refresh_sunpos_data();
    //Serial.println("---------FINISHED SUNPOS DATA-------------\n");

    //if (timer == 15) {
      //Serial.println("\n---------STARTING TRACK-------------");
    //  track();
      //Serial.println("---------FINISHED TRACK-------------\n");
    //  timer = 0;
    //}
    
    //timer++;
    inputString = "";
    stringComplete = false; 
  } 
}

boolean parse(String in)
{
  //Serial.println("we parsin");
  String type = "$GPRMC,";
  if (!in.startsWith(type))
  {
    Serial.println("Not the type... Exiting parse...");
    return false;
  }
  int firstComma = 0;
  int secondComma = 0;
  int count = 0;
  String val = "";
  boolean change = false;
  while (in.indexOf(',', firstComma) != -1)
  {
    secondComma = in.indexOf(',', firstComma+1);
    val = in.substring(firstComma+1, secondComma);
    switch (count)
    {
      case 1: //time
        timee = val;
        change = true;
        Serial.println("time: " + timee);
        break;
      //case 2: //status (A or V)
      case 3: //latitude
        latitude = val;
        change = true;
        Serial.println("latitude: " + latitude);
        break;
      case 4: //latitude direction
        if (val.equals("N"))
          lat_dir = 1;
        else
          lat_dir = -1;
        change = true;
        Serial.println("lat_dir: " + String(lat_dir));
        break;
      case 5: //longitude
        longitude = val;
        change = true;
        Serial.println("longitude: " + longitude);
        break;
      case 6: //longitude direction
        if (val.equals("E"))
          long_dir = 1;
        else
          long_dir = -1;
        change = true;
        Serial.println("long_dir: " + String(long_dir));
        break;
      case 9: //date
        date = val;
        change = true;
        Serial.println("date: " + date + "\n");
        break;
    }
    count++;
    firstComma = secondComma;
    //Serial.println("firstComma = " + String(firstComma));
  }
  count = 0;
  firstComma = 0;
  secondComma = 0;
  return change;
}

/*
 * latitude: 4534.30846
 * lat_dir: 1
 * longitude: 12243.67538
 * long_dir: -1
 */
double convert_coordinates(int code)
{
  int period;
  if (code == LATITUDE)
  {
    period = latitude.indexOf('.', 0);
    if (period)
      return latitude.toInt() / 100.00000 * lat_dir;
  }
  else
  {
    period = longitude.indexOf('.', 0);
    if (period)
      return longitude.toInt() / 100.00000 * long_dir;
  }
  return 0;
}

void refresh_sunpos_data(void)
{
  double timezone = -8.0;
  
  //int
  c_time.iYear = ("20" + date.substring(4,6)).toInt();
  c_time.iMonth = (date.substring(2,4)).toInt();
  c_time.iDay = (date.substring(0,2)).toInt();
  
  //double
  c_time.dHours = timee.substring(0,2).toInt() + timezone;
  if (c_time.dHours < 0)
    c_time.dHours += 24;
  c_time.dMinutes = timee.substring(2,4).toInt();
  c_time.dSeconds = timee.substring(4,6).toInt();
  
  //double
  c_location.dLongitude = convert_coordinates(1);
  c_location.dLatitude = convert_coordinates(0);

  //char buf[200];
  //sprintf(buf, "\nYear: %d\nMonth: %d\nDay: %d\nHour: %lf\nMinute: %lf\nSecond: %lf\n", c_time.iYear,c_time.iMonth,c_time.iDay,c_time.dHours,c_time.dMinutes,c_time.dSeconds);
  //Serial.println(buf);
  Serial.println("Month: " + String(c_time.iMonth));
  Serial.println("Day: " + String(c_time.iDay));
  Serial.println("Year: " + String(c_time.iYear));
  Serial.println("Hours: " + String(c_time.dHours));
  Serial.println("Minutes: " + String(c_time.dMinutes));
  Serial.println("Seconds: " + String(c_time.dSeconds));
  Serial.println("Longitude: " + String(c_location.dLongitude));
  Serial.println("Latitude: " + String(c_location.dLatitude));
}

void track(void)
{
  sunpos(c_time,c_location,&c_suncoordinates);
  Serial.println("Zenith Angle: " + String(c_suncoordinates.dZenithAngle));
  Serial.println("Azimuth: " + String(c_suncoordinates.dAzimuth));
}
