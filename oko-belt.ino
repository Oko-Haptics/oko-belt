#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


SoftwareSerial EspSerial(2, 3); // RX, TX
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

const int decode_multiplier = 512;

const String ssid = "\"wifi_name\"";
const String pwd = "\"pwd\"";

void setup() {
  Serial.begin(115200);
  delay(100);
  pwm1.begin();
  pwm1.setPWMFreq(1600);
  delay(50);
  pwm2.begin();
  pwm2.setPWMFreq(1600);
  delay(50);

  for (int s = 0; s <=15; s++){
    pwm1.setPWM(s, 0, 4096);
    delay(10);
    pwm2.setPWM(s, 0, 4096);
    delay(10);
  }
  EspSerial.begin(9600); // your esp's baud rate might be different 9600, 57600, 76800 or 115200
  delay(2000);
  get_info();
  delay(5000);
  set_wifi_mode(3);
  delay(5000);
  connect2wifi();
  delay(5000);
  multi_connect(1);
  delay(5000);
  setup_server(1, 80);
  delay(5000);
}

void get_info() {
  Serial.println("Reset Connection....");
  EspSerial.println("AT+RST");
}

void connect2wifi() {
  Serial.print("Trying to connect to: ");
  Serial.println(ssid);
  String cmd = "AT+CWJAP="+ssid+","+pwd;
  EspSerial.println(cmd);
}


void set_wifi_mode(int wmode) {
  String _wmode = "AT+CWMODE="+String(wmode);
  Serial.println(_wmode);
  EspSerial.println(_wmode);
}

void multi_connect(int flag) {
  String _mconnect = "AT+CIPMUX="+String(flag);
  Serial.println(_mconnect);
  EspSerial.println(_mconnect);
}


void setup_server(int setup, int port)
{
  String _setup = "AT+CIPSERVER="+String(setup)+","+String(port);
  Serial.println(_setup);
  EspSerial.println(_setup);
}

int decode_vibration (int pwm_dn) {
  return pwm_dn*decode_multiplier;
}

void vibrate_motor(int motor_num, int pwm_dn) {
  int target_pin;
  int pwm_out = decode_vibration(pwm_dn);
  if (motor_num <= 15) {
    target_pin = motor_num;
    pwm1.setPWM(target_pin, 0, 4096-pwm_out);
  }
  else {
    target_pin = motor_num - 16;
    pwm2.setPWM(target_pin, 0, 4096-pwm_out);
  }
}


void loop() {
  if(EspSerial.available()) // check if the ESP module is sending a message 
  {
    while(EspSerial.available())
    {
      char c = EspSerial.read(); // read the next character.
      Serial.write(c);
    }
  }
  
  if(Serial.available()) // check if connection through Serial Monitor from computer is available
  {
    delay(1000); 
    
    String command="";
    
    while(Serial.available()) // read the command character by character
    {
      command+=(char)Serial.read();
    }
    Serial.println("Trying to write command");
    Serial.println(command);
    EspSerial.println(command); // send the read character to the Esp module
  }
  delay(10);
}
