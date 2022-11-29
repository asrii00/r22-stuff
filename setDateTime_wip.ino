/*
Based on Clock_Set_Date_Time by Tiziano Bianchettin (Public Domain)
https://create.arduino.cc/projecthub/tittiamo68/clock-set-date-time-0d46a4
 */


// #include <Wire.h> may be needed?
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

int menu = 0;
int hourToSet = 0;
int minToSet = 0;

/* 
int B1= ; // menu/set button
int B2= ; //+/up button
int B3= ; //-/down button
*/

void setHour(void );
void setMinute(void );

//LCD, RTC need to be initalized


void setup() {

//LCD setup needed, probably.

pinMode(B1,INPUT);
pinMode(B2,INPUT);
pinMode(B3,INPUT);

Serial.begin(9600);
Wire.begin();
RTC.begin();
}

void loop() {
 
  if (digitalRead(B1))
  {
    menu=menu+1;
    }
  if (menu==0)//display time
  { 
    draw(); //is this what the time display function is called? check
  }
  
  if (menu==1)//set hour
  {
    setHour(); 
  }
  
  if (menu==2)//set minute
  { 
    setMinute();
  }
  
  if (menu==3)
  {
    menu=0;
  }
  
  //for now lacks any interaction with RTC! RTC.adjust?
}

void DisplayDateTime ()
{

  //puuttuu.
}
}

void setHour()
{
  lcd.clear();
  DateTime now = RTC.now();
    if(digitalRead(B2)==HIGH) 
    {
      if(hourToSet==23) //stops at 23.
      {
        hourToSet=0;
      }
      else
      {
        hourToSet=hourToSet+1; //increment up
      }
     if(digitalRead(B3)==HIGH) 
  
      if(hourToSet==0) //stops at 23.
      {
        hourToSet=23;
      }
      else
      {
        hourToSet=hourToSet-1; //increment down
      }
    }
   lcd.setCursor(0,0);
   lcd.print("Set time:");
   lcd.setCursor(0,1);
   lcd.print(hourToSet,DEC);
   delay(200);
}

void setMinute()
{
    if(digitalRead(B2)==HIGH) 
    {
      if(minToSet==23) //stops at 23.
      {
        minToSet=0;
      }
      else
      {
        minToSet=minToSet+1; //increment up
      }
     if(digitalRead(B3)==HIGH) 
  
      if(minToSet==0) //stops at 23.
      {
        minToSet=23;
      }
      else
      {
        minToSet=minToSet-1; //increment down
      }
    }
   lcd.setCursor(0,0);
   lcd.print("Set time:");
   lcd.setCursor(0,1);
   lcd.print(minToSet,DEC);
   delay(200);
   
}
