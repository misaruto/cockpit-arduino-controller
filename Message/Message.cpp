#include "Message.h"

Message::Message(){
  this->message = "";
  this->value = "";
  this->action = 0;
  this->delay_micro_seconds = 0;
  this->times = 0;
}
Message::Message(String value, int action, int delay_micro_seconds, int times){
  this->value = value;
  this->action = action;
  this->delay_micro_seconds = delay_micro_seconds;
  this->times = times;
}
Message::Message(String message){
  this->message = message;
  this->translate_massage();
}
Message::~Message(){
}

String Message::get_value(){
  return this->value;
}

String Message::next_message_part(String default_value){
  int index = message.indexOf(SEPARATOR);
  String result = message.substring(0, index);
  this->message = message.substring(index + 1);
  if(result.equals("")){
    return default_value;
  }
  return result;
}

void Message::translate_massage(){
  int index = message.indexOf(SEPARATOR);
  this->value = message.substring(0, index);
  message = message.substring(index + 1);

  index = message.indexOf(SEPARATOR);
  this->action = message.substring(0, index).toInt();
  message = message.substring(index + 1);

  index = message.indexOf(SEPARATOR);
  this->delay_micro_seconds = message.substring(0, index).toInt();

  message = message.substring(index + 1);
  this->times = message.toInt();
}

String Message::to_string(){
  return this->value + SEPARATOR + this->action +SEPARATOR+ this->delay_micro_seconds + SEPARATOR + this->times;
}

void Message::proccess_message(void (*function)(Message)){
  for(int i = 0; i < this->times; i++){
    function(*this);
    delay(this->delay_micro_seconds);
  }
}