/*
  Analog input, analog output, serial output in binary, LED flashing
 */

#define MEASUREMENT_INTERVAL  20000

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin  = A9;          // analog input pin that the potentiometer is attached to
const int analogOutPin = A8;          //  analog output pin is simulated with PWM encoding
const int ledPin       = LED_BUILTIN; //

int sensorValue = 0;                  // value read
int outputValue = 0;                  // value output to the PWM (analog out)

unsigned long lastMeasurement = 0;
unsigned long now = 0;
byte* data;

void setup() {
  pinMode(analogOutPin, OUTPUT);
  pinMode(ledPin,       OUTPUT);
  pinMode(analogInPin,  INPUT);  
  digitalWriteFast(ledPin,0);
  while (!Serial && millis() < 3000) ;
  Serial.begin(Serial.baud());  
}

void loop() {
  now = micros();
  if ((now - lastMeasurement) >= MEASUREMENT_INTERVAL) {
    lastMeasurement = now;
    // read the analog in value:
    sensorValue = analogRead(analogInPin);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255); // input is 1024 different values and output is 256
    // change the analog out value:
    analogWrite(analogOutPin, outputValue);
    // Send data in binary over serial port
    data = (byte *) &now;
    Serial.write(data,sizeof(now));
    data = (byte *) &outputValue;
    Serial.write(data,sizeof(outputValue));
    Serial.write('\n');
    // Serial.flush();
    digitalWriteFast(ledPin,!digitalReadFast(ledPin));
  }
}
