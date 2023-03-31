#include<stdio.h>
#include<stdint.h>

uint8_t pin_Button_Start = 2;
uint8_t pin_Button_Stop = 3;

typedef enum{
    button_Start,
    button_Stop
}
void ReadButton(){
    if(digitalRead(pin_Button_Start) == 1){
            
    }
}
void Controller(button_Start,button_Stop){

}