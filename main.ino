#include <SimpleDHT.h>
#include <Servo.h>

class DHTSensor {
  private:
    SimpleDHT11 dht;
    byte temperature;
    byte humidity;
  
  public:
    DHTSensor(int pin) : dht(pin) {}
  
    void refresh() {
      int err = SimpleDHTErrSuccess;
      if ((err = dht.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
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
    WaterSensor(int pin) : pin(pin) {}
  
    void refresh() {
      waterLevel = analogRead(pin);
    }

    int getWaterLevel() const {
      return waterLevel;
    }
};

class WaterValve {
  private:
    Servo servo;
    
  public:
    WaterValve(int pin) {
      servo.attach(pin);
    }
    
    void open() {
      servo.write(90);
    }
    
    void close() {
      servo.write(45);
    }
};

const int DHT_PIN = 2;
const int WATER_PIN = A3;
const int SERVO_PIN = 3;
const int DHT_SAMPLE_RATE = 1500;

DHTSensor dhtSensor(DHT_PIN);
WaterSensor waterSensor(WATER_PIN);
WaterValve waterValve(SERVO_PIN);

void setup() {
  Serial.begin(115200);
  waterValve.close();
}

void loop() {
  if (Serial.available() > 0 && Serial.read() == 'a') {
    waterValve.open();
    delay(3000);
    waterValve.close();
  }

  dhtSensor.refresh();
  waterSensor.refresh();

  Serial.print('t'); Serial.println(dhtSensor.getTemperature());
  Serial.print('h'); Serial.println(dhtSensor.getHumidity());
  Serial.print('w'); Serial.println(waterSensor.getWaterLevel());
  
  delay(DHT_SAMPLE_RATE);
}
