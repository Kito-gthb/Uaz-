#include <OLED_I2C.h> // подключаем бибилиотку OLED дисплея

OLED  myOLED(8, 9, 8); // SDA - 8pin, SCL - 9pin, пины подулючения дисплея

extern uint8_t akb[]; // иконка "Аккумулятор"
extern uint8_t uaz[]; // логотип используемый при запуске
extern uint8_t SmallFont[]; // шрифт латинского текста
extern uint8_t RusFont[]; // шрифт русского текста
extern uint8_t MediumNumbers[]; // шрифт цифр показаний

int voltPin = A0;  // пин подключенный к +АКБ
float voltage = 0; // переменная вольтметра
int x, y, z = 0; // переменные графика тренда
int BuzPin = 10; // пин подключенный к бузеру
int BrighPin = 11; // пин регулировки яркости дисплея

void setup() {
  pinMode(BuzPin, OUTPUT); // задаем 10 пин бузера как выход
  pinMode(BrighPin, INPUT); // задаем 11 пин регулятора подсетки дисплея как вход
  myOLED.begin(); // подключаем дисплей
  myOLED.clrScr(); // стираем все с дисплея
  startup();
}

void loop() {
  brigh();
  voltmeter();
  alarm(); 
}

void voltmeter() {
  voltage = float(analogRead(voltPin))/68,2; // считываем значение на пине вольтметра, расчет произведен на предел значений 0-15В. 1023/15=68,2
  y = map(voltage, 10, 15, 63, 43); // задаем пределы значений для графика тренда. Где voltage-значение напряжения, 10-минимальное напряжение, 15-максимальное напряжение, 63-начальная координата миниамльного значения напряжения, 43-конечная координата максимального значения напряжения
  myOLED.setFont(RusFont); // выбираем русский шрифт
  myOLED.print("DJKMNVTNH", CENTER, 0); // пишем заголовок "ВОЛЬТМЕТР"
  myOLED.setFont(MediumNumbers); // выбираем шрифт цифр показаний
  myOLED.print(String(voltage), 30, 15); // выводим значение напряжения
  myOLED.setFont(SmallFont); // выбираем маленький шрифт
  myOLED.print("Volt", 90, 24); // пишем "Volt" после значания напряжения
  myOLED.drawBitmap(9, 17, akb, 18, 15); // выводим иконку "Аккумулятор"
  myOLED.drawLine(x, z, x+1, y); // рисуем линию тренда по заданным координатам со сдвигом по координате "X"
  myOLED.update(); // обновляем информацию на дисплее
  z=y; 
  x++; // за каждый цикл к линии добавляем новую с новыми значениями
  if (x >= 127) { // создаем условие. Если значение координаты "X" достигло 127, то
    x = 0; // начинаем рисовать график с начальной координаты
    myOLED.clrScr(); // и стираем все с дисплея
  }
}

void alarm () { 
  if (voltage <= 11.5) { // создаем уловие. Если значение напряжения упало ниже 12.5, то
    tone (BuzPin, 1915); // пищим бузером с чатотой 1915Гц
    myOLED.setBrightness(1); // уменьшаем подсветку дисплея
    delay(100); // ждем 100 Мс
    tone(BuzPin, 1700); // пищим бузером с чатотой 1700Гц
    myOLED.setBrightness(255); // увеличиваем подсетку
    delay(100); // ждем 100 Мс
    noTone(BuzPin); // отключаем бузер
    myOLED.setFont(RusFont); // выбираем русский шрифт
    myOLED.print("YBPRJT YFGHZ:TYBT", CENTER, 35);  // выводим ошибку "НИЗКОЕ НАПРЯЖЕНИЕ"
  }
}

void startup () {
  myOLED.clrScr(); // стираем все с дисплея
  myOLED.drawBitmap(0, 0, uaz, 128, 51);
  myOLED.setFont(SmallFont); // выбираем русский шрифт
  myOLED.print("by VjWhite", LEFT, 56);
  myOLED.print("v1.0", RIGHT, 56);
  myOLED.update(); // обновляем информацию на дисплее
  delay(3000); // задержка времени для показа логотипа
  myOLED.clrScr(); // стираем все с дисплея
  myOLED.setFont(RusFont); // выбираем русский шрифт
  myOLED.print("GJLGBIBCM", CENTER, 8); // пишем заголовок "ПОДПИШИСЬ"
  myOLED.print("VJQ RFYFK YF >NE<T", CENTER, 20); // пишем заголовок "МОЙ КАНАЛ НА ЮТУБЕ"
  myOLED.print("VFKTYMRBQ GFZKMYBR", CENTER, 40); // пишем заголовок "МАЛЕНЬКИЙ ПАЯЛЬНИК"
  myOLED.update(); // обновляем информацию на дисплее
  delay(5000); // задержка времени для показа логотипа
  myOLED.clrScr(); // стираем все с дисплея
}

void brigh () {
  if (digitalRead(BrighPin) == HIGH) { // создаем условие для притухания подсетки дисплея при включении ближнего света. Если 12В с индикатора ближнего света приходит на 11 пин, то
    myOLED.setBrightness(1);  // уменьшаем подсветку дисплея
  }
  else { // если 12В не приходит, то
    myOLED.setBrightness(255); // оставляем яркость на максимальном уровне
  }
}

