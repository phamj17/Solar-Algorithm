// SoftwareSerial - Version: Latest 
#include <SoftwareSerial.h>

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
boolean stringComplete = true;  // whether the string is complete
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
  //if(mySerial.available()){
  //  Serial.write(mySerial.read());
  //}  
}

void parse(){
  
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent1() {
  while (mySerial.available()) {
    // get the new byte:
    //Serial.println("reading data...\n");
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
          break;
        case 4:
          latitude = temp;
          break;
        case 6:
          longitude = temp;
          break;
        case 10:
          date = temp;
          break;
       }
      }
      stringComplete = true;
    }
  }
}

