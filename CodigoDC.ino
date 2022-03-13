/*
  Programa creado para la medicion del voltaje de salida de una fuente de voltaje DC-DC
  conectada a una ESP8266 para el control de voltaje 
*/

#include "Wire.h"
#include "Adafruit_INA219.h"
#include "Adafruit_SSD1306.h"
#include <ezButton.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define DEBOUNCE_TIME 50 // the debounce time in millisecond, increase this time if it still chatters

Adafruit_INA219 ina219;

#define OLED_RESET 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
ezButton button1(18); // create ezButton object that attach to pin GIOP18;
ezButton button2(19); // create ezButton object that attach to pin GIOP19;


unsigned int Ts=5000;//[us]
unsigned int t, t0;
float Ref=10;
float r=15;
float e=0;
float y=0;
float kp = 0;
float shuntvoltage=0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
int PWM=65535;
float ess = 0;
// the number of the LED pin
const int ledPin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 16;
 

void setup() 
{
  Serial.begin(115200);//Se inicia el monitor serie 
  uint32_t currentFrequency;
  ina219.begin();
  t0=micros();
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  button1.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds
  button2.setDebounceTime(DEBOUNCE_TIME); // set debounce time to 50 milliseconds

  // initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.display();
}

void loop() 
{
    t=micros();
    button1.loop(); // MUST call the loop() function first
    button2.loop(); // MUST call the loop() function first
    if (button1.isPressed())
    r=r+0.1;
    if (button2.isPressed())
    r=r-0.1;
  if(t-t0>=Ts){
    if (Serial.available()) { 
      String command = Serial.readStringUntil('\n');
      r = command.toFloat();
    }
    
    Ref = validar(r,23,7.5);
    PWM_C(Ref);
    t0=t;
  }
}

float validar(float ref, float VMax, float VMin){

  if(ref>VMax){
    ref = VMax;
  }
  if(ref<VMin){
    ref = VMin;
  }
  return ref;
  
}

void PWM_C(float Voltage){

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
 loadvoltage = busvoltage + (shuntvoltage / 1000);
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print(e);
  Serial.print(" \n");
  Serial.print(PWM);
  Serial.print(" \n");
  //r=12;
  y=loadvoltage;
  e=Voltage-y;
  kp = 100;
  ess = 100*abs(e)/Voltage;
  
  if(e!=0){
    PWM=PWM-kp*e;
    
    ledcWrite(ledChannel, PWM);
    }

  if(e==0){
    //PWM=PWM;
    ledcWrite(ledChannel, PWM);
    }

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Vout: ");
  display.print(loadvoltage);
  display.print(" V");

  display.setCursor(75, 0);
  display.print("R:");
  display.print(Voltage);


  display.setCursor(0, 10);
  display.print("I: ");
  display.print(current_mA);
  display.print(" mA");

  display.setCursor(0, 20);
  display.print("Error: ");
  display.print(ess);
  display.print(" %");


  display.display();

    

 }
