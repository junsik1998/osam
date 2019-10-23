#pragma once
#include <SoftwareSerial.h>
#include"pin.h"
#include "DHT.h"

// struct
typedef struct LED_{
   int r_pin;
   int g_pin;
   int r;
   int g;
} LED;

typedef struct Shelf{
   // shelf info
   int max_d; // 선반 길이
   int now_d; // 현재 물건 길이
   int size; // 물건 1개당 길이, 지정 안하면 기본값 0
   char name[100]; // 물건 이름
   LED *led;
   // pin info
   int trig_pin;
   int echo_pin;
} SHELF;


// variable
int i, limit_temperature, moving, outting, cycle;
int temperature;
double ratio;
unsigned long pre_time = 0;
unsigned long cur_time = 0;
char tempc;
char temps[100];
SHELF *shelves[2];

SoftwareSerial bluetooth(rx_pin, tx_pin);
DHT dht(dht_pin, DHTTYPE);

void inputString_temps(){
  for(i = 0; i < 100; i++){
    temps[i] = 0;
  }
  i = 0;
  while(bluetooth.available()){
     tempc = bluetooth.read();
     temps[i++] = tempc;
     delay(10);
   }
  if(temps[0] != 0) bluetooth.write(temps);
  if(temps[0] != 0) bluetooth.write("_____\n");
}
