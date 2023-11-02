
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(D5,D6);
SoftwareSerial gsm(D2,D3);

String late;
String longe;
String txt1;
String txt2;
String link;

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
  gsm.begin(9600);
  delay(5000);
  Serial.println("Testing Sim800l");
  gsm.println("AT");
  delay(100);
  if(gsm.available())
  {
    Serial.println(gsm.readString());
  }
    gsm.println("AT+CMGF=1");           //AT+CNMI=1,2,0,0,0
    delay(200);
    gsm.println("AT+CNMI=1,2,0,0,0");   //AT+CNMI=1,2,0,0,0
    delay(200);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    late = String(gps.location.lat(),6);
    longe= String(gps.location.lng(),6);
    link = "http://maps.google.com/maps?q="+late+","+longe;
   
    Serial.println(late);
    Serial.println(F(","));
    Serial.println(longe);
   
  }
  else
  {
    Serial.print(F("INVALID"));
  }
  if(gsm.available())
  {
    txt1 = gsm.readString();
    Serial.println(txt1);

      String number = txt1.substring(9,22);
      Serial.println(number);
 if(number == "Your phone number")
 {    
    if(txt1.indexOf("Direction")!= -1)
    {
      gsm.println("AT+CMGS=\"Your phone number\"");
                delay(200);
                gsm.println(link);
                delay(200);
                gsm.write(26);
    }
 
  }
 }
 if(Serial.available())
  {
    txt2 = Serial.readString();
    
    gsm.println("AT+CMGS=\"Your phone number\"");  //Your phone number
    delay(200);
    gsm.println(txt2);
    delay(200);
    gsm.write(26);
  }
}
