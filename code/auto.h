#pragma once
#include"setting.h"

// auto led control
void lightControl(SHELF *shelf){
    double shelfState = shelf->now_d / shelf->max_d;
    int glightState = shelfState * 255;
    int rlightState = (1-shelfState) * 255;
    setLED(shelf->led, rlightState, glightState);
}

// auto check
int check_distance(int trig_pin, int echo_pin){
   int distance;
   digitalWrite(trig_pin, LOW);
   delayMicroseconds(2);
   digitalWrite(trig_pin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trig_pin, LOW);
   distance = pulseIn(echo_pin, HIGH) * 17 / 1000 /2;
   return distance;
}

void auto_check_distance(SHELF **shelves, int num, int cycle){
   cur_time = millis();
   if(cur_time - pre_time >= cycle){
      pre_time = cur_time;
      for(i = 0; i < num; i++){
        shelves[i]->now_d = check_distance(shelves[i]->trig_pin,shelves[i]->echo_pin);
        lightControl(shelves[i]);
      }
   }
}

//auto working
void auto_temperature(int limit){
   temperature = dht.readTemperature();
   if(temperature > limit){
      digitalWrite(relay_pin, HIGH);
   }
   else{
      digitalWrite(relay_pin, LOW);
   }
}

SHELF* find_name(SHELF **shelves, int num, char *name){
   SHELF *find_shelf = NULL;
   for(i = 0; i < num; i++){
      if(strcmp(shelves[i]->name, name) == 0){
         find_shelf = shelves[i];
         break;
      }
   }
   return find_shelf;
}

void show_space(SHELF *shelf){
   int max = shelf->max_d;
   int now = shelf->now_d;
   int amount = max - now;
   int per = amount / max * 100;
   int count = 0;
   if(shelf->size > 0) count = amount / shelf->size;
   sprintf(temps, "max = %d cm \n", max);
   bluetooth.write(temps);
   sprintf(temps, "currnet amount = %d cm, %d %% ", amount, per);
   bluetooth.write(temps);
   if(count > 0){
        if(count == 1){
          sprintf(temps, "There is 1 %s on this shelf",shelf->name);
          bluetooth.write(temps);
        }
        else{
          sprintf(temps, "There are %d %s on this shelf", count, shelf->name);
          bluetooth.write(temps);
        }
   }
}

String findspace(Shelf **shelves, int num){
    int max = 0;
    String shelfName;
    for(int i=0;i<num;++i)
        if (shelves[i]->now_d>max){
            max = shelves[i]->now_d;
            shelfName = shelves[i]->name;
        }
    return shelfName;
}
void btwrite_str(const String& str){
  for(int i=0;i<str.length();++i)
    bluetooth.write(str[i]);
}
void crime_prevention(){
   if(outting == 1){
      if(moving == HIGH) bluetooth.write("crime");
      while(moving == HIGH){
         digitalWrite(beep_pin, HIGH);
         delay(500);
         digitalWrite(beep_pin, LOW);
         delay(500);
      }
   }
}

