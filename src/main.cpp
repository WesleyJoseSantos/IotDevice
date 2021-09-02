#include <Arduino.h>
#include "Prov/Prov.h"
#include "IotDevice.hpp"

IotDevice device(Serial);
Prov prov(device, "Prov AP", 30000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74400);

  device.init();
  prov.begin("/prov.json", "/prov.html"); 
}

void loop() {
  // put your main code here, to run repeatedly:
  prov.task();
}