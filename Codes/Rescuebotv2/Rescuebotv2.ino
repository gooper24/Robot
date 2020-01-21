// test
// Code voor het aansturen van een ESP8266-E12 via Wifi (schoolrouter Tesla IoT)
// Basis is te vinden op https://www.instructables.com/id/Esp8266-Esp12e-Webserver/#discuss
#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)


// ESP LIBRARIES
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Wire.h> //Voor I2C

MDNSResponder mdns;

ESP8266WebServer server(80);
String webPage;
//const char* ssid     = "";      //wifi name
//const char* password = "E6CA72ADC93A";  //wifi password
const char* ssid     = "Tesla IoT";      //wifi name
const char* password = "fsL6HgjN";  //wifi password

String command;             //String to store app command state.
int speedCar = 400;         // 400 - 1023.
int speed_Coeff = 3;

bool manual = false;

void setup() {
  Wire.begin();
  Serial.begin(74880);
  delay(100);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Verbinding maken met ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi aangesloten");
  Serial.println("IP addres: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP()))
    Serial.println("MDNS responder gestart");

  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });



  server.begin();
  Serial.println("HTTP server gestart");

  webPage += "<a href=\"Vooruit\"><button>Vooruit</button></a>&nbsp;";
  webPage += "<a href=\"Stoppen\"><button>Stoppen</button></a>&nbsp;";
  webPage += "<a href=\"Links\"><button>Links</button></a>&nbsp;";
  webPage += "<a href=\"Rechts\"><button>Rechts</button></a>&nbsp;";
  webPage += "<a href=\"Achteruit\"><button>Achteruit</button></a>&nbsp;";
  webPage += "<a href=\"Linksvoor\"><button>Linksvoor</button></a>&nbsp;";
  webPage += "<a href=\"Rechtsvoor\"><button>rechtsvoor</button></a>&nbsp;";
  webPage += "<a href=\"Linksachter\"><button>Linksachter</button></a>&nbsp;";
  webPage += "<a href=\"Rechtsachter\"><button>Rechtsachter</button></a>&nbsp;";

  commandCheck(); //Controleert invoer gebruiker en voert commands uit
  if (manual)
  {
    webPage += "<a href=\"ToggleManual\"><button>Manual</button></a>&nbsp;";
  }
  if (!manual)
  {
    webPage += "<a href=\"ToggleManual\"><button>Automatic</button></a>&nbsp;";
  }
}
void goAhead() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goBack() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goRight() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goLeft() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goAheadRight() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar / speed_Coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar); //speedCar/speed_Coeff
}

void goAheadLeft() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar); //speedCar/speed_Coeff

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar / speed_Coeff); //speedCar/speed_Coeff

}

void goBackRight() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar / speed_Coeff); //speedCar/speed_Coeff

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goBackLeft() {

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar / speed_Coeff);

}

void stopRobot() {

  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void loop() {
  server.handleClient();
  char sensorState;

  command = server.arg("State");
  if (!manual)
  {
    sensorState = requestSensor();
    switch (sensorState) {
      case '1':
        edgeFound();
        break;
      case 'L':
        leftLineFound();
        break;
      case 'R':
        rightLineFound();
        break;
      case 'O':
        hardLineFound();
      default:
        nothingFound();
        break;
    }
  }
}

char requestSensor() {
  char response;
  Wire.requestFrom(8, 1);
  response = Wire.read();
  return response;

  /*
  while (Wire.available())
  {
    response = Wire.read();
  }
  return response;
  */
}

void hardLineFound()
{
  //Wat te doen als links en rechts een lijn detecteren;
  stopRobot();
}

void leftLineFound()
{
  //Wat te doen als er links een lijn wordt gedetecteerd
  Serial.println("Linkerlijn");
  goRight();
  delay(100);
}

void rightLineFound()
{
  //Wat te doen als er rechts een lijn wordt gedetecteerd
  Serial.println("Rechterlijn");
  goLeft();
  delay(100);
}

void edgeFound()
{
  //als er een afgrond is gevonden
  stopRobot();
  delay(100)
  goBack();
  delay(500);
  goRight();
  delay(2000);
}

void nothingFound()
{
  //als er geen afgrond is gevonden
  goAhead();
}

void HTTP_handleRoot(void) {

  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

void commandCheck() {
  server.on("/Stoppen", []() {
    server.send(200, "text/html", webPage);
    stopRobot();
  });

  server.on("/Vooruit", []() {
    server.send(200, "text/html", webPage);
    goAhead();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });
  server.on("/Links", []() {
    server.send(200, "text/html", webPage);
    goLeft();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });
  server.on("/Rechts", []() {
    server.send(200, "text/html", webPage);
    goRight();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });
  server.on("/Achteruit", []() {
    server.send(200, "text/html", webPage);
    goBack();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });

  server.on("/Linksvoor", []() {
    server.send(200, "text/html", webPage);
    goAheadLeft();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });

  server.on("/Rechtsvoor", []() {
    server.send(200, "text/html", webPage);
    goAheadRight();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });

  server.on("/Linksachter", []() {
    server.send(200, "text/html", webPage);
    goBackLeft();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });

  server.on("/Rechtsachter", []() {
    server.send(200, "text/html", webPage);
    goBackRight();
    delay(100); //zorgt ervoor dat hij niet constant blijft rijden
    stopRobot();
  });

  server.on("/ToggleManual", []() {
    server.send(200, "text/html", webPage);
    manual = !manual;
  });
}
