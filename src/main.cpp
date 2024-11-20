#include <Arduino.h>

#define out_STBY 5
#define out_B_PWM 3
#define out_A_PWM 11
#define out_A_IN2 9
#define out_A_IN1 10
#define out_B_IN1 8
#define out_B_IN2 7
#define motor_A 0
#define motor_B 1
#define motor_AB 2

void setup() {
  pinMode(out_STBY, OUTPUT);
  pinMode(out_A_PWM, OUTPUT);
  pinMode(out_A_IN1, OUTPUT);
  pinMode(out_A_IN2, OUTPUT);
  pinMode(out_B_PWM, OUTPUT);
  pinMode(out_B_IN1, OUTPUT);
  pinMode(out_B_IN2, OUTPUT);
}

void loop() {

}
