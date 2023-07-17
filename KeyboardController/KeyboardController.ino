#include <Wire.h>
#include "Keyboard.h"

#define SLAVE_ADDR 0x15
#define SEPARATOR ";"

typedef struct {
  String value;
  //0 write, 1 press, 2 release, 3 press and release, 4 press and hold
  int action;
  int delay_micro_seconds;
  int times;
  String to_string(){
    return this->value + SEPARATOR + this->action +SEPARATOR+ this->delay_micro_seconds + SEPARATOR + this->times;
  }
} Message;

Message translate_massage(String message){
  Message msg;
  int index = message.indexOf(SEPARATOR);
  msg.value = message.substring(0, index);
  message = message.substring(index + 1);

  index = message.indexOf(SEPARATOR);
  msg.action = message.substring(0, index).toInt();
  message = message.substring(index + 1);

  index = message.indexOf(SEPARATOR);
  msg.delay_micro_seconds = message.substring(0, index).toInt();

  message = message.substring(index + 1);
  msg.times = message.toInt();
  return msg;
}

void proccess_message(Message msg){
  Serial.println("Processing message");
  for(int i = 0; i < msg.times; i++){
    for(int x = 0; x<msg.value.length();x++){
      Keyboard.write((int)msg.value[x]);
    }
    delay(msg.delay_micro_seconds);
  }
}

void setup(){
  Keyboard.begin();
  Keyboard.write(48);
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
  Message msg = translate_massage(message);
  proccess_message(msg);
  Serial.println("Event processed");
}