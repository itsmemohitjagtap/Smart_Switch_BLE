
#include "BluetoothSerial.h" 
#include <AceButton.h>
using namespace ace_button;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run
`make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define RelayPin1 23  //D23
#define RelayPin2 22  //D22


#define SwitchPin1 13  //D13
#define SwitchPin2 12  //D12


int toggleState_1 = 1;
int toggleState_2 = 1; 

char bt_data; 

ButtonConfig config1;
AceButton button1(&config1);
ButtonConfig config2;
AceButton button2(&config2);



void handleEvent1(AceButton*, uint8_t, uint8_t);
void handleEvent2(AceButton*, uint8_t, uint8_t);

void all_Switch_ON(){
  digitalWrite(RelayPin1, LOW); toggleState_1 = 0; delay(100);
  digitalWrite(RelayPin2, LOW); toggleState_2 = 0; delay(100);
}

void all_Switch_OFF(){
  digitalWrite(RelayPin1, HIGH); toggleState_1 = 1; delay(100);
  digitalWrite(RelayPin2, HIGH); toggleState_2 = 1; delay(100);
  
}

void Bluetooth_handle()
{
  bt_data = SerialBT.read();
//  Serial.println(bt_data);
  delay(20);

  switch(bt_data)
      {
        case 'A': digitalWrite(RelayPin1, LOW);  toggleState_1 = 0; break; // if 'A' received Turn on Relay1
        case 'a': digitalWrite(RelayPin1, HIGH); toggleState_1 = 1; break; // if 'a' received Turn off Relay1
        case 'B': digitalWrite(RelayPin2, LOW);  toggleState_2 = 0; break; // if 'B' received Turn on Relay2
        case 'b': digitalWrite(RelayPin2, HIGH); toggleState_2 = 1; break; // if 'b' received Turn off Relay2
        case 'Z': all_Switch_ON(); break;  // if 'Z' received Turn on all Relays
        case 'z': all_Switch_OFF(); break; // if 'z' received Turn off all Relays
        default : break;
      }
}

void setup()
{
  Serial.begin(9600);

  btStart();  //Serial.println("Bluetooth On");
  
  SerialBT.begin("ESP32_BT"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  delay(5000);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  
//  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  
  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
 

  config1.setEventHandler(button1Handler);
  config2.setEventHandler(button2Handler);
  

  button1.init(SwitchPin1);
  button2.init(SwitchPin2);
 
  delay(200);
}

void loop()
{  
  if (SerialBT.available()){
   Bluetooth_handle();
 }

  button1.check();
  button2.check();
  
}

void button1Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT1");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_1 = 0;
      digitalWrite(RelayPin1, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_1 = 1;
      digitalWrite(RelayPin1, HIGH);
      break;
  }
}

void button2Handler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  Serial.println("EVENT2");
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println("kEventPressed");
      toggleState_2 = 0;
      digitalWrite(RelayPin2, LOW);
      break;
    case AceButton::kEventReleased:
      Serial.println("kEventReleased");
      toggleState_2 = 1;
      digitalWrite(RelayPin2, HIGH);
      break;
  }
}