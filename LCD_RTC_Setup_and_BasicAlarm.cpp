

//  10.12.2022 18:39 - menus work on LCD display now and alarm is triggered. it plays once and can't be turned off yet. 
//there may be a bug hiding somewhere.

//  sorry about the bad formatting. 

/*MISSING/IN PROGRESS: 
- menu option to clear alarm
- being able to dismiss/turn off the alarm
- temperature sensor + displaying the temperature
- first boot? i'm not sure how this would be detected. (it's not crucial functionality though)

 bonus/non-essential stuff:
- improve button detection
- clean up structure in void loop
*/
#include <Arduino.h>
#include <Wire.h>
#include <ds3231.h> // https://github.com/rodan/ds3231
#include <LiquidCrystal_I2C.h> 
#include <SPI.h>

/*   Buzzer Pin   */
#define BUZZER_PIN 6

#define LCD_I2C_ADDRESS 0x3F
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

int P1= 2; // interrupt/menu button.
int P2= 3; //+/up button 
int P3= 4; //-/down button

bool redraw = true;
bool alarmSet = false; //not used yet!
void draw();
void alarmSong();
void interruptFunction(void );
 
struct ts t; 

/* All the menu stuff.. unorganized */
int menu = 0;
int hourToSet = 0;
int minToSet = 0;

int menu2=0;
int alarmHrToSet = 0;
int alarmMinToSet = 0;

int alarmHr = 80;
int alarmMin = 80; 

bool timeToEnterMenu = false;
bool inSetup = false;

void firstMenu(void );
void alarmSetMenu(void );
void timeSetMenu(void );
void setHour(void );
void setMinute(void );
void setAlarmHour(void );
void setAlarmMinute(void );

 
void setup() {
  /* Pins*/
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(P1,INPUT_PULLUP);
    pinMode(P2,INPUT_PULLUP); 
    pinMode(P3,INPUT_PULLUP);

  Serial.begin(9600);
  

  /*  Setup RTC  */
 
  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  /*  Setup LCD  */
  lcd.init();
  lcd.backlight();

 /* Temporary time set, will be removed.
  t.hour=12; 
  t.min=30;
  t.sec=0;
  t.mday=25;
  t.mon=12;
  t.year=2019;
 
  DS3231_set(t); */
}
 
void loop() {
    DS3231_get(&t);
    if(digitalRead(P1)==LOW)
            {
            delay(100);
            if(digitalRead(P1)==LOW){
            timeToEnterMenu = true;  //this needs to be simplified, just call firstMenu() directly if P1 is low & insetup == false
            
            }
            }
            
    if (timeToEnterMenu==true && inSetup==false){ 
    firstMenu();
    }
    else
    { 
    
    if (t.hour == alarmHr && t.min == alarmMin && t.sec == 0){ //plays once now, could be a loop until a minute has passed or it's dismissed by button press
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("ALARM");
        lcd.setCursor(1,1);
        lcd.print("OK to dismiss"); //THIS IS NOT IMPLEMENTED YET
        alarmSong();
        }

        
    if (redraw){
        draw();}
    }
    
}


void draw(void){
  
  lcd.setCursor(1,1);
  if (t.hour<10){
    lcd.print("0");
  }
  lcd.print(t.hour);
  lcd.print(":");
  if (t.min<10){
    lcd.print("0");
  }
  lcd.print(t.min);
  lcd.print(".");
  if (t.sec<10){
    lcd.print("0");
  }
  lcd.print(t.sec);
  lcd.print("   ");
  if(digitalRead(P1)==LOW){}
 delay(20); //longer delays interfere with button.. a structure issue probably. or a small brain issue.

}

