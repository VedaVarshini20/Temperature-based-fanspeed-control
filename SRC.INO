#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 7          // DHT11 sensor data pin
#define DHTTYPE DHT11     // DHT sensor type

#define MOTOR_PIN_ENA 9   // Enable pin for motor driver (PWM pin)
#define MOTOR_PIN_IN1 10  // Input pin 1 for motor driver
#define MOTOR_PIN_IN2 11  // Input pin 2 for motor driver

#define TEMPERATURE_THRESHOLD 29.80   // Temperature threshold to adjust motor speed
#define TEMPERATURE_THRESHOLD1 30.50   // Higher temperature threshold to adjust motor speed

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the I2C address if necessary

void setup() {
  Serial.begin(9600);
  dht.begin();

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("T");
  lcd.setCursor(1,0);
  lcd.print(":");
  lcd.setCursor(0, 1);
  lcd.print("H");
  lcd.setCursor(1, 1);
  lcd.print(":");
 // Move cursor to position 2 on the second line

}

void loop() {
  delay(1000); // Delay between readings (adjust as needed)
  float h = dht.readHumidity();
  float temperature = dht.readTemperature();
// Read temperature in Celsius
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  } 
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print("  %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("  °C");

  // Clear the previous motor speed

  // Adjust motor speed based on temperature
  if (temperature > TEMPERATURE_THRESHOLD1) {
    // Increase motor speed
    analogWrite(MOTOR_PIN_ENA, 255);    // Set the motor speed to maximum (255)
    digitalWrite(MOTOR_PIN_IN1, HIGH);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    
  }
  else if (temperature > TEMPERATURE_THRESHOLD) {
    // Increase motor speed
    analogWrite(MOTOR_PIN_ENA, 50);    // Set the motor speed to a value (150)
    digitalWrite(MOTOR_PIN_IN1, HIGH);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    
  }
  else {
    // Decrease motor speed
    analogWrite(MOTOR_PIN_ENA, 0);    // Set the motor speed to a lower value (150)
    digitalWrite(MOTOR_PIN_IN1, LOW);  // Set motor direction (forward)
    digitalWrite(MOTOR_PIN_IN2, LOW);
    
  }

  lcd.setCursor(2, 0);
   char tempStr[5];
sprintf(tempStr, "%02d", (int)temperature);
lcd.print(tempStr);// Set cursor to the temperature position on the first line
  lcd.print(temperature,2);

  // Print temperature
  lcd.setCursor(2,1);
  char humStr[5];
sprintf(humStr, "%02d", (int) h);
lcd.print(humStr);
  lcd.print(h,2);// print humidity
  lcd.setCursor(0, 1); // Set cursor to the second line
lcd.print("H:");
lcd.setCursor(2, 1); // Move cursor to position 2 on the second line

  
}