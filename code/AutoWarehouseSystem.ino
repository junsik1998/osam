#include "DHT.h"
#include "Timer.h"
#include "opop.h"
#define DHTTYPE DHT11

using namespace Pin;

DHT dht(dht_pin, DHTTYPE);
unsigned long pre_time;
unsigned long cur_time;
int moving, temperature,limit_temperature,cycle;
Shelf shelves[2] = {{dsensor_trig_pin[0], dsensor_echo_pin[0], led_r_pin[0], led_g_pin[0]},
                      {dsensor_trig_pin[1], dsensor_echo_pin[1], led_r_pin[1], led_g_pin[1]}};
//SoftwareSerial bluetooth(Pin::rx_pin, Pin::tx_pin);

//extern SoftwareSerial bluetooth;

void auto_check_distance(Shelf *shelves, int num, int cycle);

void setup() {
  // put your setup code here, to run once:
  for(int i = 2; i < 10; i++){
    pinMode(i, OUTPUT);
  }
  for(int i = 10; i < 14; i++){
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
  bluetooth.begin(9600);

  pre_time = millis();
  temperature = dht.readTemperature();
  limit_temperature = 30; // default
  cycle = 30; //default
  // humidity = dht.readHumidity();
  moving = digitalRead(pir_pin);
  crime_prevention();
}

void loop() {
  // put your main code here, to run repeatedly:
   if(outting == 0) auto_check_distance(shelves, 2, cycle);
   commandProcessing::exeCommand(bluetooth.read(), shelves, 2);
}

void auto_check_distance(Shelf *shelves, int num, int cycle){
   cur_time = millis();
   if(cur_time - pre_time >= cycle){
      pre_time = cur_time;
      for(int i = 0; i < num; i++){
         shelves[i].now_d = shelves[i].check_distance();
      }
   }
}

void crime_prevention(){
   if(commandProcessing::outting == 1){
      if( moving == HIGH) bluetooth.write("crime");
      while(moving == HIGH){
         digitalWrite(beep_pin, HIGH);
         delay(500);
         digitalWrite(beep_pin, LOW);
         delay(500);
      }
   }
}
