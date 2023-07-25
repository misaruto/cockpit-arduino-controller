#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDR  0x27
#define LCD_ROWS  4
#define LCD_COLS  16

#define SLAVE_ADDR 0x15
#define SEPARATOR ";"

const int TOTAL_BUTTONS = 5;

typedef struct {
  String value;
  //0 write, 1 press, 2 release, 3 press and release, 4 press and hold
  int action = 0;
  int delay_micro_seconds =0;
  int times = 1;
  String to_string(){
    return this->value + SEPARATOR + this->action +SEPARATOR+ this->delay_micro_seconds + SEPARATOR + this->times;
  }
} Message;


typedef struct Button {
  int pin;
  int action;
  String button_name;
  String value;
  int last_pressed_state = LOW;
  bool state_changed(){
    bool current_state = read();
    bool changed = current_state != this->last_pressed_state;
    this->last_pressed_state = current_state;
    return changed;
  }
  void initialize(){
    if(this->pin == 0)
      return;
    pinMode(this->pin, INPUT);
  }
  int read(){
    return digitalRead(this->pin);
  }
  bool in_pressed(){
    return this->read() == HIGH;
  }
}Button;

LiquidCrystal_I2C lcd(LCD_ADDR,LCD_COLS,LCD_ROWS);
Button ls_button[TOTAL_BUTTONS];
void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS); 
  lcd.backlight();
  write_lcd("Iniciando...");
  Wire.begin();
  Serial.begin(9600);
  initialize();
  write_lcd("Pronto!");
}
void loop(){
  read_buttons();
}

void write_lcd(String message){
  lcd.clear();
  lcd.setCursor(0,0);
  Serial.println(message);
  Serial.println(message.length());
  if(message.length() > LCD_COLS){
    String temp = message.substring(0, LCD_COLS);
    int index = temp.lastIndexOf(" ");
    if(index > 0){
      temp = temp.substring(0, index);
    }else{
      index = LCD_COLS;
    }
    lcd.print(temp);
    lcd.setCursor(0,1);
    lcd.print(message.substring(index+1, LCD_COLS*2));
  }else{  
    lcd.print(message);
  }
}

Button create_button(String button_name, int pin,int action, String value){
  Button button;
  button.button_name = button_name;
  button.pin = pin;
  button.value = value;
  button.action = action;
  return button;
}

void prepare_buttons(){
  Serial.println("Preparing buttons");
  ls_button[0] = create_button("Reboque", 53,0, "t");
  ls_button[1] = create_button("Diferencial", 52,0, "v");
  ls_button[2] = create_button("Retarder +", 51,0, "j");
  ls_button[3] = create_button("Retarder - ", 50,0, "k");
  ls_button[4] = create_button("Limpador", 49,0, "p");
  Serial.println("Buttons prepared");
}
void initialize_buttons(){
  Serial.println("Initializing buttons");
  for(int i = 0; i < TOTAL_BUTTONS; i++){
    ls_button[i].initialize();
  }
  Serial.println("Buttons initialized");
}

void initialize(){
  prepare_buttons();
  initialize_buttons();
}

void read_buttons(){
  for(int i = 0; i < TOTAL_BUTTONS; i++){
    if(ls_button[i].state_changed()){
      execute_action(ls_button[i]);
      delay(300);
    }
  }
}

void execute_action(Button button){
  Message message;
  message.value = button.value;
  message.action = button.action;
  write_lcd(button.button_name + " Ativado");
  Serial.println(message.to_string());
  write_message(message);
}

void write_message(Message message){
  Serial.println("Starting transmission");
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(message.to_string().c_str());
  Wire.endTransmission();
  Serial.println("Transmission ended");
}
