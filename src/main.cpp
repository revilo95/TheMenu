#include <Arduino.h>
#include <menu.h>

int var = 0;

void setup() {
Serial.begin(115200);
menuBegin();
}

void loop() {
menuExec();

}
