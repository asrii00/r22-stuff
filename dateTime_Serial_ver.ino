int menu = 0;
int hourToSet = 0;
int minToSet = 0;


int nowhour = 11; //instead of these constants, the current time will be requested from RTC
int nowminute = 11;

int P1= 2; // interrupt/menu button.
int P2= 4; //+/up button //excuse the numbering
int P3= 5; //-/down button

bool timeToEnterMenu = false;


void setHour(void );
void setMinute(void);
void interruptFunction(void);

void setup() {
pinMode(P1, INPUT_PULLUP);
pinMode(P2,INPUT_PULLUP); 
pinMode(P3,INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(P1), interruptFunction, FALLING);

Serial.begin(9600);
  
}

void loop() {

if (timeToEnterMenu==true){ 
  enterTimeMenu(); //possibly rethink this structure
  }
else{
if (nowhour<10) 
    {
    Serial.print('0');
    }
   Serial.print(nowhour);
   Serial.print(":");
   if (nowminute<10) 
    {
    Serial.print('0');
    }
   Serial.println(nowminute);
delay(500);
}

}


void interruptFunction(){

  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore.
  if (interrupt_time - last_interrupt_time > 200) 
  {
    Serial.println("interruptFunction");
timeToEnterMenu = true; //this might be a convoluted way of doing things
menu++;
Serial.println(menu);
  }
  last_interrupt_time = interrupt_time;
}

  
void enterTimeMenu(void){ 
  


  if (menu==0)//display time
  { 
    
   //this is probably where draw() goes. now we just return to loop to print the "time" there.
    timeToEnterMenu = false;
  }
  
  else if (menu==1)
  {
    setHour(); 
  }
  
  else if (menu==2)
  { 
    setMinute();
  }
  
  if (menu==3)
  {
    //this is where the set time will be sent to RTC 
    nowminute = minToSet;
    nowhour = hourToSet;
    menu=0;
    
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
   Serial.println(hourToSet,DEC);
   delay(150);
   //button press should probably be adjusted/ implemented better so that it doesn't increment too much or too slow, 
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
   Serial.println(minToSet,DEC);
   delay(150); //here as well, the button solution should be better
  
}
