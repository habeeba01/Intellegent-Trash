#define BLYNK_TEMPLATE_ID "TMPLIJCctTtm"
#define BLYNK_Device_NAME "Smart Trash IOT"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Servo_ESP32.h>

WidgetLED led(V1);

// authentification
char auth[] = "BPkb9dozm1yqYi2IOOWecKJA_egAlBm0";
char ssid[] = "Infinix HOT 10S";
char pass[] = "mimo2002";

BlynkTimer timer;

Servo_ESP32 servo1;
int trigPin1 =32;
int echoPin1 = 33;
long distance1;
long duration1;
#define TRIG_PIN  26 
#define ECHO_PIN  25 
#define DISTANCE_THRESHOLD 10 // centimeters

float duration, distance;
void setup() 
{
  Blynk.begin(auth, ssid, pass);
  servo1.attach(14); 
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  Serial.begin (9600);       // initialize serial port
  pinMode(TRIG_PIN, OUTPUT); // set ESP32 pin to output mode
  pinMode(ECHO_PIN, INPUT);  // set ESP32 pin to input mode
  pinMode(LED_PIN, OUTPUT);  // set ESP32 pin to output mode

   timer.setInterval(1000L, ultra);
}
 
void loop() {
  ultra();
  Blynk.run();
  timer.run(); 
  servo1.write(0);
  if((distance1 < 8) && !detecter() ){
  servo1.write(90);
  delay(2500);
  }else if((distance1 < 8) && detecter()){
    Serial.println("objet detecte");
    servo1.write(0);
  }else if(detecter()){
     Serial.println("objet detecte");
      led.on();
     Blynk.logEvent("notification","Full Trash");
  }else{
    led.off();
  }

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}
 
void ultra(){
  //Detection  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1*0.034/2;

  //Remplissage
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.034/2;
Blynk.virtualWrite(V0, distance);  
  
}
bool detecter(){
  if(distance < DISTANCE_THRESHOLD){
     return pdTRUE;
  }
  else
     return pdFALSE;
}