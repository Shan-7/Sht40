#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_SHT4x.h>

// Data wire for DS18B20 sensors is connected to digital pin 2
#define ONE_WIRE_BUS 2

// I2C Multiplexer TCA9548A address
#define TCA9548A_ADDRESS 0x70

// Setup a OneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to Dallas Temperature.
DallasTemperature ds18b20(&oneWire);

// Initialize the SHT40 sensors
Adafruit_SHT4x sht40_1 = Adafruit_SHT4x();  // SHT40 sensor on I2C channel 0
Adafruit_SHT4x sht40_2 = Adafruit_SHT4x();  // SHT40 sensor on I2C channel 1
Adafruit_SHT4x sht40_3 = Adafruit_SHT4x();  // SHT40 sensor on I2C channel 2

// Array to store temperatures from DS18B20 sensors
float ds18b20Temps[7];

// Function to select the TCA9548A multiplexer channel
void tcaSelect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(TCA9548A_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  Wire.begin();

  // Start up the DS18B20 library
  ds18b20.begin();

  // Initialize the first SHT40 sensor (I2C channel 0)
  tcaSelect(0);
  if (!sht40_1.begin()) {
    Serial.println("Couldn't find SHT40 sensor 1!");
    while (1) delay(10);
  }
  Serial.println("SHT40 sensor 1 found");

  // Initialize the second SHT40 sensor (I2C channel 1)
  tcaSelect(1);
  if (!sht40_2.begin()) {
    Serial.println("Couldn't find SHT40 sensor 2!");
    while (1) delay(10);
  }
  Serial.println("SHT40 sensor 2 found");

  // Initialize the third SHT40 sensor (I2C channel 2)
  tcaSelect(2);
  if (!sht40_3.begin()) {
    Serial.println("Couldn't find SHT40 sensor 3!");
    while (1) delay(10);
  }
  Serial.println("SHT40 sensor 3 found");
}

void loop() {
  // Request temperature readings from all DS18B20 sensors
  ds18b20.requestTemperatures();
  
  // Store the temperature readings in the array
  for (int i = 0; i < 7; i++) {
    ds18b20Temps[i] = ds18b20.getTempCByIndex(i);
  }

  // Print DS18B20 temperatures
  for (int i = 0; i < 7; i++) {
    Serial.print("DS18B20 Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(ds18b20Temps[i]);
    Serial.println(" °C");
  }

  // Read data from the first SHT40 sensor (I2C channel 0)
  tcaSelect(0);
  sensors_event_t humidity1, temp1;
  sht40_1.getEvent(&humidity1, &temp1);
  
  // Print SHT40 sensor 1 data
  Serial.print("SHT40 Sensor 1 Temperature: ");
  Serial.print(temp1.temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity1.relative_humidity);
  Serial.println(" %");

  // Read data from the second SHT40 sensor (I2C channel 1)
  tcaSelect(1);
  sensors_event_t humidity2, temp2;
  sht40_2.getEvent(&humidity2, &temp2);

  // Print SHT40 sensor 2 data
  Serial.print("SHT40 Sensor 2 Temperature: ");
  Serial.print(temp2.temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity2.relative_humidity);
  Serial.println(" %");

  // Read data from the third SHT40 sensor (I2C channel 2)
  tcaSelect(2);
  sensors_event_t humidity3, temp3;
  sht40_3.getEvent(&humidity3, &temp3);

  // Print SHT40 sensor 3 data
  Serial.print("SHT40 Sensor 3 Temperature: ");
  Serial.print(temp3.temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity3.relative_humidity);
  Serial.println(" %");

  // Wait before repeating the loop
  delay(15000);  // 15 seconds delay
}
