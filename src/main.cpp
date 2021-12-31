#include <Arduino.h>
#include "Prov/Prov.h"
#include "IotDevice.hpp"

IotDevice device(Serial);
Prov prov;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74400);

  device.init();

  prov.setSsid("Prov IoT Device");

  prov.begin(device); 
}

void loop() {
  // put your main code here, to run repeatedly:
  prov.task();
}