#include <SimpleDHT.h>
#include <Servo.h>

// Configuration struct
struct Configuration {
    int dhtPin;
    int waterPin;
    int servoPin;
    int dhtSampleRate;
    int valveOpenPosition;
    int valveClosePosition;
};

// Global configuration
Configuration config = {
    .dhtPin = 2,
    .waterPin = A3,
    .servoPin = 3,
    .dhtSampleRate = 1500,
    .valveOpenPosition = 90,
    .valveClosePosition = 45
};

// Function to log messages
void logMessage(const char* message) {
    Serial.println(message);
}

class DHTSensor {
  private:
    SimpleDHT11 dht;
    byte temperature;
    byte humidity;
  
  public:
    DHTSensor(int pin) : dht(pin) {
      logMessage("DHTSensor initialized.");
    }
  
    void refresh() {
      int err = SimpleDHTErrSuccess;
      if ((err = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        Serial.print("Read DHT11 failed, err="); Serial.println(SimpleDHTErrCode(err));
      } else {
        logMessage("DHT data refreshed.");
      }
    }

    byte getTemperature() const {
      return temperature;
    }

    byte getHumidity() const {
      return humidity;
    }
};

class WaterSensor {
  private:
    int pin;
    int waterLevel;
    
  public:
    WaterSensor(int pin) : pin(pin) {
      logMessage("WaterSensor initialized.");
    }
  
    void refresh() {
      waterLevel = analogRead(pin);
      logMessage("Water data refreshed.");
    }

    int getWaterLevel() const {
      return waterLevel;
    }
};

class WaterValve {
  private:
    Servo servo;
    int openPosition;
    int closePosition;
    
  public:
    WaterValve(int pin, int openPos, int closePos) : openPosition(openPos), closePosition(closePos) {
      servo.attach(pin);
      logMessage("WaterValve initialized.");
    }
    
    void open() {
      servo.write(openPosition);
      logMessage("Water valve opened.");
    }
    
    void close() {
      servo.write(closePosition);
      logMessage("Water valve closed.");
    }
};

DHTSensor dhtSensor(config.dhtPin);
WaterSensor waterSensor(config.waterPin);
WaterValve waterValve(config.servoPin, config.valveOpenPosition, config.valveClosePosition);

void setup() {
    Serial.begin(115200);
    logMessage("Setup completed.");
    waterValve.close();
}

void loop() {
    if (Serial.available() > 0 && Serial.read() == 'a') {
        logMessage("Received dispense command.");
        waterValve.open();
        delay(3000);
        waterValve.close();
    }

    dhtSensor.refresh();
    waterSensor.refresh();

    Serial.print('t'); Serial.println(dhtSensor.getTemperature());
    Serial.print('h'); Serial.println(dhtSensor.getHumidity());
    Serial.print('w'); Serial.println(waterSensor.getWaterLevel());
    
    delay(config.dhtSampleRate);
}
