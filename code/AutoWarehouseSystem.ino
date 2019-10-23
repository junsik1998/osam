#include"command.h"

void setup(){
   for(i = 2; i < 10; i++){
      pinMode(i, OUTPUT);
   }
   for(i = 10; i < 14; i++){
      pinMode(i, INPUT);
   }
   Serial.begin(9600);
   bluetooth.begin(9600);
   pre_time = millis();
   limit_temperature = 30; // default
   cycle = 300; // default
   outting = 0;

   SHELF* shelf1 = makeShelf(dsensor_trig_pin[0], dsensor_echo_pin[0], led_r_pin[0], led_g_pin[0]);
   SHELF* shelf2 = makeShelf(dsensor_trig_pin[1], dsensor_echo_pin[1], led_r_pin[1], led_g_pin[1]);
   shelves[0] = shelf1;
  shelves[1] = shelf2;
}

void loop()
{
  temperature = dht.readTemperature();
  moving = digitalRead(pir_pin);
  crime_prevention();
  if(outting == 0) auto_check_distance(shelves, 2, cycle);
  
  inputString_temps();
  if(temps[0] != 0) commandProcessing(temps, shelves, 2);
  
}