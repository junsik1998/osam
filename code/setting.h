#pragma once
#include"struct_var.h"

// setting function
void setLED(LED *led, int r, int g){
   led->r = r;
   led->g = g;
   analogWrite(led->r_pin, led->r);
   analogWrite(led->g_pin, led->g);
}

void setShelfName(SHELF *shelf){
  bluetooth.write("input name : ");
  temps[0] = 0;
  while(temps[0] == 0){
    inputString_temps();
  }
  if(temps[0] != 0) strcpy(shelf->name, temps);
  sprintf(temps, "name = %s", shelf->name);
  bluetooth.write(temps);
  temps[0] = 0;
}

void setShelfSize(SHELF *shelf){
   bluetooth.write("input size : ");
   temps[0] = 0;
    while(temps[0] == 0){
      inputString_temps();
    }
   shelf->size = atoi(temps);
    sprintf(temps, "size = %d", shelf->size);
    bluetooth.write(temps);
    temps[0] = 0;
}

/*
void setLimitTemperature(){
  sprintf(temps, "before limit_temperature = %d", limit_temperature);
  bluetooth.write(temps);
  bluetooth.write("input limit temperature : ");
  while(temps[0] == 0){
     inputString_temps();
   }
  limit_temperature = atoi(temps);
  sprintf(temps, "after limit_temperature = %d", limit_temperature);
  bluetooth.write(temps);
  temps[0] = 0;
}
*/

void setCycle(){
  sprintf(temps, "before cycle = %d", cycle);
  bluetooth.write(temps);
  bluetooth.write("input check distance cycle : ");
  while(temps[0] == 0){
    inputString_temps();
  }
  cycle = atoi(temps);
  sprintf(temps, "after cycle = %d", cycle);
  bluetooth.write(temps);
  temps[0] = 0;
}