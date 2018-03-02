#include <SoftwareSerial.h>
//#include "P:\seniorProject\Solar-Algorithm\spa.h"
//#include "C:\Users\Pham\Documents\Arduino\Solar-Algorithm\spa.h"
//#include "P:\seniorProject\Solar-Algorithm\Buffers.h"
//#include "P:\seniorProject\Solar-Algorithm\SCI0.h"
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
}


void loop() {
  if(mySerial.available()){
    //Serial.write(mySerial.read());
    
    char inChar = (char)mySerial.read();
    inputString += inChar;
    if (inChar == '\n')
      stringComplete = true;
  }

  //delay(5000);
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


//void refresh_spa_data(void)
//{
//  spa.timezone = -8.0; //double
//  //int
//  spa.year = ("20" + date.substring(4,6)).toInt();
//  spa.month = (date.substring(2,4)).toInt();
//  spa.day = (date.substring(0,2)).toInt();
//  spa.hour = timee.substring(0,2).toInt() + spa.timezone;
//  if (spa.hour < 0)
//    spa.hour += 24;
//  spa.minute = timee.substring(2,4).toInt();
//  spa.second = timee.substring(4,6).toInt();
//  //double
//  spa.delta_t = 0.0;
//  spa.longitude = convert_coordinates(1);
//  spa.latitude = convert_coordinates(0);
//  
//  spa.elevation = 0.0;
//  spa.pressure = 30.0;
//  spa.temperature = 30.0;
//  //spa.slope =
//  //spa.azm_rotation =
//  //spa.atmos_refrac =
//  spa.function = SPA_ALL;
//  
//  char buf[200];
//  sprintf(buf, "year: %d\nmonth: %d\nday: %d\nhour: %d\nminute: %d\nsecond: %d\n", spa.year,spa.month,spa.day,spa.hour,spa.minute,spa.second);
//  Serial.println("Longitude: " + String(spa.longitude));
//  Serial.println("Latitude: " + String(spa.latitude));
//  Serial.println("Timezone: " + String(spa.timezone));
//  Serial.println(buf);
//}
//
//
//
///*
//TT = TAI + 32.184sec
//TAI = UTC + 37sec
//TT = UTC + 69.184sec
//if leap year, add 1.00sec to these numbers
//*/
//void track()
//{
//  int result = spa_calculate(&spa);
//  if (result == 0)
//  {
//    Serial.println(spa.zenith);
//    Serial.println(spa.azimuth180);
//    Serial.println(spa.azimuth);
//    Serial.println(spa.incidence);
//    Serial.println(spa.suntransit);
//    Serial.println(spa.sunrise);
//    Serial.println(spa.sunset);
//  }
//}


