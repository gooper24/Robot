#include <ESP8266WiFi.h>

const char* ssid = "UPC1369912";
const char* password = "BPEPJQUU";

int ledPin = 2;
bool ledOn = true;

WiFiServer server(80); //just pick any port number you like

void setup() {
  Serial.begin(115200);
  delay(10);
Serial.println(WiFi.localIP());
  // prepare GPIO2
  pinMode(ledPin, OUTPUT);
  digitalWrite(2, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();

  // Match the request
  if (req.indexOf("") != -10) {  //checks if you're on the main page

    if (req.indexOf("/OFF") != -1) { //checks if you clicked OFF
      ledOn = false;
      Serial.println("You clicked OFF");
    }
    if (req.indexOf("/ON") != -1) { //checks if you clicked ON
      ledOn = true;
      Serial.println("You clicked ON");
    }
    if (req.indexOf("/SWITCH") != -1) {
      ledOn = !ledOn;
      Serial.println("You clicked SWITCH");
    }
    digitalWrite(ledPin, !ledOn);
  }
  else {
    Serial.println("invalid request");
   client.stop();
    return;
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<strong> The LED is: ";
  if(ledOn)
  {
    s += "ON </strong>";
  }
  else
  {
    s += "OFF </strong>";
  }
  s += "<br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Turn LED ON \" onclick=\"location.href='/ON'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Turn LED OFF\" onclick=\"location.href='/OFF'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"bl\" value=\"Switch LED\" onclick=\"location.href='/SWITCH'\">";
  s += "</html>\n";

  client.flush();


  // Send the response to the client
  client.print(s);
  delay(1);


  
}
