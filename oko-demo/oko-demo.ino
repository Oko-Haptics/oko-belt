#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

int outputData[15]= { 0 };
int curr_index = 0;
char datafromUser[15];
const int decoder = 512;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.read();
  delay(500);
  pwm1.begin();
  pwm1.setPWMFreq(1600);
  delay(100);
  for (int s = 0; s <=15; s++){
    pwm1.setPWM(s, 0, 4096);
  }
}

void loop() {
  bool update_flag = false;
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (isDigit(c)) {
      if (c == '9') {
        curr_index = 0;
        while (Serial.available()) {
          Serial.read();
        }
        update_flag = true;
      }
      else {
        datafromUser[curr_index] = c;
        curr_index ++;
      }
    }
  }

  if (update_flag == true) {
    for (int i = 0; i < 15; i ++) {
      int test = (datafromUser[i] - '0')*decoder;
      pwm1.setPWM(i+1, 0, test);
    }
    delay(50);
    Serial.flush();
    delay(50);
  }
  delay(50);
}
