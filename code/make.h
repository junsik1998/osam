#pragma once
#include"setting.h"
#include"auto.h"

// object make function
LED* makeLED(int r_pin, int g_pin, int r, int g){
   LED *led = (LED*)malloc(sizeof(LED));
   led->r_pin = r_pin;
   led->g_pin = g_pin;
   led->r = r;
   led->g = g;
   return led;
}

SHELF* makeShelf(int trig_pin, int echo_pin, int r_pin, int g_pin){
   SHELF *shelf = (SHELF*)malloc(sizeof(SHELF));
   LED *led = makeLED(r_pin, g_pin, 0 ,0);
   // 
   shelf->max_d = check_distance(trig_pin, echo_pin);
   shelf->now_d = 0;
   shelf->size = 0;
   strcpy(shelf->name, "tempname");
   shelf->led = led;
   shelf->trig_pin = trig_pin;
   shelf->echo_pin = echo_pin;
   return shelf;
}