// Best code so far, works with both router and mobile phone hotspot!!. The graphing also works!!.
#include <adafruitmpl.h>
#include <HDC1050-v2.h>
#include <utils.h>
#include <OPT3001.h>
//#include <SPI.h>
#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiServer.h>
#include <Wire.h>
opt3001 instance;
uint32_t resultt =0;
HDC1000 mySensor;
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
float pascals;
float altm ;
float tempC ;
float hdctemp ;
float hdchumi ;

// your network name also called SSID
char ssid[] = "******";
// your network password
char password[] = "*******";
// your network key Index number (needed only for WEP)
int keyIndex = 0;

WiFiServer server(80);
int i = 0;
void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(RED_LED, OUTPUT);      // set the LED pin mode
  instance.begin();
  mySensor.begin();
  baro.begin();
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid);
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");

  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  // you're connected now, so print out the status
  printWifiStatus();

  Serial.println("Starting webserver on port 80");
  server.begin();                           // start the web server on port 80
  Serial.println("Webserver started!");

}
void loop() {
  getSensorData();
//  resultt = instance.readResult();
//  //Serial.println(resultt);
//  float pascals = baro.getPressure();
//  float altm = baro.getAltitude();
//  float tempC = baro.getTemperature();
//  double hdctemp = mySensor.getTemp();
//  double hdchumi = mySensor.getHumi();

  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=utf-8");
          //Content-Type: text/html; charset=utf-8
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println("Access-Control-Allow-Origin: *"); //comment this line if u are facing issues showing things on webpage.
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          client.println("<html>");
          client.print("LUX:");
          client.print(resultt);
          client.print(";");
          client.print("Temperature:");
          client.print(hdctemp);
          client.print(";Humidity:");
          client.print(hdchumi);
          client.print(";");
          client.print("  ");
          client.print("Hg:");
          client.print(pascals/3377);
          client.print(";Height:");
          client.print(altm); //client.print("m ");
          client.print(";Temp:");
          client.print(tempC); //client.print("*C");
          client.println("<br>");
          client.println("</html>");
          client.println();
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
  i++;
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void getSensorData(){
  resultt = instance.readResult();
  Serial.println(resultt);
  pascals = baro.getPressure();
  Serial.println(pascals);
  altm = baro.getAltitude();
  Serial.println(altm);
  tempC = baro.getTemperature();
  Serial.println(tempC);
  hdctemp = mySensor.getTemp();
  Serial.println(hdctemp);
  hdchumi = mySensor.getHumi();
  Serial.println(hdchumi);
}
