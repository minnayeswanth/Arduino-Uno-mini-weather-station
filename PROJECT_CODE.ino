#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
LiquidCrystal lcd(13,12,11,10,9,8);
SoftwareSerial mySerial(2,3);//rx , tx
int ldr = A1;
int temp = A0;
int uv = A2;
int hum = A3;
int dust=A4;
char res[130];
void upload1(unsigned char *chr,unsigned char *chr1,unsigned char *chr2,unsigned char *chr3);

void myserialFlush(){
  while(mySerial.available() > 0) {
    char t = mySerial.read();
  }
} 
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
} 


char check(char* ex,int timeout)
{
  int i=0;
  int j = 0,k=0;
  while (1)
  {
    sl:
    if(mySerial.available() > 0)
    {
      res[i] = mySerial.read();
      if(res[i] == 0x0a || res[i]=='>' || i == 100)
      {
        i++;
        res[i] = 0;break;
      }
      i++;
    }
    j++;
    if(j == 30000)
    {
      k++;
      //Serial.println("kk");
      j = 0;  
    }
    if(k > timeout)
    {
     // Serial.println("timeout");
      return 1;
     }
  }//while 1
  if(!strncmp(ex,res,strlen(ex)))
  {
    //Serial.println("ok..");
    return 0;
   }
  else
  {
   // Serial.print("Wrong  ");
   // Serial.println(res);
    i=0;
    goto sl;
   }
} 
char buff[200];
int k=0;
const char* ssid = "project";
const char* password = "project1235";
int T;
int tt;

void setup(void)
{ 
  char ret;
  Serial.begin(9600);
  mySerial.begin(115200);
  lcd.begin(16,2);
  //pinMode(hbpin, INPUT);
  pinMode(A0,INPUT); //ecg  led+
  pinMode(A1,INPUT); //ecg  led-
    pinMode(A2,INPUT);
      pinMode(A3,INPUT);
        pinMode(A4,INPUT);
  delay(1000);
  lcd.clear();lcd.setCursor(0, 0);lcd.print("WELCOME");     
  delay(1000);
  st:
  mySerial.println("ATE0");
  Serial.println("ATE0");
  ret  = check((char*)"OK",50);
  mySerial.println("AT");
  Serial.println("AT");
  ret  = check((char*)"OK",50);
  if(ret != 0)
  {
    delay(1000);
   goto st; 
  }
  
  lcd.clear();lcd.setCursor(0, 0);lcd.print("CONNECTING");  
  mySerial.println("AT+CWMODE=1");
  Serial.println("AT+CWMODE=1");
   ret  = check((char*)"OK",50);
 cagain:
    
  myserialFlush();
  Serial.print("AT+CWJAP=\"");
  mySerial.print("AT+CWJAP=\"");
  mySerial.print(ssid);
  Serial.print(ssid);
  mySerial.print("\",\"");
  Serial.print("\",\"");
  mySerial.print(password);
  Serial.print(password);
  mySerial.println("\"");
  Serial.println("\"");
  if(check((char*)"OK",300))
  goto cagain;    
  mySerial.println("AT+CIPMUX=1");
  Serial.println("AT+CIPMUX=1");
  delay(1000);
  lcd.clear();lcd.setCursor(0, 0);lcd.print("WAITING");
  

}
int c=0;
//unsigned long int duration = 0;
//int hbeat=0;
void loop() 
{
   int td = digitalRead(temp)/10;
   Serial.println(td);
   int ld = digitalRead(ldr);
      int humd = digitalRead(hum);
   Serial.println(ld);
   int uvd = digitalRead(uv);
   Serial.println(uvd);
   int dd = digitalRead(dust);
   
    lcd.clear(); 
    lcd.setCursor(0, 0); lcd.print("T:");lcd.print(td);lcd.print("    ");
    lcd.setCursor(5, 0); lcd.print("H:");lcd.print(humd);lcd.print("    ");
    lcd.setCursor(10, 0); lcd.print("L:");lcd.print(ld);lcd.print("    ");
    lcd.setCursor(0, 1); lcd.print("UV:");lcd.print(uvd);lcd.print("    ");
    lcd.setCursor(0, 1); lcd.print("D:");lcd.print(dd);lcd.print("    ");
    delay(3000);
    
      if(td > 25)
     {
      lcd.clear();
      lcd.setCursor(0, 1);lcd.print("HIGH TEMP");
      upload1(td,ld,uvd,humd,dd);
      
      }
if(ld == LOW)
     {
      lcd.clear();
      lcd.setCursor(0, 1);lcd.print("DAY");
      upload1(td,ld,uvd,humd,dd);
      
      }
if(uvd == HIGH)
     {
      lcd.clear();
      lcd.setCursor(0, 1);lcd.print("UV HIGH");
      upload1(td,ld,uvd,humd,dd);
      
      }
        if(humd == LOW)
     {
      lcd.clear();
      lcd.setCursor(0, 1);lcd.print("HUM HIGH");
      upload1(td,ld,uvd,humd,dd);
      
      }
      if(dd == LOW)
     {
      lcd.clear();
      lcd.setCursor(0, 1);lcd.print(" DUST");
      upload1(td,ld,uvd,humd,dd);
      
      }    
}



char bf2[100];
void upload1(unsigned char *chr,unsigned char *chr1,unsigned char *chr2,unsigned char *chr3,unsigned char *chr4)
{
  delay(2000);
  lcd.clear();lcd.setCursor(0, 1);lcd.print("UPLOADING");
  myserialFlush();
  mySerial.println("AT+CIPSTART=4,\"TCP\",\"api.thingspeak.com\",80");
 // mySerial.println("AT+CIPSTART=4,\"TCP\",\"embeddedspot.top\",80");
      delay(8000);
      sprintf(buff,"GET https://api.thingspeak.com/update?api_key=OBYBAV3LM9BY8BUT&field1=0&field1=%u&field2=%u&field3=%u&field4=%u&field5=%u\r\n\r\n",chr,chr1,chr2,chr3,chr4);
    //  sprintf(buff,"GET http://embeddedspot.top/iot/storedata.php?name=sensors010&s1=%u&s2=%u&s3=%u&s4=%u\r\n\r\n",chr,chr1,chr2,chr3);
      myserialFlush();
      sprintf(bf2,"AT+CIPSEND=4,%u",strlen(buff));
      mySerial.println(bf2);
         delay(5000);
          myserialFlush();
          mySerial.print(buff);
          Serial.print(buff);
              delay(2000);
              
              mySerial.println("AT+CIPCLOSE");
              Serial.println("AT+CIPCLOSE");
              lcd.setCursor(0, 1);lcd.print("UPLOADED");   lcd.clear();

}
