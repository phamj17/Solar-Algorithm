// SoftwareSerial - Version: Latest 
#include <SoftwareSerial.h>
//#include <Gpsneo.h>

/*
  Serial Event example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop prints the string and
 clears it.

 A good test for this is to try it with a GPS receiver
 that sends out NMEA 0183 sentences.

 Created 9 May 2011
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialEvent

 */
SoftwareSerial mySerial(10,11); //RX, TX
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String timee, latitude, longitude, date;
boolean flag = false;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  mySerial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  /*
  if (stringComplete) {
    Serial.print("\nTime: ");
    Serial.print(timee);
    Serial.print("\nLatitude: ");
    Serial.print(latitude);
    Serial.print("\nLongitude: ");
    Serial.print(longitude);
    Serial.print("\nDate: ");
    Serial.print(date);
    Serial.print("\n");
    Serial.print(inputString);
    // clear the string:
    inputString = "";
    timee = "";
    latitude = "";
    longitude = "";
    date = "";
    stringComplete = false;
  }
  */
  
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
  //parse();
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
    secondComma = in.indexOf(',', firstComma);
    Serial.println("secondComma = " + String(secondComma));
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
      //case 4: //latitude direction
      case 5: //longitude
        longitude = val;
        Serial.println("longitude: " + longitude);
        break;
      //case 6: //longitude direction
      case 9: //date
        date = val;
        Serial.println("date: " + date);
        break;
    }
    count++;
    firstComma = secondComma;
    Serial.println("firstComma = " + String(firstComma));
  }
  return;
}

/*
void parse() {
  while (mySerial.available()) {
    // get the new byte:
    Serial.println("reading data...\n");
    char inChar = (char)mySerial.read();
    // add it to the inputString:
    inputString += inChar;
    //Serial.println(inputString);
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    //char * GPRMC = "$GPRMC, ";
    //char * temp_input = inputString;
    if (inputString.equals("$GPRMC, ")==0) {
      flag = true;
      //Serial.print(flag);
    }

    if (inChar == '\n' && flag) {
      Serial.println("wudddddduuuuuppppppp");
      // 1: $GPRMC
      // 2: UTC Position: hhmmss.sss
      // 4: Latitude: ddmm.mmmm
      // 6: Longitude: ddmm.mmmm
      // 10: Date: ddmmyy
      char * start = &inputString[0];
      char * temp = strtok(start, ",");
      int count = 1;
      while (temp != NULL) {
       temp = strtok(NULL, ",\n");
       count++;
       switch (count) {
        case 2:
          timee = temp;
          Serial.println("time: " + timee);
          break;
        case 4:
          latitude = temp;
          Serial.println("latitude: " + latitude);
          break;
        case 6:
          longitude = temp;
          Serial.println("longitude: " + longitude);
          break;
        case 10:
          date = temp;
          Serial.println("date: " + date);
          break;
       }
      }
      Serial.println("done with while.....................................");
      stringComplete = true;
    }
  }
}
*/

