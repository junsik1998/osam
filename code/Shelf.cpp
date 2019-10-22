#include "Shelf.h"
#include "Pin.h"

Shelf::Shelf(int tirg_pin_, int echo_pin_, int r_pin_, int g_pin_){
    this->trig_pin = tirg_pin_;
    this->echo_pin = echo_pin_;
    this->max_d = check_distance();
    this->now_d = 0;
    this->size = 0;
    this->name = "temp name";
    this->led.makeLed(this->led.r_pin, this->led.g_pin);
};

int Shelf::check_distance(){
    int distance;
    digitalWrite(this->trig_pin,LOW);
    delayMicroseconds(2);
    digitalWrite(this->trig_pin,HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trig_pin,LOW);
    distance = pulseIn(this->echo_pin, HIGH) * 17 / 1000 / 2;
    return distance;
}

void Shelf::setName(){
    bluetooth.write("input name : ");
    this->name = bluetooth.read();
}

void Shelf::setSize(){
    bluetooth.write("input size : ");
    this->size = bluetooth.read();
}
