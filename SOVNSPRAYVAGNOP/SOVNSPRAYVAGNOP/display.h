#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal.h>

const int rs = 32, en = 13, d4 = 14, d5 = 27, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setupDisplay() {
  // 16 columns, 2 rows
  lcd.begin(16, 2);
  lcd.clear();
}

void DisplayText(String message) {
  static String lastMessage = "";
  if (message != lastMessage) {
    lastMessage = message;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
  }
  
}
#endif