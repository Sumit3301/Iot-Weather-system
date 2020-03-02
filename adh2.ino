//#include <WiFi101.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 
#include <ESP8266WiFi.h>
#include "DHT.h"  
// WiFi parameters 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "wifi"
#define WLAN_PASS       "12345677"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "YOUR USERNAME"
#define AIO_KEY         "YOUR AIO KEY"

#define DHTPIN D1    //PIN WHICH WE CONNECTED TO THE DHT11 
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE); 
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); 
/****************************** Feeds ***************************************/ 
// Setup feed for temperature 
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/Temprature"); 
Adafruit_MQTT_Publish tempr = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/Temprature_Variation");
Adafruit_MQTT_Publish humi = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/Humidity"); 
Adafruit_MQTT_Publish humi_var = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/Humidity_Variation"); 


static int temp = 0; 
static int tempPrev = 0; 
void setup() { 
 Serial.begin(9600); 
 delay(3000); 
 //Connect to WiFi & Adafruit.IO 
 connectToWiFi(); 
 connectToAdafruit(); 
 //connect to DHT11 Temp Sensor 
 dht.begin(); 
  
} 
void loop() { 
   // ping adafruit io a few times to make sure we remain connected 
 if(! mqtt.ping(3)) { 
   // reconnect to adafruit io 
   if(! mqtt.connected()) 
     connect(); 
 } 
   // Grab the current state of the sensor 
   temp = dht.readTemperature(true); 
   temp=((temp-32)*0.55555555555555555555555555555556);
  float hu=dht.readHumidity();
   char c[4];
   String u;
   u=String(hu);
   for(int j=0;j<u.length();j++)
   {
   c[j]=u.charAt(j);
   }
   c[(u.length())+1]=0;
 //int to string for humidity  
   
   char b[4]; 
   String str; 
   str=String(temp); 
   for(int i=0; i<str.length(); i++) 
   { 
     b[i]=str.charAt(i); 
   } 
   b[(str.length())+1]=0;  //convert int temp to char array 
 // Publish data 
  
 if (!temperature.publish((char*)b)) { 
   Serial.println(F("Failed to publish temp")); 
 } else { 
   Serial.print(F("Temp published: ")); 
   Serial.println(temp);  
 } 
  if (!tempr.publish((char*)b)) { 
   Serial.println(F("Failed to publish temp")); 
 } else { 
   Serial.print(F("Temp published: ")); 
   Serial.println(temp); 

 } 
 if (!humi.publish((char*)c)) { 
   Serial.println(F("Failed to publish Humidity")); 
 } else { 
   Serial.print(F("Humidity published: ")); 
   Serial.println(hu); }
 
 if (!humi_var.publish((char*)c)) { 
   Serial.println(F("Failed to publish Humidity")); 
 } else { 
   Serial.print(F("Humidity published: ")); 
   Serial.println(hu); 
 
 } 

 // Check if any reads failed and exit early (to try again). 
 if (isnan(temp)) { 
   Serial.println("Failed to read from DHT sensor!"); 
   return; 
 } 
 
 delay(9000); 
} 
// connect to adafruit io via MQTT 
void connect() { 
 Serial.print(F("Connecting to Adafruit IO... ")); 
 int8_t ret; 
 while ((ret = mqtt.connect()) != 0) { 
   switch (ret) { 
     case 1: Serial.println(F("Wrong protocol")); break; 
     case 2: Serial.println(F("ID rejected")); break; 
     case 3: Serial.println(F("Server unavail")); break; 
     case 4: Serial.println(F("Bad user/pass")); break; 
     case 5: Serial.println(F("Not authed")); break; 
     case 6: Serial.println(F("Failed to subscribe")); break; 
     default: Serial.println(F("Connection failed")); break; 
   } 
   if(ret >= 0) 
     mqtt.disconnect(); 
   Serial.println(F("Retrying connection...")); 
   delay(5000); 
 } 
 Serial.println(F("Adafruit IO Connected!")); 
} 

void connectToWiFi() 
{ 
 // Connect to WiFi access point. 
 delay(10); 
 Serial.print(F("Connecting to ")); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print(F(".")); 
 } 
 Serial.println(F("WiFi connected!")); 
} 
void connectToAdafruit() 
{ 
   // connect to adafruit io 
 connect(); 
} 
