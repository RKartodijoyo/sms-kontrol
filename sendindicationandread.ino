/*
 * Complete Project Details http://randomnerdtutorials.com
 */

// Include Software Serial library to communicate with GSM
#include <SoftwareSerial.h>

// Configure software serial port
SoftwareSerial SIM900(7, 8);

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIDUP";

// Relay connected to pin 12
const int relay  = 2;
const int sensor = 3;
int kirimpesan;
void setup() {
  // Automatically turn on the shield
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);
  pinMode(sensor, INPUT);

  // By default the relay is off
  digitalWrite(relay, LOW);
  
  // Initializing serial commmunication
  Serial.begin(19200); 
  SIM900.begin(19200);

  // Give time to your GSM shield log on to network
  delay(20000);
  Serial.print("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop(){
  if(SIM900.available()>0){
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
  } 
  
    Serial.println(digitalRead(sensor));
    Serial.println(kirimpesan);
    Serial.println("");
  if(textMessage.indexOf("HIDUP")>=0){
    // Turn on relay and save current state
    delay(5000);
    digitalWrite(relay, HIGH);
    delay(5000);
    digitalWrite(relay, LOW);
    lampState = "hidup";
    Serial.println("LAMPU AKTIF");  
    textMessage = "";  
  }
  if(digitalRead(sensor)==1){
    delay(2000);
    kirimpesan=0;
    }
    else if(digitalRead(sensor)==0&&kirimpesan==1){delay(2000);}
    else if(digitalRead(sensor)==0&&kirimpesan==0){
      String message = "Mati Lampu ";
    sendSMS(message);
    textMessage = "";
        Serial.println("LAMPU SEDANG MATI");
        
    kirimpesan=1;
    delay(5000);
      }
    

  if(textMessage.indexOf("KEADAAN")>=0){
    String message = "Lampu dalam keadaan " + lampState;
    sendSMS(message);
    Serial.println("Lamp state resquest");
    textMessage = "";
  }
}  

// Function that sends SMS
void sendSMS(String message){
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+6281248265808\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);  
}
