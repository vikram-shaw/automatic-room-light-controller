#include<EEPROM.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(2,3,8,9,10,11);
#define MAX 10
const int key=A1;
const int ldr=A0;
const int light=13;
const int motion=5;
int queue[MAX],p,x=0,front=-1,rear=-1,e=0,y=0,s=0,q=0,l;
int address=0;
void setup()
{
  pinMode(key,INPUT);//reading input from keyboard to A1 
  pinMode(ldr,INPUT);//reading input from external light source
  pinMode(light,OUTPUT);//output to the relay
  pinMode(motion,INPUT);//reading human motion 
  lcd.begin(16,2);//using LCD display of size 16x2
  lcd.setCursor(0,0);//setting cursor at 0,0 point of the LCD display
  lcd.print("ENTER NUM: ");
  l=EEPROM.read(address); //retrieve previously stored data from Arduino
  lcd.setCursor(0,1);//setting cursor at 0,1 point of the LCD display
  lcd.print("KEY:");
  lcd.print(4*l);//retrieved data is multiplied by 4
  digitalWrite(light,0);
  s=4*l;//setting the retrieved data to s
  Serial.begin(9600);
}
void loop()
{
  y=analogRead(ldr);//reading value of ldr
  x=analogRead(key);//reading from keyboard
  lcd.setCursor(8,1);//cursor set at the position 8,1 of the LCD
  lcd.print("LDR:");
  lcd.print("    ");
  lcd.setCursor(12,1);//cursor set at the position 12,1 of the LCD
  lcd.print(y);
  while(1)
  {
    x=analogRead(key);//reading value from user with the help of keyboard
    y=1023-analogRead(ldr);
    q=digitalRead(motion);//detecting motion of human
    if(y<=s && q)
    {
      digitalWrite(light,0);//relay reset and light glow
    }
    else
    digitalWrite(light,1);//relay will remain set and light do not glow
    if(q)//printing the value to the LCD display
    {
      lcd.setCursor(14,0);
      lcd.print("  ");
      lcd.setCursor(14,0);
      lcd.print("M"); //printing and indicating human presence
    }
    else
      {
        lcd.setCursor(14,0);
        lcd.print("NM");//printing there is no human
      }
  lcd.setCursor(8,1);
  lcd.print("LDR:");
  lcd.print("    ");
  lcd.setCursor(12,1);
  lcd.print(y);
    delay(500);
    p=button(x);//which button has been pressed by user
    if(p==35)//# is pressed indicating enter is pressed
    {
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
    if(p!=110)//ignoring this value continuously coming from keypad
       break;
  }
  if(x!=-1)
  {
    if(p!=35 && p!=42)
    {
    insert(p);//inserting value of pressed key to the queue
    {
      if(e==0)
      {
      lcd.setCursor(10,0);
      lcd.print("      ");
      }
      lcd.setCursor(10+e,0);
      lcd.print(p);
      e++;
    }
    }
  }
  while(p==35)//#(enter) is pressed
  {
    int t=0,r,h=0;
    while(!empty())//while queue is not empty
    {
      r=del();//deleting the element from queue
      t=t*10+r;//t is calculated as in the form of pure number
    }
    e=0;
    EEPROM.write(address,t/4);//storing the value inputed by user to the Rom
    lcd.setCursor(0,1);
    lcd.print("KEY:");
    lcd.print(t);
    s=t;//store the value permanently to the variable s
    p=42;//value from 0 to 110 is ignored
  }
   if(s<=y)//if value given by user is less than value read by LDR
    digitalWrite(light,1);// relay is set and light will not glow
    else
    digitalWrite(light,0);//relay is reset and light will glow
}
int button(int y)//passing the value of current passing to the A1 via keypad
{
	//according to the current we are distinguishing the pressed button of keypad
  Serial.println(y);
  if(y>=0 && y<=200)
  {
    return 'n';}// return n=110;//ignoring this value as is returns ascii value of 'n'
  else if(y>=260 && y<=280)
    return 1;
  else if(y>=415 && y<=430)
    return 2;
  else if(y>=1010 && y<=1023)
    return 3;
  else if(y>=240 && y<=255)
    return 4;
  else if(y>=365 && y<=385)
    return 5;
  else if(y>=770 && y<=790)
    return 6;
  else if(y>=225 && y<=239)
    return 7;
  else if(y>=330 && y<=355)
    return 8;
  else if(y>=620 && y<=645)
    return 9;
  else if(y>=300 && y<=320)
    return 0;
  else if(y>=220 && y<=235)
    return '*';     //'*'=42
  else if(y>=520 && y<=545)
  return '#';       //'#'=35 //enter is pressed returning ascii value of '#'
}
void insert(int item)
{
  if(full())
    return;
  if(front==-1)
    front=0;
  if(rear==MAX-1)
    rear=0;
  else
    rear=rear+1;
    queue[rear]=item;
}
int del(void)
{
  int item;
  if(!empty())
  {
    item=queue[front];
    if(front==rear)
      front=rear=-1;
    else if(front==MAX-1)
      front=0;
    else
      front=front+1;
    return item;
  }
}
int empty()
{
  if(front==-1)
  return 1;
  else
  return 0;
}
int full()
{
  if((front==0 && rear==MAX-1) || front==rear+1)
  return 1;
  else
  return 0;
}

