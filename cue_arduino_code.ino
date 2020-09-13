#include <dht.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

dht DHT;

//Define all pin variables to the Arduino
#define LedPin 2 //D2: LED (RED) shows Motion Sensor Motion Detected Light or Power ON LED
#define SensorPin 3 //D3: Motion Sensor Pin
#define TouchPin 4 //D4: Touch Sensor Module
#define NextProgramLedPin 5 //D5: LED (GREEN) shows when the next program is comming
#define DHT11_PIN 7 //D7: Temperature and Humidity Sensor Pin
#define SoilPin 8 //D8: Soil Moisture Sensor Pin 

//Declare integer variables
int sensorPin = A0; //For Motion Sensor
int sensorValue;  //For Motion Sensor
int limit = 300;  //For Soil Moisture Welcome

//Setting up C.U.E. display and pinMode to the Arduino
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  pinMode(SoilPin, OUTPUT);
  pinMode(SensorPin, INPUT);
  pinMode(TouchPin, INPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(NextProgramLedPin, OUTPUT);
}

//C.U.E. Main Menu
void loop() {
  digitalWrite(NextProgramLedPin, HIGH); //Powered ON Light
  digitalWrite(LedPin, HIGH);
  delay(500);
  digitalWrite(LedPin, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Welcome to CUE ");
  lcd.setCursor(0, 1);
  lcd.print(" Press to START ");

  //If Touch Panel pressed, it starts the program rotation modes
  int StartTouchValue = digitalRead(TouchPin);
  if (StartTouchValue == HIGH)
  {
    motion_sensor_start();  //Starts "Motion Sensor" mode
    soil_moisture_start();  //Starts "Soil Moisture" mode
    dht_start();      //Starts "DHT11" mode
    auto_mode();      //Rotates through all 3 sensors.
  }
}

void motion_sensor_start() {  //Starts Motion Sensor to record motion.
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("1. Motion Sensor");
  Serial.println("1. Motion Sensor");

  //LED (GREEN) next program mode was inititated.
  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);
  delay(1000);
  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);

  int StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  while (StartTouchValue != HIGH)
  {
    int sensorValue = digitalRead(SensorPin);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Motion Sensor: ");
    if (sensorValue == HIGH) //If there is motion, Motion was detected.
    {
      digitalWrite(LedPin, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Motion Detected");
      Serial.println("Motion Sensor: Motion Detected");
    }
    else //If there is no motion, motion was not detected.
    {
      digitalWrite(LedPin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("No Motion");
      Serial.println("Motion Sensor: No Motion");
    }

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  }
}

void soil_moisture_start() {  //Starts Soil Moisture Sensor to record data..
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("2. Soil Moisture");
  Serial.println("2. Soil Moisture");

  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);
  delay(1000);
  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);

  int StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  while (StartTouchValue != HIGH)
  {
    sensorValue = analogRead(sensorPin);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    //Soil Moisture Value
    lcd.setCursor(0, 0);
    lcd.print("Moisture Level: ");
    Serial.print("Moisture Level: ");
    lcd.setCursor(0, 1);
    lcd.print(sensorValue);     //display Moisture Level on LCD
    Serial.println(sensorValue);  //display Moisture Level on Serial Monitor

    //Soil Moisture Warning LED
    if (sensorValue < limit)
    {
      digitalWrite(SoilPin, HIGH);
    }
    else {
      digitalWrite(SoilPin, LOW);
    }

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  }
}

void dht_start() {  //Starts DHT11 Sensor to record data.
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("3. DHT ");
  Serial.println("3. DHT");

  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);
  delay(1000);
  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);

  int StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  while (StartTouchValue != HIGH)
  {
    int chk = DHT.read11(DHT11_PIN);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    //Temperature Sensor Value
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    Serial.print("Temp: ");
    lcd.print(DHT.temperature);
    Serial.print(DHT.temperature);
    lcd.print("*C ");
    Serial.print("*C ");

    //Humidity Sensor Value
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    Serial.print("Humidity: ");
    lcd.print(DHT.humidity);
    Serial.print(DHT.humidity);
    lcd.print("%");
    Serial.println("%");

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  }
}

void auto_mode() { //This mode shuffles through each sensor every 1 second in order.
  lcd.init();
  lcd.setCursor(0, 0);
  lcd.print("4. Auto");
  Serial.println("4. Auto");

  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);
  delay(1000);
  digitalWrite(NextProgramLedPin, LOW);
  delay(1000);
  digitalWrite(NextProgramLedPin, HIGH);

  int StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
  while (StartTouchValue != HIGH)
  {
    //Motion Sensor Code
    int sensorValue = digitalRead(SensorPin);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Motion Sensor: ");
    if (sensorValue == HIGH)
    {
      digitalWrite(LedPin, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Motion Detected");
      Serial.println("Motion Sensor: Motion Detected");
    }
    else
    {
      digitalWrite(LedPin, LOW);
      lcd.setCursor(0, 1);
      lcd.print("No Motion");
      Serial.println("Motion Sensor: No Motion");
    }

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
    if (StartTouchValue == HIGH)
    {
      break;
    }
    delay(1000);

    //Soil Moisture Code
    sensorValue = analogRead(sensorPin);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    //Soil Moisture Value
    lcd.setCursor(0, 0);
    lcd.print("Moisture Level: ");
    Serial.print("Moisture Level: ");
    lcd.setCursor(0, 1);
    lcd.print(sensorValue);
    Serial.println(sensorValue);

    //Soil Moisture Warning LED
    if (sensorValue < limit)
    {
      digitalWrite(SoilPin, HIGH);
    }
    else {
      digitalWrite(SoilPin, LOW);
    }

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
    if (StartTouchValue == HIGH)
    {
      break;
    }
    delay(1000);

    //DHT Sensor Value
    int chk = DHT.read11(DHT11_PIN);
    lcd.init(); // initialize the lcd
    lcd.backlight();

    //Temperature Sensor Value
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    Serial.print("Temp: ");
    lcd.print(DHT.temperature);
    Serial.print(DHT.temperature);
    lcd.print("*C ");
    Serial.print("*C ");

    //Humidity Sensor Value
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    Serial.print("Humidity: ");
    lcd.print(DHT.humidity);
    Serial.print(DHT.humidity);
    lcd.print("%");
    Serial.println("%");

    StartTouchValue = digitalRead(TouchPin); //Checks TouchValue if Low or High
    if (StartTouchValue == HIGH)
    {
      break;
    }
    delay(1000);
  }
}
