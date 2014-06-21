#include <Servo.h>

Servo base;
Servo mount;
Servo cont;
int flag=0;
int pos =0;
char ch;
int basepos=90,mountpos=0,contpos=94;
void setup()
{
  base.attach(9);
  mount.attach(11);
  cont.attach(10);
  base.write(90);
  mount.write(0);
  cont.write(94);
  Serial.begin(57600);
}
void loop()
{
   if(Serial.available()>0)
   {
         ch=(char)Serial.read();
         Serial.print(ch);
         while(ch=='a')
         {
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   basepos+=3;
                   
                   base.write(basepos);
                   cont.write(91);
                   
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='a')
                   {
                     cont.write(94);
                     break;
                   }
                 
          }
         while(ch=='b')
         {
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   
                   basepos-=3;

                   base.write(basepos);
                   cont.write(91);
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='b')
                   {
                     cont.write(94);
                     break;
                   }
         
          }          
          while(ch=='c')
         {
                   basepos-=3;
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   base.write(basepos);
                cont.write(97);
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='c')
                   {
                     cont.write(94);
                     break;
                   }
                   
          }         
         while(ch=='d')
         {
                   basepos+=3;
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   base.write(basepos);
                   cont.write(97);
                    while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='d')
                   {
                     cont.write(94);
                     break;
                   }
                   
          }             
         while(ch=='p')
         {
                   basepos+=3;
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   base.write(basepos);
                   
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='p')
                   {
                     
                     break;
                   }
                   
          }  
          while(ch=='q')
         {
   
                   
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   cont.write(91);
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='q')
                   {
                     cont.write(94);
                     break;
                   }
      
          } 
        while(ch=='r')
         {
                   basepos-=3;
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   base.write(basepos);
                   
                     while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='r')
                   {
                    
                     break;
                   }
                  
          }         
while(ch=='s')
         {
                  
                   if(flag==0)
                  { 
                   mount.write(0);
                   flag=1;
                  } 
                   
                   cont.write(97);
                   while(Serial.available()<=0);
                   ch=(char)Serial.read();
                   if(ch!='s')
                   {
                     cont.write(94);
                     break;
                   }
                   
          } 
             
    if(ch=='n')
    {
     mount.write(basepos-90);
     flag=0;
    }    
           
      }
     
}
 
