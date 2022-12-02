#include "SerialCommand.h"
#include "EEPROMAnything.h"

#define SerialPort Serial
#define Baudrate 9600

#define MotorLeftForward 11
#define MotorLeftBackward 3
#define MotorRightForward 6
#define MotorRightBackward 5

SerialCommand sCmd(SerialPort);
bool debug;
bool run;
bool stop;
unsigned long previous, calculationTime;

int led = 13;

const int sensor[] = {A7, A6, A5, A4, A3, A2, A1, A0};

struct param_t
{
  unsigned long cycleTime;
  int black[8];
  int white[8];
  int power;
  float diff;
  float kp;
  float ki;
} params;

int normalised[8];
float debugPosition;
float iTerm = 0;
float error;
float output;



void setup()
{
  SerialPort.begin(Baudrate);
  
  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibrate", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.addCommand("stop", onStop);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  pinMode(led, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  SerialPort.println("ready");
}

void loop()
{
  sCmd.readSerial();
  
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;

    
    //int normalised[8];
    
    
    //measure & normalize
    for (int i = 0; i < 8; i++) normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 1000);
      
    //interpolatie
    float position;
    int index = 0;
    for (int i = 1; i < 8; i++) if (normalised[i] < normalised[index]) index = i;

    if (normalised[index] > 1200) run = false;

    if (index == 0)position = -30;
    else if (index == 7)position = 30;
    else
    {
    int sNul = normalised[index];
    int sMinEen = normalised[index-1];
    int sPlusEen = normalised[index+1];

    float b = sPlusEen - sMinEen;
    b = b / 2;

    float a = sPlusEen - b - sNul;

    position = -b / (2* a);
    position += index;
    position -= 3.5;

    position *= 9.525;
    }
    debugPosition = position;

   //propotioneel regelaar
    error = -position; 
    output = error * params.kp;

    //integrerend regelaar
     iTerm += params.ki * error;
     output += iTerm;

    output = constrain(output, -510, 510);

    int powerLeft = 0;
    int powerRight = 0;

    if (run) if(output >= 0)
    {
      powerLeft = constrain(params.power + params.diff * output, -255, 255);
      powerRight = constrain(powerLeft - output, -255, 255);
      powerLeft = powerRight + output;
    }
    else 
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255);
      powerLeft = constrain(powerRight + output, -255, 255);
      powerRight = powerLeft - output;
    }

    analogWrite(MotorLeftForward, powerLeft > 0 ? powerLeft : 0);
    analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);
    analogWrite(MotorRightForward, powerRight > 0 ? powerRight : 0);
    analogWrite(MotorRightBackward, powerRight < 0 ? -powerRight : 0);
    
  }
  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();

  
   if (strcmp(param, "cycle") == 0)
   {
    long newCycleTime = atol(value);
    float ratio = ((float) newCycleTime) / ((float) params.cycleTime);

    params.ki *= ratio;

    params.cycleTime = newCycleTime;
   }
   else if (strcmp(param, "power") == 0) params.power = atol(value);
   else if (strcmp(param, "diff") == 0) params.diff = atof(value);
   else if (strcmp(param, "kp") == 0) params.kp = atof(value);
   else if (strcmp(param, "ki") == 0)
  {
    float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
    params.ki = atof(value) * cycleTimeInSec;
  }
  
  EEPROM_writeAnything(0, params);
}

void onRun()
{
  run = true;
  digitalWrite(led, HIGH);
}

void onStop()
{
  run = false;
  digitalWrite(led, LOW);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);

  SerialPort.print("black: ");
  for (int i =0; i < 8; i++)
  {
    SerialPort.print(params.black[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("white: ");
  for (int i =0; i < 8; i++)
  {
    SerialPort.print(params.white[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("normalised: ");
  for (int i = 0; i < 8; i++)
  {
    SerialPort.print(normalised[i]);
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("position: ");
  SerialPort.println(debugPosition);

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialPort.print("kp: ");
  SerialPort.println(params.kp);

  float cycleTimeInSec = ((float) params.cycleTime) / 1000000;
  float ki = params.ki / cycleTimeInSec;
  SerialPort.print("ki: ");
  SerialPort.println(ki);

  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 8; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 8; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");      
  }

  EEPROM_writeAnything(0, params);
}

void onUnknownCommand(char* command)
{
  SerialPort.print("Unknown Command: \"");
  SerialPort.print(command);
  SerialPort.println("\""); 
}
