#ifndef MESSAGE_H
#define MESSAGE_H
#include <Arduino.h>
#define SEPARATOR ";"
class Message{
private:
  String message;
  String value;
  int action;
  int delay_micro_seconds;
  int times;
  void translate_massage();
  String next_message_part(String message);
public:
  String to_string();
  void proccess_message(void (*function)(Message));
  String get_value();
  Message();
  Message(String value, int action, int delay_micro_seconds, int times);
  Message(String message);
  ~Message();
};
#endif