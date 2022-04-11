#include <ESP8266WiFi.h>
 
String apiKey = "A3RCMLSO3S5MUETS";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "Shakil";     //  wifi ssid 
const char *pass =  "01923002461";    // wifi password
const char* server = "api.thingspeak.com";

WiFiClient client;
 

#define echoPin D0
#define trigPin D1
#define greenPin D2
#define yelloPin D3
#define redPin D4

long duration;
int distance;



void setup() {
  
    Serial.begin(9600); 
    delay(10); 
    pinMode(trigPin,OUTPUT);
    pinMode(echoPin,INPUT);
    pinMode(greenPin,OUTPUT);
    pinMode(yelloPin,OUTPUT);
    pinMode(redPin,OUTPUT);

    digitalWrite(greenPin,LOW);
    digitalWrite(yelloPin,LOW);
    digitalWrite(redPin,LOW);
    

    Serial.print("Connecting to: ");
    Serial.println(ssid);

      WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}


int distanceMeasure(){

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delay(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  distance = duration *0.034 / 2;


  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println("cm");
  

  return distance;
  
  }


void labelControl(){


  int label = distance;

  if (label >=5){
    digitalWrite(greenPin,HIGH);
    
  }
  else{
    digitalWrite(greenPin,LOW);
  }

  if (label >=3 && label <5){
    digitalWrite(yelloPin,HIGH);
    
  }
  else{
    digitalWrite(yelloPin,LOW);
  }

  if(label < 3){
    digitalWrite(redPin,HIGH);
  }
  else{
    digitalWrite(redPin,LOW);
  }
}
void loop() {

  

     distance = distanceMeasure();
     labelControl();

    if (isnan(distance)) 
                 {
                     Serial.println("Failed to read from Ultrasonic sensor!");
                      return;
                 }

                         if (client.connect(server,80))   // establishing the connection with thingspeak server
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(distance);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
//                             Serial.print("distance: ");
//                             Serial.print(distance);
                            
                             Serial.println(" Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  
  delay(500);  // dealy half second and regenerate the temperature and send it to the thingspeak


}