void alarmSong() { // by Din

  delay(600);
  tone(6, 329.63, 300);
  delay(350);
  tone(6, 311.13, 300);
  delay(350);
  tone(6, 329.63, 300);
  delay(350);
  tone(6, 311.13, 300);
  delay(350);
  tone(6, 329.63, 300);
  delay(350);
  tone(6, 246.94, 300);
  delay(400);
  tone(6, 293.66, 300);
  delay(400);
  tone(6, 261.63, 300);
  delay(400);
  tone(6, 220, 900);
  delay(1000);
  tone(6, 146.83, 300);
  delay(350);
  tone(6, 174.61, 300);
  delay(400);
  tone(6, 220, 300);
  delay(400);
  tone(6, 246.94, 900);
  delay(1000);
  tone(6, 174.61, 300);
  delay(400);
  tone(6, 233.08, 300);
  delay(400);
  tone(6, 246.94, 300);
  delay(400);
  tone(6, 261.63, 900);
  delay(1000);
  delay(300);

  lcd.clear();
}


void firstMenu(void){ 
 
 inSetup=true;
 int timeOrAlarm = 0;
 bool canExit = false;

  while (canExit == false){
    
  if(digitalRead(P2)==LOW) //up button pressed
        {
           delay(200);
           timeOrAlarm++;
        }
   if(digitalRead(P3)==LOW) //down button pressed
        {
           delay(200);
           timeOrAlarm--;

        }

   if(timeOrAlarm>1){
      timeOrAlarm = 0;
      }
      
   else if(timeOrAlarm<0){
      timeOrAlarm = 1;
      }

   else if(timeOrAlarm==0)
      { 
      lcd.setCursor(1,0);
      lcd.print("Set time       ");
      lcd.setCursor(1,1);
      lcd.print("<-      ->");
  
      if(digitalRead(P1)==LOW) //OK button pressed
        {
        delay(50);
          if(digitalRead(P1)==LOW)
          {
          lcd.clear();
          timeSetMenu();
          timeToEnterMenu = false;
          canExit = true;
          }
        } 
     }
    
    else if (timeOrAlarm==1) 
    {
      lcd.setCursor(1,0);
      lcd.print("Set/clear alarm");
      lcd.setCursor(1,1);
      lcd.print("<-      ->");

     if(digitalRead(P1)==LOW) //OK button pressed
        {
          delay(50);
          if(digitalRead(P1)==LOW){
          lcd.clear();
          alarmSetMenu();
          timeToEnterMenu = false;
          canExit = true;
          }
        }
    }
    
  }
}
  
void timeSetMenu(void){ 
  bool canExit2 = false;
  menu=0;
  delay(300);
  while (canExit2 == false) {
  
     if(digitalRead(P1)==LOW) 
    {
        menu++; //increment up
    }
    
   
    if (menu==0)
    {
        setHour(); 
    }
      
    else if (menu==1)
    { 
        setMinute();
    }
      
    else if (menu==2)
    {
         //set time is sent to RTC 
        t.min = minToSet;
        t.hour = hourToSet;
        t.sec = 0;
        DS3231_set(t);
        lcd.clear();  
        menu=0;

        delay(300);
        inSetup = false;
        canExit2 = true;
        
    }
    else
    { 
         menu=0;
    }
       
  
  }
       
    
} 


void setHour()
{
   
    if(digitalRead(P2)==LOW) 
    {
      if(hourToSet==23) //stops at 23.
      {
        hourToSet=0;
      }
      else
      {
        hourToSet=hourToSet+1; //increment up
      }
    }
     if(digitalRead(P3)==LOW)
    { 
  
      if(hourToSet==0) //stops at 0.
      {
        hourToSet=23;
      }
      else
      {
        hourToSet=hourToSet-1; //increment down
      }
    }
   
   Serial.print("Set hour:");
   Serial.println(hourToSet, DEC);
   delay(100);

   lcd.setCursor(1,0);
   lcd.print("Set hour       ");
   lcd.setCursor(1,1);
   if (hourToSet<10){
        lcd.print("0");}
   lcd.print(hourToSet, DEC);
   
   //button press should probably be adjusted/ implemented better so that it doesn't increment too fast or too slow, 
   //but it works just enough.
}

