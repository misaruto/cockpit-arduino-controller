#include <Wire.h>
#include "Keyboard.h"
#include "../Message/Message.h"
#define SLAVE_ADDR 0x15
#define SEPARATOR ";"


void execute_keyboard_action(Message msg){
  Serial.println("Processing message");
  String value = msg.get_value();
  for(int x = 0; x<msg.value.length();x++){
    Keyboard.write((int)msg.value[x]);
  }
}

void setup(){
  Keyboard.begin();
  Wire.begin(SLAVE_ADDR);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
}

void loop(){
  delay(100);
  Serial.println("Waiting for message");
}

void receiveEvent(int howMany){
  Serial.println("Receiving event");
  String message = "";
  while(Wire.available()){
    char c = Wire.read(); // receive byte as a character
    message += c;
  }
  Message msg = new Message(message);
  msg.proccess_message(execute_keyboard_action);
  Serial.println("Event processed");
}