// All code by Rolf R Bakke, Oct 2012
#include <Adafruit_BMP085.h>

// Constants
#define BUZZER_PIN 9
const byte led = 13;

// Instances
Adafruit_BMP085 bmp;

// Tracker variables
unsigned long time = 0;
float toneFreq, toneFreqLowpass, pressure, lowpassFast, lowpassSlow ;
int ddsAcc;
int counter = 0;

void setup()
{
  Serial.begin(115200);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  pressure = bmp.readPressure();
  lowpassFast = lowpassSlow = pressure;
}

void loop()
{
  pressure = bmp.readPressure();

  lowpassFast = lowpassFast + (pressure - lowpassFast) * 0.1;
  lowpassSlow = lowpassSlow + (pressure - lowpassSlow) * 0.05;

  toneFreq = (lowpassSlow - lowpassFast) * 50;

  toneFreqLowpass = toneFreqLowpass + (toneFreq - toneFreqLowpass) * 0.1;

  toneFreq = constrain(toneFreqLowpass, -500, 500);

  ddsAcc += toneFreq * 100 + 2000;

  if (toneFreq < 0 || ddsAcc > 0)
  {
    tone(BUZZER_PIN, toneFreq + 510);
  }
  else
  {
    noTone(BUZZER_PIN);
  }

  ledOff();

  while (millis() < time); //loop frequency timer
  time += 20; // 20ms. Run at 50Hz

  Serial.println(toneFreq);
  
  ledOn();
}

void ledOn()
{
digitalWrite(led,1);
}

void ledOff()
{
digitalWrite(led,0);
}