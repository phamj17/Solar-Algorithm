#include <SoftwareSerial.h>
//#include "P:\seniorProject\Solar-Algorithm\spa.h"
#include "C:\Users\Pham\Documents\Arduino\Solar-Algorithm\spa.h"
//#include "P:\seniorProject\Solar-Algorithm\Buffers.h"
//#include "P:\seniorProject\Solar-Algorithm\SCI0.h"

#define LATITUDE 0
#define LONGITUDE 1

SoftwareSerial mySerial(10,11); //RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String timee, latitude, longitude, date;
int lat_dir, long_dir;
boolean flag = false;

spa_data spa;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  mySerial.begin(9600);
  inputString.reserve(200); // reserve 200 bytes for the inputString:
}

void loop() {
  if(mySerial.available()){
    //Serial.write(mySerial.read());
    char inChar = (char)mySerial.read();
    inputString += inChar;
    if (inChar == '\n')
      stringComplete = true;
  }

  if(stringComplete){
    //Serial.print(inputString);
    parse(inputString);
    inputString = "";
    stringComplete = false;
    refresh_spa_data();
  }
}

void parse(String in)
{
  //Serial.println("we parsin");
  String type = "$GPRMC,";
  if (!in.startsWith(type))
  {
    Serial.println("not the type");
    return;
  }
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
        Serial.println("time: " + timee);
        break;
      //case 2: //status (A or V)
      case 3: //latitude
        latitude = val;
        Serial.println("latitude: " + latitude);
        break;
      case 4: //latitude direction
        if (val.equals("N"))
          lat_dir = 1;
        else
          lat_dir = -1;
        Serial.println("lat_dir: " + String(lat_dir));
        break;
      case 5: //longitude
        longitude = val;
        Serial.println("longitude: " + longitude);
        break;
      case 6: //longitude direction
        if (val.equals("E"))
          long_dir = 1;
        else
          long_dir = -1;
        Serial.println("long_dir: " + String(long_dir));
        break;
      case 9: //date
        date = val;
        Serial.println("date: " + date);
        break;
    }
    count++;
    firstComma = secondComma;
    //Serial.println("firstComma = " + String(firstComma));
  }
  count = 0;
  firstComma = 0;
  secondComma = 0;
  return;
}


void refresh_spa_data(void)
{
  spa.timezone = -8.0; //double
  //int
  spa.year = ("20" + date.substring(4,6)).toInt();
  spa.month = (date.substring(2,4)).toInt();
  spa.day = (date.substring(0,2)).toInt();
  spa.hour = timee.substring(0,2).toInt() + spa.timezone;
  if (spa.hour < 0)
    spa.hour += 24;
  spa.minute = timee.substring(2,4).toInt();
  spa.second = timee.substring(4,6).toInt();
  //double
  //spa.delta_t = wtf
  spa.longitude = convert_coordinates(1);
  spa.latitude = convert_coordinates(0);
  /*
  spa.elevation =
  spa.pressure =
  spa.temperature =
  spa.slope =
  spa.azm_rotation =
  spa.atmos_refrac =
  spa.function = //int
  */
  char buf[200];
  sprintf(buf, "year: %d\nmonth: %d\nday: %d\nhour: %d\nminute: %d\nsecond: %d\n", spa.year,spa.month,spa.day,spa.hour,spa.minute,spa.second);
  Serial.println("Longitude: " + String(spa.longitude));
  Serial.println("Latitude: " + String(spa.latitude));
  Serial.println("Timezone: " + String(spa.timezone));
  Serial.println(buf);
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


/*
TT = TAI + 32.184sec
TAI = UTC + 37sec
TT = UTC + 69.184sec
if leap year, add 1.00sec to these numbers

void track()
{
  
}
*/

