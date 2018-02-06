#include <SoftwareSerial.h>
//#include "spa.h"
//#include "Buffers.h"
//#include "SCI0.h"

SoftwareSerial mySerial(10,11); //RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String timee, latitude, longitude, date;
int lat_dir, long_dir;
boolean flag = false;

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
    Serial.print(inputString);
    parse(inputString);
    inputString = "";
    stringComplete = false;
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
  Serial.println("we the type");
  int firstComma = 0;
  int secondComma = 0;
  int count = 0;
  String val = "";
  while (in.indexOf(',', firstComma) != -1)
  {
    //Serial.println("firstComma = " + String(firstComma));
    secondComma = in.indexOf(',', firstComma+1);
    //Serial.println("secondComma = " + String(secondComma));
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
        {
          lat_dir = 1;
        }
        else
        {
          lat_dir = -1;
        }
        Serial.println("lat_dir: " + String(lat_dir));
        break;
      case 5: //longitude
        longitude = val;
        Serial.println("longitude: " + longitude);
        break;
      case 6: //longitude direction
        if (val.equals("E"))
        {
          long_dir = 1;
        }
        else
        {
          long_dir = -1;
        }
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
/*
void track()
{
  
}
*/
