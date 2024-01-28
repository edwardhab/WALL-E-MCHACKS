//Wifi Connection Authentification details
#define SECRET_SSID "SSID"
#define SECRET_PASS "password"
 

#include "WiFiS3.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


//Servo motor
Servo servoBig;

//initialize the liquid crystal library
LiquidCrystal_I2C lcd(0x27, 20, 4);

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0; 

// Positions for servo
int garbage_position = 0;
int recycle_position = 90;
int metals_position = 180;

// Current position
int currentPosition = 0;

// Initialize LEDS and send easy request to validate that webserver communication is up
int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  servoBig.attach(3);   //Servo
  lcd.init();           //initialize lcd screen
  lcd.backlight();      // turn on the backlight
  //clear display
  lcd.clear();
  // tell the screen to write on the top row
  lcd.setCursor(0,0);
  lcd.print("Hello From");

  // tell the screen to write on the bottom row
  lcd.setCursor(0,1);
  lcd.print("WALL-E");

  //WebServer Init
  Serial.begin(9600);      // initialize serial communication
  pinMode(led, OUTPUT);    // set the LED pin mode

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                  

    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  // start the web server on port 80
  server.begin();                           
  printWifiStatus();                        
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();

  //Server is ready
  if (client) {  

    // print a message out the serial port                          
    Serial.println("new client");           
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                    
        if (c == '\n') {                  
          // if the current line is blank, you got two newline characters in a row.
          // end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/H\">here</a> turn the LED on<br></p>");
            client.print("<p style=\"font-size:7vw;\">Click <a href=\"/L\">here</a> turn the LED off<br></p>");  
            client.println();
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      
        }

        // GET /H turns the LED on, to be used to test webserver
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               
        }
        // GET /L turns the LED off, to be used to test webserver
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                
        }
        // GET /3 rotates servo to garbage container
        if (currentLine.endsWith("GET /3")){            
           //Clear LCD display
           lcd.clear();
           Serial.println("Garbage");
           lcd.setCursor(0,0);
           lcd.print("Material Type:");
           lcd.setCursor(1,1);
           lcd.print("Garbage");
           servoBig.write(0);
        }
        // GET /1 rotates servo to recycling container
        if (currentLine.endsWith("GET /1")){            
          Serial.println("Recycle");
          //Clear display
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Material Type:");
          lcd.setCursor(1,1);
          lcd.print("Recycle");
          servoBig.write(90);
        }
        // GET /2 rotates servo to metal container
        if (currentLine.endsWith("GET /2")){           
           Serial.println("Metal");
           //Clear display
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("Material Type:");
           lcd.setCursor(1,1);
           lcd.print("Metal");
           servoBig.write(180);
        }
      }    
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//Displays to serial monitor connectivity information 
void printWifiStatus() {
  // print the SSID of the network 
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print board's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP: ");
  Serial.println(ip);

  // print received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("signal strength:");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("http://");
  Serial.println(ip);
}

