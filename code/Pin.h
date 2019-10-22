#include <SoftwareSerial.h>

namespace Pin{
  // output pin
  int dsensor_trig_pin[2] = {2,4};
  int led_r_pin[2] = {3,5};
  int led_g_pin[2] = {5,9};
  int dht_pin = 7;
  int beep_pin = 8;

    // input pin
    int relay_pin = 10;
    int dsensor_echo_pin[2] = {11,12};
    int pir_pin= 13;
    // analog pin to digital out pin
    int tx_pin = 14; // A0 pin = 14
    int rx_pin = 15; // A1 pin = 15
}

SoftwareSerial bluetooth(Pin::rx_pin, Pin::tx_pin);
