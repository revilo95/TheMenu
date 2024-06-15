//#include <Arduino.h>
#include <ESP_Knob.h>

#define GPIO_NUM_KNOB_PIN_A     23
#define GPIO_NUM_KNOB_PIN_B     19

ESP_Knob *knob;
int knobButtonPin = 25;
bool buttonState = false;
uint8_t btnInterval = 200;

//To choose the menu IP, SSID, PW, Universe, SaveAndRestart and back
byte menuCounter = 0;
byte menuMax = 5;

//Limited to 255 anyway
byte ipCount = 0;

//Highest DMX universe
byte uniCount = 0;
byte uniMax = 12;

byte saveCount = 0;

//Flags for 
bool menuFlag = true;
bool ipFlag = false;
bool uniFlag = false;
bool ssidFlag = false;
bool pwFlag = false;
bool saveFlag = false;

unsigned long interrupt_time = 0;
static unsigned long last_interrupt_time = 0;


//Function to read the rotary encoder
void onKnobLeftEventCallback(int count, void *usr_data)
{
    
    Serial.print("Detect left event!");
    if(menuFlag){
        menuCounter--;
        
        if(menuCounter > menuMax){
            menuCounter = menuMax;
        }
        Serial.println(menuCounter);
    }
    else if(ipFlag){
        ipCount--;
        if(ipCount < 0){
            ipCount = 255;
        }
    }
    else if(ssidFlag){
        //ssidCount--;
    }
    else if(pwFlag){
        //pwCount--;
    }
    else if(uniFlag){
        uniCount--;
        if(uniCount > uniMax){
            uniCount = uniMax;
        }
    }else if(saveFlag){
        saveCount--;
        if(saveCount > 1){
            saveCount = 1;
        }

    }

    
}

void onKnobRightEventCallback(int count,void *usr_data)
{
    Serial.print("Detect right event!");
    if(menuFlag){
        menuCounter++;
        if(menuCounter > menuMax){
            menuCounter = 0;
        }
        Serial.println(menuCounter);
    }
    else if(ipFlag){
        ipCount++;
        if(ipCount > 255){
            ipCount = 0;
        }
    }
    else if(ssidFlag){
        //ssidCount++;
    }
    else if(pwFlag){
        //pwCount++;
    }
    else if(uniFlag){
        uniCount++;
        if(uniCount > uniMax){
            uniCount = 0;
        }
    }else if(saveFlag){
        saveCount++;
        if(saveCount > 1){
            saveCount = 0;
        }
    }
}

void startKnob(){
    pinMode(knobButtonPin, INPUT_PULLUP);
    knob = new ESP_Knob(GPIO_NUM_KNOB_PIN_A, GPIO_NUM_KNOB_PIN_B);
    knob->begin();
    knob->attachLeftEventCallback(onKnobLeftEventCallback);
    knob->attachRightEventCallback(onKnobRightEventCallback);
}

void readBtn(int interval){
    interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > interval)
    {
        if (digitalRead(knobButtonPin) == LOW){
            Serial.println("Button pressed");  
            buttonState = true; 
        }
        

        last_interrupt_time = interrupt_time;
    }else{
            buttonState = false;
        }

}