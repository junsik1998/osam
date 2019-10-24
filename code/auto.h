#include"struct_var.h"

// auto led control
void setLED(LED *led, int r, int g) {
	led->r = r;
	led->g = g;
	analogWrite(led->r_pin, led->r);
	analogWrite(led->g_pin, led->g);
}

void lightControl(SHELF *shelf) {
	ratio = (double)shelf->now_d / shelf->max_d;
	if(ratio > 1) ratio = 1;
	if(ratio < 0) ratio = 0;
	int glightState = ratio * 255;
	int rlightState = (1-ratio) * 255;
	setLED(shelf->led, rlightState, glightState);
}


// auto check
int check_distance(int trig_pin, int echo_pin) {
	int distance;
	digitalWrite(trig_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(trig_pin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig_pin, LOW);
	distance = pulseIn(echo_pin, HIGH) * 17.0 / 1000.0;
	return distance;
}

void auto_check_distance(SHELF **shelves, int num, int cycle) {
  int before_d, now_d;
	cur_time = millis();
	if(cur_time - pre_time >= cycle) {
		pre_time = cur_time;
		for (i = 0; i < num; i++) {
    		before_d = shelves[i]->now_d;
    		now_d = check_distance(shelves[i]->trig_pin,shelves[i]->echo_pin);
			shelves[i]->now_d = now_d;
			lightControl(shelves[i]);
    		if(before_d - now_d > 2){
    			if(shelves[i]->size > 0){
    				sprintf(temps, "%s %d개가 입고되었습니다. \n", shelves[i]->name, (int)(((double)(before_d - now_d) / shelves[i]->size) + 0.5));
    				bluetooth.write(temps);
    			}
    		}
    		if(now_d - before_d > 2){
    			if(shelves[i]->size > 0){
    				sprintf(temps, "%s %d개가 출고되었습니다. \n", shelves[i]->name, (int)(((double)(now_d - before_d) / shelves[i]->size) + 0.5));
    				bluetooth.write(temps);
    			}
    		}
		}
	}
}


//auto working
void auto_temperature(int limit) {
	temperature = dht.readTemperature();
	if(temperature > limit) {
		digitalWrite(relay_pin, HIGH);
	} else {
		digitalWrite(relay_pin, LOW);
	}
}

SHELF* find_name(SHELF **shelves, int num, char *name) {
	SHELF *find_shelf = NULL;
	for (i = 0; i < num; i++) {
		if(strcmp(shelves[i]->name, name) == 0) {
			find_shelf = shelves[i];
			break;
		}
	}
	return find_shelf;
}

void show_info(SHELF *shelf) {
	int max_d = shelf->max_d;
	int now = shelf->now_d;
	int amount = max_d - now;
	int per = (double)amount / max_d * 100;
	int count = 0;
	if(shelf->size > 0) count = (int)(((double) amount / shelf->size) + 0.5);
	sprintf(temps, "name = %s \n", shelf->name);
	bluetooth.write(temps);
	sprintf(temps, "max = %d cm \n", max_d);
	bluetooth.write(temps);
	sprintf(temps, "currnet amount = %d cm, %d %% ", amount, per);
	bluetooth.write(temps);
	if(count > 0) {
		if(count == 1) {
			sprintf(temps, "There is 1 %s on this shelf", shelf->name);
			bluetooth.write(temps);
		} else {
			sprintf(temps, "There are %d %s on this shelf", count, shelf->name);
			bluetooth.write(temps);
		}
	}
}

void findspace(Shelf **shelves, int num) {
	int max = 0;
	int max_i = 0;
	Shelf *max_shelf = NULL;
	for(int i = 0; i < num; i++){
		if(shelves[i]->now_d > max){
			max = shelves[i]->now_d;
			max_i = i;
		}
	}
	max_shelf = shelves[max_i];
	sprintf(temps, "가장 많은 빈 공간이 있는 곳은 %s 이(가) 보관된 칸으로\n", max_shelf->name);
	bluetooth.write(temps);
	sprintf(temps, "%d cm만큼 더 보관 가능합니다.\n", max);
	bluetooth.write(temps);
	for(i = 0; i < 3; i++){
		setLED(max_shelf->led, 255, 0);
		delay(300);
		setLED(max_shelf->led, 0, 255);
		delay(300);
	}
}

void crime_prevention() {
	if(outting == 1) {
		if(moving == HIGH) bluetooth.write("crime");
		while(moving == HIGH) {
			moving = digitalRead(pir_pin);
			digitalWrite(beep_pin, HIGH);
			delay(50);
			digitalWrite(beep_pin, LOW);
			delay(50);
		}
	}
}
