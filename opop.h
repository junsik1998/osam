#include "Shelf.h"

namespace commandProcessing{
    char opcode[10];
    char value1[10], value2[10];
    char temp;
    int index;
    int index_op;
    int index_v[2];
    int outting = 0;
    //friend crime_prevention();
    void reset();
    void opop(const char *command, Shelf *shelves, const int num);
    void opinput(const char *command,Shelf *shelves);
    void opfind(const char *command, Shelf *shelves, const int num);
    void opoutting(const char *opcode);
    void exeCommand(const char *command, Shelf *shelves, const int num);
};

extern int limit_temperature;

using namespace commandProcessing;
    void commandProcessing::reset(){
        for(int i = 0; i<10;++i)
            opcode[i] = 0;
        index = 0;
        index_op = 0;
        index_v[0] = 0;
        index_v[1] = 0;
    }
    void commandProcessing::opop(const char *command, Shelf *shelves, const int num){
        if(strcmp(opcode,"INPUT") == 0)
            opinput(command, shelves);
        else if(strcmp(opcode, "FIND") == 0)
            opfind(command, shelves, num);
        else if(strcmp(opcode, "OUTTING") == 0)
            opoutting(opcode);
        else
            bluetooth.write("op code error");
    }
    void commandProcessing::opinput(const char *command,Shelf *shelves){
        while(command[index] != ' '){
            value2[index_v[1]] = command[index];
            index++;
            index_v[1]++;
        }
        if(strcmp(value1, "NAME") == 0){
            if(strcmp(value2, "shelf1") == 0) shelves[0].setName();
            if(strcmp(value2, "shelf2") == 0) shelves[1].setName();
        }
        else if(strcmp(value1, "TMP") == 0) limit_temperature = atoi(value1);
        if(strcmp(value1, "LEN") == 0){
            if(strcmp(value2, "shelf1") == 0) shelves[0].setName();
            if(strcmp(value2, "shelf2") == 0) shelves[1].setName();
        }
    }
    void commandProcessing::opfind(const char *command, Shelf *shelves, const int num){
        if(strcmp(value1, "NAME") == 0){
            Shelf *found_shelf = nullptr;
            while(command[index] != ' '){
                value2[index_v[1]] = command[index];
                index++;
                index_v[1]++;
            }
            found_shelf = find_item(shelves, num, value2);//?????
            if(found_shelf != nullptr){
                found_shelf->led.setLed(255, 255);
                found_shelf->show_space();
            }else{
                bluetooth.write("not find");
            }
        } 
        if(strcmp(value1, "SPACE") == 0){
             String abundantItem = find_abundant_item(shelves,num);
             bluetooth.write("The most abundant item is ");
             btwrite_str(abundantItem);
             bluetooth.write(".\n");
        }
    }
    void commandProcessing::opoutting(const char *opcode){
        if(strcmp(value1, "ON") == 0)
            outting = 1;
        else if(strcmp(value1, "OFF") == 0)
            outting = 0;
    }
    void commandProcessing::exeCommand(const char *command, Shelf *shelves, const int num){
        reset();
        while(command[index] != ' '){
            opcode[index_op] = command[index];
            index++;
            index_op++;
        }
        while(command[index] != ' '){
            value1[index_v[0]] = command[index];
            index++;
            index_v[0]++;
        }
        opop(command, shelves, num);
    }
