// test
// Code voor het aansturen van een ESP8266-E12 via Wifi (schoolrouter Tesla IoT)
// Basis is te vinden op https://www.instructables.com/id/Esp8266-Esp12e-Webserver/#discuss
#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)


// IR SENSOREN 
#define IR_L 4     //GPIO4 (D2)
#define IR_R 5    //GPIO5 (D1)


// ESP LIBRARIES
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;
 
ESP8266WebServer server(80);
String webPage;
const char* ssid     = "Tesla IoT";      //wifi name
const char* password = "fsL6HgjN";  //wifi password

String command;             //String to store app command state.
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;

void setup() {
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
 
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  
  commandCheck(); //Controleert invoer gebruiker en voert commands uit

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
}
 void goAhead(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
      Serial.println("ROBOT START MET RIJDEN");
  }

void goBack(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);
 
      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void goBackRight(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar/speed_Coeff);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
      Serial.println("STOP ROBOT!!!");
 }

void loop() {
    server.handleClient();
    
      command = server.arg("State");  
      /* if (command == "F") goAhead();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "I") goAheadRight();
      else if (command == "G") goAheadLeft();
      else if (command == "J") goBackRight();
      else if (command == "H") goBackLeft();
      else if (command == "0") speedCar = 400;
      else if (command == "1") speedCar = 470;
      else if (command == "2") speedCar = 540;
      else if (command == "3") speedCar = 610;
      else if (command == "4") speedCar = 680;
      else if (command == "5") speedCar = 750;
      else if (command == "6") speedCar = 820;
      else if (command == "7") speedCar = 890;
      else if (command == "8") speedCar = 960;
      else if (command == "9") speedCar = 1023;
      else if (command == "S") stopRobot(); */
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

void commandCheck(){
    server.on("/Stoppen", [](){
    server.send(200, "text/html", webPage);
    stopRobot();
  });
  
    server.on("/Vooruit", [](){
    server.send(200, "text/html", webPage);
    goAhead();
  });
      server.on("/Links", [](){
    server.send(200, "text/html", webPage);
    goLeft();
  });
      server.on("/Rechts", [](){
    server.send(200, "text/html", webPage);
    goRight();
  });
      server.on("/Achteruit", [](){
    server.send(200, "text/html", webPage);
    goBack();
  });

    server.on("/Linksvoor", [](){
    server.send(200, "text/html", webPage);
    goAheadLeft();
  });
  
      server.on("/Rechtsvoor", [](){
    server.send(200, "text/html", webPage);
    goAheadRight();
  });

      server.on("/Linksachter", [](){
    server.send(200, "text/html", webPage);
    goBackLeft();
  });

      server.on("/Rechtsachter", [](){
    server.send(200, "text/html", webPage);
    goBackRight();
  });
}


// CODE To be tested IR SENSOR
void IR_left(){
  while(digitalRead(IR_L) != 1)
  {
    goAheadRight();
    Serial.println("[IR LEFT] TRUE GOING RIGHT");
    
  }
}

void IR_Right(){
  while(digitalRead(IR_R) != 1)
  {
    goAheadLeft();
    Serial.println("[IR RIGHT] TRUE GOING LEFT");
    
  }
}
