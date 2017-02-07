#include <avr/pgmspace.h>
#include <LiquidCrystal.h>

const uint8_t relay_output                = 9;
const uint8_t button_output               = 8;
const uint8_t button_input                = 7;
const uint8_t time_potentiometer          = A0;
const PROGMEM char set_time_string[]      = "UV Exposure Time";
const PROGMEM char exposure_string[]      = "! UV Exposure !";

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
uint16_t timer = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  pinMode(relay_output, OUTPUT);
  digitalWrite(relay_output, HIGH);
  pinMode(button_input, INPUT_PULLUP);
  pinMode(button_output, OUTPUT);
  digitalWrite(button_output, LOW);
}

void update_timer() {
  timer = analogRead(time_potentiometer) * .6;
}

void progmem_lcd_print(const char *str) {
  char c = 0;
  while ((c = (char)pgm_read_byte(str++)))
    lcd.print(c);
}

void print_remaining_time() {
  lcd.setCursor(11, 1);
  lcd.print(timer);
  lcd.setCursor(15, 1);
  lcd.print("s");
}

void do_exposure() {
  lcd.setCursor(0, 0);
  progmem_lcd_print(exposure_string);
  //lcd.print(exposure_string);
  digitalWrite(relay_output, LOW);
  while (timer) {
    print_remaining_time();
    timer--;
    delay(1000);
  }
  digitalWrite(relay_output, HIGH);
}

void print_exposure_settings() {
  lcd.setCursor(0, 0);
  progmem_lcd_print(set_time_string);
  //lcd.print(set_time_string);
  print_remaining_time();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  update_timer();
  if (digitalRead(button_input) == LOW)
    do_exposure();
  else
    print_exposure_settings();
  delay(100);
}
