#include <SoftwareSerial.h>
#include "P:\seniorProject\Solar-Algorithm\spa.h"
//#include "P:\seniorProject\Solar-Algorithm\Buffers.h"
//#include "P:\seniorProject\Solar-Algorithm\SCI0.h"
#include <LiquidCrystal.h>

#define LATITUDE 0
#define LONGITUDE 1

SoftwareSerial mySerial(10,11); //RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String timee, latitude, longitude, date;
int lat_dir, long_dir;
boolean flag = false;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 50, en = 52, d4 = 47, d5 = 49, d6 = 51, d7 = 53;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

spa_data spa;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  mySerial.begin(9600);
  inputString.reserve(200); // reserve 200 bytes for the inputString:

  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
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
    Serial.print(inputString);
    parse(inputString);
    inputString = "";
    stringComplete = false;

    refresh_spa_data();
    lcdprint();
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
  spa.year = ("20" + date.substring(4,5)).toInt();
  spa.month = (date.substring(0,1)).toInt();
  spa.day = (date.substring(2,3)).toInt();  
  spa.hour = timee.substring(0,1).toInt() + spa.timezone;
  spa.minute = timee.substring(2,3).toInt();
  spa.second = timee.substring(4,5).toInt();
  //double
  //spa.delta_t = wtf
  spa.longitude = convert_coordinates(LONGITUDE);
  spa.latitude = convert_coordinates(LATITUDE);
  /*
  spa.elevation =
  spa.pressure =
  spa.temperature =
  spa.slope =
  spa.azm_rotation =
  spa.atmos_refrac =
  spa.function = //int
  */
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
      return latitude.toInt() / 100.00 * lat_dir;
  }
  else
  {
    period = longitude.indexOf('.', 0);
    if (period)
      return longitude.toInt() / 100.00 * long_dir;
  }
  return 0;
}


void lcdprint()
{
  //print to lcd
  lcd.setCursor(0, 0);
  lcd.print("time: %d:%d:%d", spa.hour, spa.minute, spa.second);
  
  lcd.setCursor(0, 1);
  lcd.print("lat: %lf", spa.latitude);
  
  lcd.setCursor(0, 2);
  lcd.print("long: %lf", spa.longitude);
  
  lcd.setCursor(0, 3);
  lcd.print("date: %d/%d/%d", spa.month, spa.day, spa.year);
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

