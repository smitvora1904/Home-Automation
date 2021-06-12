#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/********* Pin Definition ***********/
//Relays for switching appliances
#define Relay1            D3
#define Relay2            D4
#define Relay3            D5
#define Relay4            D7
/********* WiFi Access Point ***********/

#define WLAN_SSID       "ayush"
#define WLAN_PASS       "ayush1111"

/********* Adafruit.io Setup ***********/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "yashpatel"//"techiesms"
#define AIO_KEY         "b2f3d3d9e6304aa2994f8125aa36ad55"//"912b30c900574034a653f41e2b4df838"

/**** Global State (you don't need to change this!) ******/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/********** Feeds *************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Welcome_feed");


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Onfeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Onfeed");
Adafruit_MQTT_Subscribe Offfeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Offfeed");

Adafruit_MQTT_Subscribe Onfeed2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Onfeed2");
Adafruit_MQTT_Subscribe Offfeed2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Offfeed2");

Adafruit_MQTT_Subscribe Onfeed3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Onfeed3");
Adafruit_MQTT_Subscribe Offfed3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Offfed3");

Adafruit_MQTT_Subscribe Onfed4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Onfed4");
Adafruit_MQTT_Subscribe Offfed4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Offfed4");
uint32_t delayMS;


/********* Sketch Code ************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
//Servo myservo;
void setup() {
  Serial.begin(9600);

  delay(10);

  
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&Onfeed);
  mqtt.subscribe(&Offfeed);
 
  mqtt.subscribe(&Onfeed2);
  mqtt.subscribe(&Offfeed2);
  
  mqtt.subscribe(&Onfeed3);
  mqtt.subscribe(&Offfed3);
  
  mqtt.subscribe(&Onfed4);
  mqtt.subscribe(&Offfed4);
}

uint32_t x = 0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
 MQTT_connect();
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Onfeed) {
      Serial.print(F("Got: "));
      Serial.println((char *)Onfeed.lastread);
      if(Onfeed.lastread[0] == 'L')digitalWrite(Relay1,LOW);
      
    }
    if (subscription == &Offfeed) {
      Serial.print(F("Got: "));
      Serial.println((char *)Offfeed.lastread);
      if(Offfeed.lastread[0] == 'L')digitalWrite(Relay1,HIGH);
      
    }
    if (subscription == &Onfeed2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Onfeed2.lastread);
      if(Onfeed2.lastread[0] == 'R')digitalWrite(Relay2,LOW);
      
    }
    if (subscription == &Offfeed2) {
      Serial.print(F("Got: "));
      Serial.println((char *)Offfeed2.lastread);
      if(Offfeed2.lastread[0] == 'R')digitalWrite(Relay2,HIGH);
      
    }
 
       if (subscription == &Onfeed3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Onfeed3.lastread);
      if(Onfeed3.lastread[0] == 'U')digitalWrite(Relay3,LOW);
      
    }
    if (subscription == &Offfed3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Offfed3.lastread);
      if(Offfed3.lastread[0] == 'U')digitalWrite(Relay3,HIGH);
      
    }  
 
       if (subscription == &Onfed4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Onfed4.lastread);
      if(Onfed4.lastread[0] == 'D')digitalWrite(Relay4,LOW);
      
    }
    if (subscription == &Offfed4) {
      Serial.print(F("Got: "));
      Serial.println((char *)Offfed4.lastread);
      if(Offfed4.lastread[0] == 'D')digitalWrite(Relay4,HIGH);
      
    }
  }
  

   Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! photocell.publish(x++)) {
     Serial.println(F("Failed"));
      delay(1000);
   } else {
      Serial.println(F("OK!"));
     delay(1000);
   }
}
  void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
  }
