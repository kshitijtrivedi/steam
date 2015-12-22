//jai mata di 
#include <SPI.h>
#include<Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);


const byte mac[] = { 0x00, 0xC3, 0xA2, 0xE6, 0x3D, 0x57 };
byte ip[] = { 192, 168, 0, 177 };
float tempc;
int state = 0, mode = 1, k, flag=0,flag2=0,flag3=1;
unsigned long m,l,interval,initime,draintime = 180000;
int x,y,temp,wl,currentemp;
char a,b,c,d,e,f;

EthernetServer myServer(80);

void setup() {
  pinMode(5, OUTPUT);//steam
  pinMode(0, INPUT);//steam
  pinMode(3, OUTPUT);//heater 1
  pinMode(6, OUTPUT);//light
  pinMode(7, OUTPUT);//fan
  pinMode(8, OUTPUT);//heater 2 
  pinMode(9, OUTPUT);//heater 3 
  pinMode(4, OUTPUT);//drain is on
  
  Serial.begin(9600);
  while (!Serial);

  
  
  Serial.println("Establishing network connection...");
  
  Ethernet.begin((uint8_t*)mac, (uint8_t*)ip);   

  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
    
  Serial.print("Default Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  
  Serial.print("Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());
    
  Serial.print("DNS Server: ");
  Serial.println(Ethernet.dnsServerIP());
 
  myServer.begin(); 
  sensors.begin();
}


void loop() {
 
  
 // Serial.println(millis());
  EthernetClient client = myServer.available();
  if (client)
  {
    Serial.println("Incoming connection...");
  }
  String readString = "";
      while (client.connected()) {
        
        if(client.available() > 0) {
          char c = client.read();
          if (readString.length() < 100) {
   
//store characters to string
readString += c;
}
//if  HTTP request has ended
if (c == '\n') 
{
Serial.println("HTTP request ended");
Serial.print("readString = ");
Serial.println(readString);
a = readString[5];
b = readString[6];
c = readString[7];
d = readString[8];
e = readString[9];
f = readString[10];


if ( a == 'A' && b == 'L' && c == 'I' && d  == 'V' && e == 'E') 
{
  k=6; //app is checking if device is connected or not
}

if ( a == 'O' && b == 'N') 
{
  state=1; k =0;
}

if ( a == 'P' && b == 'O' && c == 'W' && d  == 'O' && e == 'F' && f == 'F')
 {
    k=4;
    state = 0;

 }

if(state == 1)
 {
  if ( a == 'L' && b == 'I' && c == 'G' && d  == 'H' && e == 'T')
   {
    k=1;
    if ( f == '0' )
    {
      digitalWrite(6,LOW);
    }
    if (f == '1' )
    {
      digitalWrite(6,HIGH);
    }
   }

   if ( a == 'F' && b == 'A' && c == 'N')
   { 
    k=2;
    if( d == '0' )
    {
      digitalWrite(7,LOW);
    }

    if( d == '1')
    {
      digitalWrite(7,HIGH);
    }
    
   }

if ( a == 'c' && b == 'u' && c == 'r' && d  == 'r' && e == 't' && f == 'e')
 {
    k=8;
  }

     
 if( a == 'S' )
    {
     k=3;
      if(b == '0')
      mode=1;
       if(b == '1')
      mode=2;
       if(b == '2')
      mode=3;


     x = c - '0'; 
     y = d - '0';
     temp = x*10 + y; // calculating temperature


     l = e - '0';  
     m = f - '0';
     interval = l*10 + m; // calculating interval
     digitalWrite(5,HIGH);
     digitalWrite(4,LOW);
     
     initime = millis();
     flag = 1;
     flag2 = 1;
    }

    if ( a == 'f' && b == 'u' && c=='c'  && d=='k')
{
  k=5;
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(3,LOW);
    flag = 0;
  
}

  
}
Serial.println(k);
client.println("HTTP/1.1 200 OK");
      
        client.println();
        
        if (k==0)
        client.print("omega");
        if (k==1)
        client.print("light");
        if(k==2)
        client.print("fan");
        if(k==3)
        client.print("steam");
         if(k==4)
        client.print("off");
        if(k==5)
        client.print("steam");
        if(k==6)
        client.print("alive");
        if(k==8)
        {
          String thisstring = String(currentemp);
          client.println("currte"+thisstring);
          Serial.println(thisstring);
          
        }
        
        
  
       client.println();
       delay(1);
       client.stop();
}
      }
      }
if(state == 0)
{
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(3,LOW);
    digitalWrite(6,LOW);
    digitalWrite(7,LOW);
    digitalWrite(4,LOW);
    flag = 0;
} 

 sensors.requestTemperatures();
 tempc = sensors.getTempCByIndex(0);
 currentemp = (int)tempc;
 if( (currentemp > (temp+3)) && flag3 == 1 )
 {
      digitalWrite(3,LOW);
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      flag = 0;
      flag3 =0;
 }

 if(currentemp < (temp-1) && flag3==0)
 {
  flag = 1;
  flag3 = 1;
 }
 
 

 if (((unsigned long)(millis() - initime) >= (interval*60000)) && flag2==1)
 {
 
    digitalWrite(5,LOW);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(3,LOW);
    if((unsigned long)(millis() - initime - (interval*60000)) < 180000)
    {
    digitalWrite(4,HIGH);
    flag = 0; 
    }
    else
    {
      digitalWrite(4,LOW);
      flag2 = 0;
    } 
 }

 wl = digitalRead(0);
if (wl == LOW && (currentemp < temp) )
{
  if(flag == 1)
  {
  if ( mode == 1)
  {
    digitalWrite(3,HIGH);
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
  }
   if ( mode == 2)
  {
    digitalWrite(3,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(9,LOW);
  }
   if ( mode == 3)
  {
    digitalWrite(3,HIGH);
    digitalWrite(8,HIGH);
    digitalWrite(9,HIGH);
  }

  }
}
if (wl == HIGH)
{ 
  digitalWrite(3,LOW);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW );
   
}
}


 

 


  





