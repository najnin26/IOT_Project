#include <SoftwareSerial.h> 
SoftwareSerial SIM900(7, 8); // Pins 7, 8 are used as used as software serial pins

String incomingData;
char msg;
char call;

void setup()
{
  Serial.begin(115200); // baudrate for serial monitor
  SIM900.begin(115200); // baudrate for GSM shield
  
  Serial.println("GSM SIM900A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("s : to send message");
  Serial.println("c : to make a call");  
  Serial.println("l : to read location");
  Serial.println();
  delay(100);
  
  // set SMS mode to text mode
  SIM900.print("AT+CMGF=1\r");  
  delay(100);
  
  // set gsm module to show the output on serial out
  SIM900.print("AT+CNMI=2,2,0,0,0\r"); 
  delay(100);
  SIM900.println("AT+GPS=1");
  delay(1000);
 
}

void loop()
{
  
  receive_message();


if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'l':
      ReadLocation();
      break;
  }
       
}

void receive_message()
{
  if (SIM900.available() > 0)
  {
    incomingData = SIM900.readString(); // Get the data from the serial port.
    Serial.print(incomingData); 
    delay(10); 
  }
}

void SendMessage()
{
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"01733550789\"\r"); 
  delay(1000);
  SIM900.println("HELP!");// The SMS text 
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  SIM900.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  if (SIM900.available()>0)
  {
    msg=SIM900.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  SIM900.println("ATD01733550789");
  Serial.println("Calling  "); 
  delay(1000);
}

void HangupCall()
{
  SIM900.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReadLocation() {
 SIM900.println("AT+LOCATION=2");
 delay(1000);
 String response = "";
  while (SIM900.available())
  {
    char add = SIM900.read();
    response += add;
    delay(1);
  }

  int commaIndex = response.indexOf(",");
  String latitude = response.substring(17, commaIndex);
  String longitude = response.substring(commaIndex + 1, 38);

  Serial.print("Latitude: ");
  Serial.println(latitude);
  Serial.print("Longitude: ");
  Serial.println(longitude);
  
  String Gmaps_link = "http://maps.google.com/maps?q=" + latitude + "," + longitude;

  Message(Gmaps_link);
  Serial.println("Google Maps Link: " + Gmaps_link);
}


String Message(String Gmaps_link){
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"01733550789\"\r"); 
  delay(1000);
  SIM900.println(Gmaps_link);// The SMS text 
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
/*void ReadLocation() {
 SIM900.println("AT+GPS=1");
 delay(1000);
 SIM900.println("AT+GPSRD=5");
 delay(1000);
 SIM900.println("AT+LOCATION=2");
 delay(1000);
 SIM900.println("AT+GPSRD=0");
 delay(500);
}*/

