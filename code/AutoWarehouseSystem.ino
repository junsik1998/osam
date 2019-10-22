#include "DHT.h"
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
	String name; // 물건 이름
	LED led;
	// pin info
	int trig_pin;
	int echo_pin;
} SHELF;


// variable
int i, temperature, limit_temperature, moving, outting, cycle;
unsigned long pre_time = 0;
unsigned long cur_time = 0;
SHELF *shelves[2];

SoftwareSerial bluetooth(2, 3);
DHT dht(DHTPIN, DHTTYPE);

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
	shelf->now_d = 0
	shelf->size = 0;
	shelf->name = "temp name";
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
	shelf->name = bluetooth.read();
}

void setShelfSize(SHELF *shelf){
	bluetooth.write("input size : ");
	shelf->size = bluetooth.read();
}

void setLimitTemperature(){
	bluetooth.write("input limit temperature : ");
	limit_temperature = bluetooth.read();
}

void setCycle(){
	bluetooth.write("input check distance cycle : ");
	cycle = bluetooth.read();
}


// auto working function
int check_distance(int trig_pin, int echo_pin){
	int distance;
	digitalWrite(trig_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(trig_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig_pin, LOW);
	distance = pulseIn(echoPin, HIGH) * 17 / 1000 /2;
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
	bluetooth.write("max = %d cm ", max);
	bluetooth.write("currnet amount = %d cm, %d %% ", amount, per);
	if(count > 0){
		if(count == 1) bluetooth.write("There is 1 %s on this shelf", shelf->name);
		else bluetooth.write("There are %d %s on this shelf", count, shelf->name);
	}
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
	cycle = 300 // default
	outting = 0;

	SHELF* shelf1 = makeShelf(dsensor_trig_pin[0], dsensor_echo_pin[0], led_r_pin[0], led_g_pin[0]);
	SHELF* shelf2 = makeShelf(dsensor_trig_pin[1], dsensor_echo_pin[1], led_r_pin[1], led2_g_pin[1]);
	shelves[2] = [shelf1, shelf2];

}

void loop()
{
	temperature = dht.readTemperature();
	moving = digitalRead(pir_pin);
	crime_prevention();
	if(outing == 0) auto_check_distance(shelves, 2, cycle);
}`