// External libraries
#include <SimpleDHT.h>
#include <Servo.h>

// Variables for what pin are the electronics connected to
#define pinWater A3
#define pinServo 3
#define pinDHT 2

// Initialising dht and servo libraries
SimpleDHT11 dht11(pinDHT);
Servo servo;

// Variables for storing useful data
byte temperature = 0;
byte humidity = 0;
int water = 0;
char userInput;

// Cannot cycle faster than dht sample rate, used as delay timer.
int dhtSampleRate = 1500;

// Initialises serial at correct baud rate and attaches servo to correct pin.
void setup() {
  Serial.begin(115200);
  servo.attach(pinServo);
}

void loop() {
  // Starts system with closed water
  closeWater();

  // Checks for signal from app to dispense water
  if (Serial.available() > 0) {
    if (Serial.read() == 'a') {
      openWater();
      delay(3000);
      closeWater();
    }
  }

  // Updates data variables with fresh data
  dhtRefresh();
  waterRefresh();

  // Pushes fresh data to serial for app to grab
  Serial.print('t'); Serial.println(temperature);
  Serial.print('h'); Serial.println(humidity);
  Serial.print('w'); Serial.println(water);
  
  // Delays next loop with dht sample rate cap
  delay(dhtSampleRate);
}

// Makes sure dht does not return an error, collects fresh data
void dhtRefresh() {
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
  }
}

// Collects fresh data from water sensor
void waterRefresh() {
  water = analogRead(pinWater);
}

// Triggers servo to close the air intake hole
void closeWater() {
  servo.write(45);
}

// Triggers servo to open air intake hole
void openWater() {
  servo.write(90);
}