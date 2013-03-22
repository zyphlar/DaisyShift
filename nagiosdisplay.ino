//**************************************************************//
//  Name    : DaisyShift - Daisy Chain Shift Register display
//              using Arduino Ethernet
//  Author  : Will Bradley
//  Date    : 18 Feb, 2012
//  Version : 1.0
//  Notes   : Outputs HTTP data to 74HC595 shift registers
//            Based on ShiftOut and WebClient examples from Arduino
//  URL     : http://zyphon.com/daisyshift
//****************************************************************

#include <SPI.h>
#include <Ethernet.h>

// Variables you should change based on preferences
int updateFrequency = 10; // in seconds
int numrows = 2; // number of shift registers in the daisy chain
char* queryURL = "/nag.php?minimal=1";  // path on the webserver

// Enter a MAC address and IP address info for your Arduino below.
// These values will change based on your network.
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xAC, 0xB3 };
byte ip[] = { 192,168,1,222 };
byte subnet[] = { 255,255,255,0 };
byte gateway[] = { 192,168,1,1 };

// Enter the IP address of the web server to query.
byte server[] = { 192,168,1,24 };

//Pin connected to ST_CP of 74HC595
int latchPin = 6;
//Pin connected to SH_CP of 74HC595
int clockPin = 5;
////Pin connected to DS of 74HC595
int dataPin = 7;

// Runtime variables -- no need to change
long updateTime = 0; // just a counter
String httpresponse = ""; // for storing server responses

// Initialize the Ethernet client library
EthernetClient client;

void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
    // start the Ethernet connection:
  Ethernet.begin(mac, ip, gateway, subnet);
  // start the serial library:
  Serial.begin(9600);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  // flash LEDs to indicate initialization
  displayData(0);
  displayData(0);
  delay(100);
  displayData(255);
  displayData(255);
  delay(100);
  displayData(0);
  displayData(0);
  delay(100);
  displayData(255);
  displayData(255);
  delay(100);
  displayData(0);
  displayData(0);
  
}


void displayData(int numberToDisplay) {
        // take the latchPin low so 
        // the LEDs don't change while you're sending in bits:
        digitalWrite(latchPin, LOW);
        // shift out the bits:
        shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  
    
        //take the latch pin high so the LEDs will light up:
        digitalWrite(latchPin, HIGH);
        // pause before next value:
        delay(1); 
}

// Function that prints data from the server
void printData(char* data, int len) {
  Serial.print("Printing ");
  Serial.print(data);
  Serial.print(" with a length of ");
  Serial.println(len);
  
  String matrix;
  
  digitalWrite(latchPin, LOW); // start writing to LEDs
  
  // Print the data returned by the server
  // Note that the data is not null-terminated, may be broken up into smaller packets, and 
  // includes the HTTP header. 
  for(int i=0;i<len;i++) {
    if(data[i] == '^')
    {
      for(int rows=0;rows<numrows;rows++){
        int rowData = 0;
        for(int cols=0;cols<8;cols++){
          i++;
          char thisData = 1;  // 1 is 'off'
          if(i<len) {  // make sure we're still within the bounds of the data
            thisData = data[i];
            Serial.print(thisData);
          }
          
          if(thisData == '0') {
            // a 0 in Nagios means the LED should be on
            // the absolute value of cols-7 is used so binary 10000000 shows up as 128 instead of 1 (left light on, not right)
            rowData += (1<<abs(cols));  
            Serial.print((1<<abs(cols)));
            Serial.print("+");
          }
          if(thisData == '1') {
            // any other value means the LED should be off
            Serial.print("0");
            Serial.print("+");
          }
          if(thisData == '2') {
            // any other value means the LED should be off
            Serial.print("0"); 
            Serial.print("+");
          }

          matrix += thisData;
        }
        shiftOut(dataPin, clockPin, MSBFIRST, rowData);  // write to LEDs 
        Serial.print("=");
        Serial.println(rowData);
      }
      Serial.println('*');
      Serial.println(matrix);
    }
  } 
  digitalWrite(latchPin, HIGH);  // stop writing to LEDs
}

void loop() { 
  
  // Check if it's time to get an update
  if (millis() >= updateTime) {
    Serial.println("connecting...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.print("GET ");
      client.print(queryURL);
      client.println(" HTTP/1.0");
      client.println();
      
    } 
    else {
      // kf you didn't get a connection to the server:
      Serial.println("connection failed");
    }
    
    // Get an update every 30 seconds
    updateTime += 1000 * updateFrequency;
  }
  
  while (client.available()) {
    Serial.print(".");
      char c = client.read();i
      }
    }
  
  if(!client.available() && httpresponse.length()>0) {
    Serial.println("Parse!");
    int a = httpresponse.indexOf('^');
    int b = httpresponse.indexOf('$');
    String data = httpresponse.substring(a,b);
    Serial.println(data);
    char charData[50];
    data.toCharArray(charData, sizeof(charData));
    Serial.println(charData);
    printData(charData, strlen(charData));
    
    httpresponse = "";
  }
    
    
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    client.stop();
  }
    
} // end of loop
