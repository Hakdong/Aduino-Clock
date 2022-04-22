#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <TimeLib.h>
#include <TimerOne.h>
#include "pitches.h"
#include <SoftwareSerial.h>
LiquidCrystal_I2C lcd(0x27,20,4); 

SoftwareSerial mySerial(2,3);
int lcd_key     = 0;
int adc_key_in  = 0;
char blue_key;
int i =0;
int state = 0;
int old_state = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
int tonepin = 8;
int read_LCD_buttons() 
{
  adc_key_in=analogRead(0);
  blue_key = mySerial.read();

  if(adc_key_in ==0)
  {
    state = 1;

    if(old_state != state)
    {
      old_state = state;


      return btnUP;
    }
    lcd_key = 5;
    return lcd_key;
  }
  
  else if(adc_key_in >320 && adc_key_in <330)
  {
    state = 2;
    if(old_state != state)
    {
      old_state = state;
      return btnLEFT;
    }
    lcd_key = 5;
    return lcd_key;
  }
  
  else if(adc_key_in >130 && adc_key_in <145)
  {
    state = 3;
    if(old_state != state)
    {
      old_state = state;
      return btnDOWN; 
    }
    lcd_key = 5;
    return lcd_key;
  }
  else if(adc_key_in >490 && adc_key_in <510)
  {
    state = 4;
    if(old_state != state)
    {
      old_state = state;
      return btnRIGHT; 
    }
    lcd_key = 5;
    return lcd_key;
  }
  else if(adc_key_in >720 && adc_key_in <740)
  {
    state = 5;
    if(old_state != state)
    {
      old_state = state;
      return btnSELECT; 
    }
    lcd_key = 5;
    return lcd_key;
  }
    if (blue_key == 'U')
    {
      lcd_key = 1;
    }
    else if (blue_key == 'D')
    {
      lcd_key = 2;
    }
    else if (blue_key == 'L')
    {
      lcd_key = 3;
    }
    else if (blue_key == 'S')
    {
      lcd_key = 4;
    }
    else if (blue_key =='R')
    {
      lcd_key = 0;
    }
  else
  {
    state = 0;
    old_state = 0;

    return btnNONE;
  }
    

}
char str[20];



 struct MyObject
 {
  int hours;
  int minutes;
  int seconds;
  char str2[5];
 }ctime;
 
int alarmhours;
int alarmminutes;
int alarmseconds;
int alarmcycle_12;
char str3[5];

int count = 1;
int cycle_12=0;
int j=0;

