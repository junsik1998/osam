#pragma once
#include"auto.h"
#include"make.h"

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
      find_shelf = find_name(shelf, num, value2);
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