void setMinute()
{
    if(digitalRead(P2)==LOW)
    {
      
      if(minToSet==59) //stops at 60.
      {
        minToSet=0;
      }
      else
      {
        minToSet=minToSet+1; //increment up
      }
    }
     if(digitalRead(P3)==LOW) 
     {
      if(minToSet==0) //stops at 0.
      {
        minToSet=59;
      }
      else
      {
        minToSet=minToSet-1; //increment down
      }
    }
   Serial.print("Set minute:");
   Serial.println(minToSet, DEC);
   delay(150); //here as well, the button solution should be better
   lcd.setCursor(1,0);
   lcd.print("Set minute     ");
   lcd.setCursor(1,1);
   if (minToSet<10){
        lcd.print("0");}
   lcd.print(minToSet, DEC);
  
}
 

void alarmSetMenu(void ){
  Serial.println("trying to set alarm");

  bool canExit3 = false;
  menu2=0;
  delay(300);
  while (canExit3 == false) {
  
     if(digitalRead(P1)==LOW) 
    {
        menu2++; //increment up
    }
    
   
    if (menu2==0)
    {
        setAlarmHour(); 
    }
      
    else if (menu2==1)
    { 
        setAlarmMinute();
    }
      
    else if (menu2==2)
    {
         //set time is sent to RTC 
        alarmHr = alarmHrToSet;
        alarmMin = alarmMinToSet;
       
        lcd.clear(); 
        lcd.setCursor(1,0);
        lcd.print("Alarm set at:");
        lcd.setCursor(1,1);
        if (alarmHr<10){
        lcd.print("0");}
        lcd.print(alarmHr);
        lcd.print(":");
        if (alarmMin<10){
        lcd.print("0");}
        lcd.print(alarmMin);
        delay(4000);

      
        lcd.clear();  
        menu2=0;

        delay(300);
        inSetup = false;
        canExit3 = true;
        
    }
    else
    { 
         menu2=0;
    }
       
  
  }
       
    
} 


void setAlarmHour()
{
   
    if(digitalRead(P2)==LOW) 
    {
      if(alarmHrToSet==23) //stops at 23.
      {
        alarmHrToSet=0;
      }
      else
      {
        //lcd.clear();
        alarmHrToSet=alarmHrToSet+1; //increment up
      }
    }
  if(digitalRead(P3)==LOW)
    { 
  
      if(alarmHrToSet==0) //stops at 0.
      {
        alarmHrToSet=23;
      }
      else
      {
        //lcd.clear();
        alarmHrToSet=alarmHrToSet-1; //increment down
      }
    }
   
   Serial.print("Set alarm hour:");
   Serial.println(alarmHrToSet, DEC);
   delay(150);

   lcd.setCursor(1,0);
   lcd.print("Set alarm hour   ");
   lcd.setCursor(1,1);
   if (alarmHrToSet<10){
   lcd.print("0");}
   lcd.print(alarmHrToSet, DEC);
   
   //button press should probably be adjusted/ implemented better so that it doesn't increment too fast or too slow, 
   //but it works just enough.
}

void setAlarmMinute()
{
    if(digitalRead(P2)==LOW)
    {
      
      if(alarmMinToSet==59) //stops at 60.
      {
        alarmMinToSet=0;
      }
      else
      {
        //lcd.clear();
        alarmMinToSet=alarmMinToSet+1; //increment up
      }
    }
     if(digitalRead(P3)==LOW) 
     {
      if(alarmMinToSet==0) //stops at 0.
      {
        alarmMinToSet=59;
      }
      else
      {
        //lcd.clear();
        alarmMinToSet=alarmMinToSet-1; //increment down
      }
    }
   Serial.print("Alarm minute:");
   Serial.println(alarmMinToSet, DEC);
   delay(150); //here as well, the button solution should be better

   lcd.setCursor(1,0);
   lcd.print("Alarm minute   ");
   lcd.setCursor(1,1);
   if (alarmMinToSet<10){
   lcd.print("0");}
   lcd.print(alarmMinToSet, DEC);
  
}