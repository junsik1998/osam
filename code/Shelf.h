#include <Arduino.h>
#include "Pin.h"

struct Led{
    int r_pin;
    int g_pin;
    int r;
    int g;
    Led(int r_pin_, int g_pin_, int r_ = 0 , int g_ = 0){
        r_pin = r_pin_;
        g_pin = g_pin_;
        r = r_;
        g = g_;
    }
    Led(){};
    void makeLed(int r_pin_, int g_pin_, int r_ = 0 , int g_ = 0){
        r_pin = r_pin_;
        g_pin = g_pin_;
        r = r_;
        g = g_;
    }
    void setLed(int r_, int g_){
        r = r_;
        g = g_;
        analogWrite(r_pin,r);
        analogWrite(g_pin,g);
    }
};

class Shelf{
    int max_d;
    int size; 
    int check_distance();
public:
    Led led;
    String name;
    int now_d;
    int trig_pin;
    int echo_pin;
    void setName();
    void setSize();
    void show_space();
    friend String find_abundant_item(const Shelf *shelves, const int num){
    int min = 99999;
    String shelfName;
    for(int i=0;i<num;++i)
        if (shelves[i].now_d<min){
            min = shelves[i].now_d;
            shelfName = shelves[i].name;
        }
    return shelfName;
}
    friend Shelf* find_item(const Shelf *shelves, const int num, const String name){
    Shelf *found_shelf = nullptr;
    for(int i = 0; i < num; i++){
      if(shelves[i].name.compareTo(name) == 0){
         found_shelf = &shelves[i];
         break;
      }
   }
   return found_shelf;
}
    friend void auto_check_distance(Shelf* shelves ,int num, int cycle);
    Shelf(int tirg_pin_, int echo_pin_, int r_pin_, int g_pin_);
    Shelf(){};
};

void btwrite_str(const String& str){
  for(int i=0;i<str.length();++i)
    bluetooth.write(str[i]);
}
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
/*
Shelf* find_item(const Shelf *shelves, const int num, const String name){
    Shelf *found_shelf = nullptr;
    for(int i = 0; i < num; i++){
      if(shelves[i].name.compareTo(name) == 0){
         found_shelf = &shelves[i];
         break;
      }
   }
   return found_shelf;
}*/

void Shelf::show_space(){
   int max = max_d;
   int now = now_d;
   int amount = max - now;
   int per = amount / max * 100;
   int count = 0;
   if(size > 0) count = amount / size;
   bluetooth.write("max = %d cm ", max);
   bluetooth.write("currnet amount = %d cm ", amount);
   bluetooth.write("%d \%\%\n", per);
  // bluetooth.write("currnet amount = %d cm, %d  \%\%", amount, per);
   if(count > 0){
      if(count == 1){
        bluetooth.write("There is 1 ");
        btwrite_str(name);
        bluetooth.write(" on this shelf");
       // bluetooth.write("There is 1 %s on this shelf", name);
      }
      else{
        bluetooth.write("There are %d ", count);
        btwrite_str(name);
        bluetooth.write(" on this shelf");
      }
   }
}
/*
String find_abundant_item(const Shelf *shelves, const int num){
    int min = 99999;
    String shelfName;
    for(int i=0;i<num;++i)
        if (shelves[i].now_d<min){
            min = shelves[i].now_d;
            shelfName = shelves[i].name;
        }
    return shelfName;
}*/
