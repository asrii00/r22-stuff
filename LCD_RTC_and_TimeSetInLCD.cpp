

//  10.12. 11:07 - 1st menu and time setup works on LCD display now. 
//  sorry about the bad formatting. 


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

void draw();
void alarmSong();
void interruptFunction(void );
 
struct ts t; 

/* All the menu stuff.. unorganized */
int menu = 0;
int hourToSet = 0;
int minToSet = 0;

bool timeToEnterMenu = false;
bool inSetup = false;


void alarmSetMenu(void );
void timeSetMenu(void );
void setHour(void );
void setMinute(void );
void firstMenu(void );
 
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

 /* Temporary time set*/ //will be removed.
  /*t.hour=12; 
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
            timeToEnterMenu = true;
            
            }
            }
            
    if (timeToEnterMenu==true && inSetup==false){ 
    firstMenu();
    }
    else
    { 
    
    if (1==2){ //add something sensible here
        alarmSong();}
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

void alarmSong() {

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
   lcd.print(hourToSet, DEC);
   
   //button press should probably be adjusted/ implemented better so that it doesn't increment too fast or too slow, 
   //but it works just enough.
}

void setMinute()
{
    if(digitalRead(P2)==LOW)
    {
      
      if(minToSet==60) //stops at 60.
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
   lcd.print(minToSet, DEC);
  
}
 

void alarmSetMenu(void ){
  Serial.println("you tried to set alarm");
  
  
   inSetup = false;
  
}