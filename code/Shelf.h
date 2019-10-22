#include <Arduino.h>
#include <SoftwareSerial.h>

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
    int now_d;
    int size;
    String name;
    Led led;
    int trig_pin;
    int echo_pin;
    int check_distance();
public:
    void setName();
    void setSize();
    friend void auto_check_distance(Shelf* shelves ,int num, int cycle);
    Shelf(int tirg_pin_, int echo_pin_, int r_pin_, int g_pin_);
    Shelf(){};
};
