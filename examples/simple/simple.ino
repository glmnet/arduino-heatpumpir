#include <Arduino.h>

#include <FujitsuHeatpumpIR.h>
#include <PanasonicCKPHeatpumpIR.h>
#include <PanasonicHeatpumpIR.h>
#include <CarrierHeatpumpIR.h>
#include <MideaHeatpumpIR.h>
#include <MitsubishiHeatpumpIR.h>
#include <SamsungHeatpumpIR.h>
#include <SharpHeatpumpIR.h>
#include <DaikinHeatpumpIR.h>
#include <MitsubishiHeavyHeatpumpIR.h>
#include <HyundaiHeatpumpIR.h>


#ifndef ESP8266
IRSenderPWM irSender(3);       // IR led on Arduino digital pin 3, using Arduino PWM
//IRSenderBlaster irSender(3); // IR led on Arduino digital pin 3, using IR Blaster (generates the 38 kHz carrier)
#else
IRSenderBitBang irSender(1);   // IR led on ESP8266 GPIO pin 1
#endif


// Array with all supported heatpumps
HeatpumpIR *heatpumpIR[] = {new PanasonicCKPHeatpumpIR(), new PanasonicDKEHeatpumpIR(), new PanasonicJKEHeatpumpIR(),
                            new PanasonicNKEHeatpumpIR(), new CarrierHeatpumpIR(), new MideaHeatpumpIR(),
                            new FujitsuHeatpumpIR(),
                            new MitsubishiFDHeatpumpIR(), new MitsubishiFEHeatpumpIR(), new MitsubishiMSYHeatpumpIR(),
                            new SamsungHeatpumpIR(), new SharpHeatpumpIR(), new DaikinHeatpumpIR(),
                            new MitsubishiHeavyZJHeatpumpIR(), new MitsubishiHeavyZMHeatpumpIR(),
                            new HyundaiHeatpumpIR(), NULL};

void setup()
{
  Serial.begin(9600);
  delay(500);

  Serial.println(F("Starting"));
}

void loop()
{
  int i = 0;
  const char* buf;

  do {
    // Send the same IR command to all supported heatpumps
    Serial.print(F("Sending IR to "));

    buf = heatpumpIR[i]->model();
    // 'model' is a PROGMEM pointer, so need to write a byte at a time
    while (char modelChar = pgm_read_byte(buf++))
    {
      Serial.print(modelChar);
    }
    Serial.print(F(", info: "));

    buf = heatpumpIR[i]->info();
    // 'info' is a PROGMEM pointer, so need to write a byte at a time
    while (char infoChar = pgm_read_byte(buf++))
    {
      Serial.print(infoChar);
    }
    Serial.println();

    // Send the IR command
    heatpumpIR[i]->send(irSender, POWER_ON, MODE_HEAT, FAN_2, 24, VDIR_UP, HDIR_AUTO);
    delay(500);
  }
  while (heatpumpIR[++i] != NULL);

  delay(2000);
}
