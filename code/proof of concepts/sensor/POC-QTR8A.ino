#include <QTRSensors.h>

QTRSensors qtra;

const uint8_t SensorAantal = 8;
uint16_t sensorValues[SensorAantal];
int ledOn = 10;

void setup()
{
  qtra.setTypeAnalog();
  qtra.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7} ,SensorAantal);
  qtra.setEmitterPin(10);

  Serial.begin(9600);
}

void loop()
{
  qtra.read(sensorValues);//lezen raw waarden

  //waarden omzetten als nummers tussen 0 en 1023, 0 = max reflectie
  //1023 = min reflectie
  for (uint8_t i = 0; i < SensorAantal; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
  delay(250);
  
}
