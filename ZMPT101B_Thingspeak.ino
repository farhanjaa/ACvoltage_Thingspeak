#include <ESP8266WiFi.h>
#include "Wire.h"

String apiKey = "G7SDRY7P2ZP1T8MP";     //  Enter your Write API key from ThingSpeak
const char* resource = "/update?api_key=";

const char *ssid =  "IPAN";     // replace with your wifi ssid and wpa2 key
const char *pass =  "Inpres11"; //password your SSID
const char* server = "api.thingspeak.com";

int vin = 0;
int iin = 0;
int vout;
float kal;

unsigned long start_times[100];
unsigned long stop_times[100];
unsigned long values[100];

// Define various ADC prescaler
//const unsigned char PS_16 = (1 << ADPS2);
//const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
//const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
//const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

int a = 0;
int zero = 1;

WiFiClient client;

void setup() 
{
       Serial.begin(9600);
       delay(10);

       Serial.println("Connecting to ");
       Serial.println(ssid);

// Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");

 // set up the ADC
//  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
//  ADCSRA |= PS_128;    // set our own prescaler to 64
      
}



void loop() 
{
  
  unsigned int i1;
  unsigned int z1;
  z1 = 0;

  unsigned int i2;
  unsigned int z2;
  z2 = 0;

  unsigned int i3;
  unsigned int z3;
  z3 = 0;

  unsigned int i4;
  unsigned int z4;
  z4 = 0;
  
  for(i1=0;i1<100;i1++) {
    start_times[i1] = micros();
    values[i1] = analogRead(A0);            

  if (values[i1] >= z1) {
  z1 = values[i1];
  }

    stop_times[i1] = micros();
  }

    for(i2=0;i2<100;i2++) {
    start_times[i2] = micros();
    values[i2] = analogRead(A0);            

  if (values[i2] >= z2) {
  z2 = values[i2];
  }

    stop_times[i2] = micros();
  }


    for(i3=0;i3<100;i3++) {
    start_times[i3] = micros();
    values[i3] = analogRead(A0);            

  if (values[i3] >= z3) {
  z3 = values[i3];
  }

    stop_times[i3] = micros();
  }


    for(i4=0;i4<100;i4++) {
    start_times[i4] = micros();
    values[i4] = analogRead(A0);            

  if (values[i4] >= z4) {
  z4 = values[i4];
  }

    stop_times[i4] = micros();
  }


   int vin1 = z1 ;
   int vin2 = z2 ;
   int vin3 = z3 ;
   int vin4 = z4 ;

  z1 = 0;
  z2 = 0;
  z3 = 0;
  z4 = 0;

if((vin1 > vin2)&&(vin1 > vin3)&&(vin1 > vin4)){
  vout = vin1;
  kal = (vout - 915.9) / 0.413 ;
}
else if((vin2 > vin1)&&(vin2 > vin3)&&(vin2 > vin4)){
  vout = vin2;
  kal = (vout - 915.9) / 0.413 ;
}
else if((vin3 > vin1)&&(vin3 > vin2)&&(vin3 > vin4)){
  vout = vin3;
  kal = (vout - 915.9) / 0.413 ;
}
else if((vin4 > vin1)&&(vin4 > vin2)&&(vin4 > vin3)){
  vout = vin4;
  kal = (vout - 915.9) / 0.413 ;
}

kal = kal - 20;

if(kal < 0){
kal = 0;
}

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(kal);
                             postStr += "\r\n\r\n";

                            client.print(String("GET ") + resource + apiKey + "&field1=" + kal + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n");                             
                                        
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
                             
                             Serial.print("voltage: ");
                             Serial.print(kal);
                             Serial.println("v. Send to Thingspeak.");
                        }
                        
          client.stop();

          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(10000);
}