void timerlsr()
{
  count++;
    if (count % 100 == 0)
    {
      ctime.seconds++;
      if( ctime.seconds >= 60)
      {
        ctime.minutes++;
        ctime.seconds=0;
        if(ctime.minutes >= 60)
        {
          ctime.hours++;
          ctime.minutes = 0;
          if(ctime.hours >= 12)
          {
          ctime.hours = 0;  
          cycle_12 = 1;
          }
          else if(ctime.hours >= 12)
          {
          ctime.hours = 0;
          cycle_12 = 0;
          }
        }
      }
    }
  if(cycle_12 == 0)
    strcpy(ctime.str2,"AM");
  if (cycle_12 == 1)
    strcpy(ctime.str2,"PM"); 
    
  if ( count % 1000 == 0)
  { 
     EEPROM.write(4,ctime.hours);
     EEPROM.write(5,ctime.minutes);
     EEPROM.write(6,ctime.seconds);
     EEPROM.put(15,ctime.str2);
     count = 0;
  }
}
void timeselect()
{
  lcd_key = read_LCD_buttons();
  switch (lcd_key) 
  { 
    case btnUP: 
    {    
      if(i== 1)
      {
        ctime.hours ++;
         if(ctime.hours >= 12)
         {
           ctime.hours = 11;
         }
         return ctime.hours;
       }
       if(i== 4)
       {
         ctime.minutes += 10;
           if(ctime.minutes > 60)
           {
             ctime.minutes = 59;
           }
           return ctime.minutes;
       }

       if(i== 5)
       {
         ctime.minutes += 1;
         if(ctime.minutes > 60)
         {
           ctime.minutes = 59;
         }
         return ctime.minutes;      
       }           
       if(i== 8)
       {
         ctime.seconds += 10;
         if(ctime.seconds > 60)
         {
           ctime.seconds = 59;
         }
         return ctime.seconds;
       }
       if(i== 9)
       {
         ctime.seconds += 1;
         if(ctime.seconds > 60)
         {
           ctime.seconds = 59;
         }
         return ctime.seconds;
       }
       if(i == 11 || i == 12)
       {
         if(cycle_12 == 0)
          {
            cycle_12 = 1;
            return cycle_12;
          }
          else if(cycle_12 == 1)
          {
            cycle_12 = 0;
            return cycle_12;
          }  
       }
       break;
   }
   case btnDOWN: 
   {
     if(i== 1)
     {
       ctime.hours -= 1;
       if(ctime.hours < 0)
       {
         ctime.hours = 0;
       }
       return ctime.hours;
     }
     if(i== 4)
     {
       ctime.minutes -= 10;
       if(ctime.minutes < 0)
       {
          ctime.minutes = 0;
       }
       return ctime.minutes;
     }
     if(i== 5)
     {
       ctime.minutes -= 1;
       if(ctime.minutes < 0)
       {
          ctime.minutes = 0;
       }
       return ctime.minutes;
     }
     if(i== 8)
     {
       ctime.seconds -= 10;
       if(ctime.seconds < 0)
       {
         ctime.seconds = 0;
       }
       return ctime.seconds;
     }
     if(i== 9)
     {
       ctime.seconds -= 1;
       if(ctime.seconds < 0)
       {
         ctime.seconds = 0;
       }
       return ctime.seconds;
     }
     if(i == 11 || i == 12)
     {
         if(cycle_12 == 0)
          {
            cycle_12 = 1;
            return cycle_12;
          }
          else if(cycle_12 == 1)
          {
            cycle_12 = 0;
            return cycle_12;
          }  
     }
     break;
   }
   case btnLEFT: 
   {
     i--;
     if(i < 0)
     {
       i = 12;
     }
     break;
   }
   case btnRIGHT: 
   {
     i++; 
     if(i > 12)
     {
       i = 0;
     }
     break;
   }
   case btnNONE: 
   {
     lcd.setCursor(i,1);
     lcd.blink();
     delay(50);
   }
  }
}
void timeselect2()
{
  lcd_key = read_LCD_buttons();
  switch (lcd_key) 
  { 
    case btnUP: 
    {    
      if(i== 1)
      {
        alarmhours += 1;
        if(alarmhours >= 12)
        {
          alarmhours = 11;
        }
        return alarmhours;
      }
      if(i== 4)
        {
           alarmminutes += 10;
           if(alarmminutes > 60)
           {
             alarmminutes = 59;
           }
           return alarmminutes;
        }
        if(i== 5)
        {
          alarmminutes += 1;
          if(alarmminutes > 60)
          {
            alarmminutes = 59;
          }
          return alarmminutes;      
        }
        if(i== 8)
        {
          alarmseconds += 10;
          if(alarmseconds > 60)
          {
            alarmseconds = 59;
          }
          return alarmseconds;
        }
        if(i== 9)
        {
          alarmseconds += 1;
          if(alarmseconds > 60)
          {
            alarmseconds = 59;
          }
          return alarmseconds;
        }
        break;
      }
        if(i == 11 || i == 12)
        {
         if(alarmcycle_12 == 0)
          {
            alarmcycle_12 = 1;
            return alarmcycle_12;
          }
          else if(alarmcycle_12 == 1)
          {
            alarmcycle_12 = 0;
            return alarmcycle_12;
          }  
        }
      case btnDOWN: 
      {
        if(i== 1)
        {
          alarmhours -= 1;
          if(alarmhours < 0)
          {
            alarmhours = 0;
          }
          return alarmhours;
        }
        if(i== 4)
        {
          alarmminutes -= 10;
          if(alarmminutes < 0)
          {
            alarmminutes = 0;
          }
           return alarmminutes;
        }
        if(i== 5)
        {
          alarmminutes -= 1;
          if(alarmminutes < 0)
          {
            alarmminutes = 0;
          }
          return alarmminutes;
        }
        if(i== 8)
        {
          alarmseconds -= 10;
          if(alarmseconds < 0)
          {
            alarmseconds = 0;
          }
          return alarmseconds;
        }
        if(i== 9)
        {
           alarmseconds -= 1;
           if(alarmseconds < 0)
           {
             alarmseconds = 0;
           }
           return alarmseconds;
        }
     if(i == 11 || i == 12)
        {
         if(alarmcycle_12 == 0)
          {
            alarmcycle_12 = 1;
            return alarmcycle_12;
          }
          else if(alarmcycle_12 == 1)
          {
            alarmcycle_12 = 0;
            return alarmcycle_12;
          }  
        }
        break;
      }
      case btnLEFT: 
      {
        i-= 1;
        if(i < 0)
        {
          i = 12;
        }
        break;
      }
      case btnRIGHT: 
      {
        i+= 1;            
        if(i > 12)
        {
         i = 0;
        }
        break;
      }
      case btnNONE: 
      {
        lcd.setCursor(i,1);
        lcd.blink();
        delay(50);
      }
    }
}
void timeset()
{

   lcd.setCursor(0,1);
   sprintf(str, "%02d: %02d: %02d %s     ",ctime.hours,ctime.minutes,ctime.seconds,ctime.str2);
   lcd.print(str);
   timeselect();
   if(lcd_key == 4)
   {
   EEPROM.write(7,ctime.hours);
   EEPROM.write(8,ctime.minutes);
   EEPROM.write(9,ctime.seconds);
   }
}
void alarmclock()
{
    if(alarmcycle_12 == 0)
    strcpy(str3,"AM");
    if(alarmcycle_12 == 1)
    strcpy(str3,"PM"); 
   lcd.clear();
   lcd.setCursor(0,1);
   sprintf(str, "%02d: %02d: %02d %s AL",alarmhours,alarmminutes,alarmseconds,str3);
   lcd.print(str);
   
   timeselect2();
   if(lcd_key == 4)
   {
    EEPROM.write(10,alarmhours);
    EEPROM.write(11,alarmminutes);
    EEPROM.write(12,alarmseconds);
    EEPROM.put(16,str3);
   }
}
void alarmmusic()
{

  lcd_key = read_LCD_buttons();
   switch (lcd_key) 
   { 
      case btnUP: 
      {  
        j++;
        if( j > 2)
          j = 0;
          return j;
      }
      case btnDOWN:
      {
        j--;
        if(j < 0)
          j = 2;
          return j;
      }
   }
   if(j == 0)
   {
     int melody1[] = {
     NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,NOTE_C5,               
     NOTE_E5,NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,               
     NOTE_G5,NOTE_G5,NOTE_E5,NOTE_G5,NOTE_G5,NOTE_E5,       
     NOTE_C5,NOTE_C5,NOTE_C5,                               
     NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,                       
     NOTE_G5,NOTE_G5,NOTE_G5,                               
     NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,                       
     NOTE_G5,NOTE_G5,NOTE_G5,                               
     NOTE_G5,NOTE_G5,NOTE_E5,NOTE_C5,                       
     NOTE_G5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_G5,               
     NOTE_C6,NOTE_G5,NOTE_C6,NOTE_G5,                       
     NOTE_E5,NOTE_D5,NOTE_C5                                
     };
     int noteDurations1[]={
     4,8,8,4,4,
     4,8,8,4,4,
     8,8,4,8,8,4,
     4,4,2,
     4,4,4,4,
     4,4,2,
     4,4,4,4,
     4,4,2,
     4,4,4,4,
     8,8,8,8,2,
     4,4,4,4,
     4,4,2
     };
     lcd.setCursor(1,0);
     lcd.print("Three bears   ");
     if(lcd_key==4)
     {
     EEPROM.put(13,melody1);
     EEPROM.put(14,noteDurations1);
     }
   }
   else if(j == 1)
   {
     int melody2[] = {
     NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
     NOTE_DS6,NOTE_B5,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_B5,
     NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
     NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_D5,NOTE_D5,0,
     NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
     NOTE_DS6,NOTE_B5,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_B5,
     NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
     NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_D5,NOTE_D5,0,
     NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,
     NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,
     NOTE_E5,NOTE_E5
     };
     int noteDurations2[]={
     4,4,2,4,4,2,
     4,4,4,4,4,4,4,2,
     4,4,2,4,4,2,
     4,4,4,4,4,4,2,16,
     4,4,2,4,4,2,
     4,4,4,4,4,4,4,2,
     4,4,2,4,4,2,
     4,4,4,4,4,4,2,16,
     8,8,8,8,8,8,8,8,
     8,8,8,8,8,8,8,8,
     1,1
     };
     lcd.setCursor(1,0);
     lcd.print("Squid Game BGM");
     if(lcd_key==4)
     {
     EEPROM.put(13,melody2);
     EEPROM.put(14,noteDurations2);
     }
   }
   else if(j == 2)
   {
      int melody3[] = {
      NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
      NOTE_DS6,NOTE_B5,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_B5,
      NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
      NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_D5,NOTE_D5,0,
      NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
      NOTE_DS6,NOTE_B5,NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_B5,
      NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,NOTE_B5,
      NOTE_B5,NOTE_A5,NOTE_G5,NOTE_A5,NOTE_G5,NOTE_D5,NOTE_D5,0,
      NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,
      NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,NOTE_E5,NOTE_FS5,
      NOTE_E5,NOTE_E5
      };
      
      int noteDurations3[]={
      4,4,2,4,4,2,
      4,4,4,4,4,4,4,2,
      4,4,2,4,4,2,
      4,4,4,4,4,4,2,16,
      4,4,2,4,4,2,
      4,4,4,4,4,4,4,2,
      4,4,2,4,4,2,
      4,4,4,4,4,4,2,16,
      8,8,8,8,8,8,8,8,
      8,8,8,8,8,8,8,8,
      1,1
      };  
      lcd.setCursor(1,0);
      lcd.print("Kill Bill ost  ");
      if(lcd_key==4)
     {
      EEPROM.put(13,melody3);
      EEPROM.put(14,noteDurations3);
     }
   }
}
void alarm()
{

int melody[60];
int noteDurations[60];
      
    EEPROM.get(13,melody);
    EEPROM.get(14,noteDurations);
    for (int thisNote = 0; EEPROM.length(); thisNote++) 
    {
      lcd_key = read_LCD_buttons();
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(8, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 2.00;
      delay(pauseBetweenNotes);
      noTone(8);
      if(lcd_key==4)
      {
        break;
      }
    }

}
void setup()
{
  pinMode(tonepin,INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.init();           
  lcd.backlight();   
    
  ctime.hours = EEPROM.read(4);
  ctime.minutes = EEPROM.read(5);
  ctime.seconds = EEPROM.read(6);
  EEPROM.get(16,ctime.str2);
  alarmhours = EEPROM.read(10);
  alarmminutes = EEPROM.read(11);
  alarmseconds = EEPROM.read(12);
  EEPROM.get(16,str3);
   Timer1.initialize(10000);
   Timer1.attachInterrupt(timerlsr);
}


void loop()
{

  lcd.setCursor(1,0);
  lcd.print("Bluetooth Clock");
  lcd.setCursor(0,1);
  sprintf(str, "%02d: %02d: %02d %s      ",ctime.hours,ctime.minutes,ctime.seconds,ctime.str2);
  lcd.print(str);
  lcd_key = read_LCD_buttons(); 
  Serial.print(lcd_key);
   switch (lcd_key)
        case btnSELECT: 
        {
               ctime.hours = EEPROM.read(7);
               ctime.minutes = EEPROM.read(8);
               ctime.seconds = EEPROM.read(9);
               lcd.clear();
               lcd.setCursor(1,0);
               lcd.print("Time Setting");
               lcd.setCursor(0,1);
               sprintf(str, "%02d: %02d: %02d %s      ",ctime.hours,ctime.minutes,ctime.seconds,ctime.str2);
               lcd.print(str);
               lcd_key= 0;
          while(lcd_key !=4)//시간 설정
          {
            while(1)
            {
            timeset();
            state = 1;
            if(lcd_key == 4)
              break;
            }
            
            while(1)
            {
            alarmclock();
            if(lcd_key == 4)
              break;
            }
            while(1)
            {
              alarmmusic();
             if(lcd_key == 4)
              break;
            }
          }
        }
  if( ctime.hours ==  alarmhours &&
      ctime.minutes ==  alarmminutes &&
      ctime.seconds ==  alarmseconds &&
      strcmp(ctime.str2,str3)==0)
    {
      alarm(); 
    }
}
