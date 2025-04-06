#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2) ;
DHT dht (2, DHT11);
                // г  х  с
int time_pump[] = {0, 0, 30};
int counter_time = 0;
int time_now = 0;
unsigned long prevAction;

//Функція виводу таймеру поливу на дисплей
void lcd_time(){
  prevAction = millis();
  
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("TIME");
  lcd.setCursor(0,1);
  lcd.print(time_pump[0]);
  lcd.print(":");
  lcd.print(time_pump[1]);
  lcd.print(":");
  lcd.print(time_pump[2]);
}
//Функція виводу температури та вологості на дисплей
void lcd_sensor() {
  lcd.setCursor(12,0);
  lcd.print("h:");
  lcd.print(dht.readHumidity());
  lcd.setCursor(12,1);
  lcd.print("t:");
  lcd.print(dht.readTemperature());
}

//Налаштування та вступний екран
void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
  lcd.setCursor(1,1);
  lcd.print("King of plants");
  delay(2000);
  lcd.clear();
  lcd_time();

  prevAction=millis();
}

//Зчитування конпок та керування насосом
void loop() {
  lcd_sensor();
    
  //Таймер запуску помпи
  if(millis() - prevAction >= time_pump[0]*60*60*1000 + time_pump[1]*60*1000 + time_pump[2]*1000) {
    digitalWrite(13,LOW);
    delay(5000);
    digitalWrite(13,HIGH);
    prevAction = millis();
  }
  //Кнопка зміни часу, збільшення часу
  if(digitalRead(8) == HIGH) {
    time_now++;

    if (counter_time == 0){
      if(time_now > 167) {
        time_now = 0;
      }
    } else {
      if(time_now > 59) {
          time_now = 0;
      }
    }
    time_pump[counter_time] = time_now;
    lcd_time();
    delay(150);
  }
  //Кнопка зміни часу, зменшення часу
  if(digitalRead(9) == HIGH) {
    time_now--;

    if (counter_time == 0){
      if(time_now < 0) {
        time_now = 167;
      }
    } else {
      if(time_now < 0) {
          time_now = 59;
      }
    }
    time_pump[counter_time] = time_now;
    lcd_time();
    delay(150);
  }
  //Кнопка переключення, який час змінювати години, хвилини чи секунди
  if(digitalRead(10) == HIGH) {
    time_now = 0;
    counter_time++;
    
    if(counter_time > 2) {
      counter_time = 0;
    }
    time_now = time_pump[counter_time];
    Serial.println((time_pump[0]*60*60*1000) + (time_pump[1]*60*1000) + (time_pump[2]*1000));
    delay(150);
  }
}