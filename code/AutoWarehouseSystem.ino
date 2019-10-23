#include "DHT.h"
#include <SoftwareSerial.h>
#define DHTTYPE DHT11

// output pin
int dsensor_trig_pin[2] = {2,4};
int led_r_pin[2] = {3,5}; // pwm
int led_g_pin[2] = {5,9}; // pwm
int dht_pin = 7;
int beep_pin = 8;

// input pin
int relay_pin = 10;
int dsensor_echo_pin[2] = {11,12};
int pir_pin= 13;


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
int i, temperature, limit_temperature, moving, outting, cycle;
unsigned long pre_time = 0;
unsigned long cur_time = 0;
char tempc;
char temps[100];
SHELF *shelves[2];

SoftwareSerial bluetooth(15, 14);
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


// auto working function
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
      }
   }
}

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

void commandProcessing(char *command, SHELF **shelf, int num){
  char opcode[10] = {0};  // OP CODE : INPUT - NAME TMP LEN , FIND - NAME SPACE, OUTING - ON OFF
  char value1[10] = {0};
  char value2[10] = {0};
  char temp = 0;
  int index = 0;
  int index_op = 0;
  int index_v1 = 0;
  int index_v2 = 0;

  bluetooth.write("processing \n");
  
  while(command[index] != ' '){
    if(command[index] == 0) break;
    opcode[index_op] = command[index];
    index++;
    index_op++;
  }
  index++;
  bluetooth.write("processing 1 \n");
  bluetooth.write(opcode);
  
  while(command[index] != ' '){
      if(command[index] == 0) break;
      value1[index_v1] = command[index];
      index++;
      index_v1++;
  }
  index++;
  bluetooth.write("processing 2 \n");
  bluetooth.write(value1);
  
  if(strcmp(opcode, "INPUT") == 0){
    while(command[index] != ' '){
      if(command[index] == 0) break;
      value2[index_v2] = command[index];
      index++;
      index_v2++;
    }
    if(strcmp(value1, "NAME") == 0){
      if(strcmp(value2, "shelf0") == 0) setShelfName(shelves[0]);
      if(strcmp(value2, "shelf1") == 0) setShelfName(shelves[1]);
    }
    else if(strcmp(value1, "TMP") == 0) limit_temperature = atoi(value2);
    if(strcmp(value1, "LEN") == 0){
      if(strcmp(value2, "shelf0") == 0) setShelfSize(shelves[0]);
      if(strcmp(value2, "shelf1") == 0) setShelfSize(shelves[1]);
    }
  }
  else if(strcmp(opcode, "FIND") == 0){
    if(strcmp(value1, "NAME") == 0){
      bluetooth.write("finding");
      SHELF *find_shelf = NULL;
      while(command[index] != '\n'){
        if(command[index] == 0) break;
        value2[index_v2] = command[index];
        index++;
        index_v2++;
      }
      bluetooth.write("finding1");
      //find_shelf = find_name(shelf, num);
      if(find_shelf != NULL){
        setLED(find_shelf->led, 255, 255);
        show_space(find_shelf);
      }else{
        bluetooth.write("not find");
      }
    }
    if(strcmp(value1, "SPACE") == 0){
               String maxspace = findspace(shelves,num);
             bluetooth.write("The most abundant item is ");
             btwrite_str(maxspace);
             bluetooth.write(".\n");
    }
  }
  else if(strcmp(opcode, "OUTING") == 0){
    if(strcmp(value1, "ON") == 0){
      outting = 1;
    }
    else if(strcmp(value1, "OFF") == 0){
      outting = 0;
    }
  }
  else{
    bluetooth.write("op code error");
  }
}

// basic function
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
