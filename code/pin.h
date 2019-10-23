#pragma once
#define DHTTYPE DHT11

// output pin
int dsensor_trig_pin[2] = {2,4};
int led_r_pin[2] = {3,6}; // pwm
int led_g_pin[2] = {5,9}; // pwm
int dht_pin = 7;
int beep_pin = 8;

// input pin
int relay_pin = 10;
int dsensor_echo_pin[2] = {11,12};
int pir_pin= 13;

// bluetooth pin
int rx_pin = 14;
int tx_pin = 15;
