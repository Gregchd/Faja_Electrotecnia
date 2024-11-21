#include <Arduino.h>

// Pines del TB6612
const int pwmPin = 26;  // Pin para PWMA o PWMB
const int in1Pin = 12;  // Pin para AIN1 o BIN1
const int in2Pin = 14;  // Pin para AIN2 o BIN2
const int stbyPin = 27; // Pin para STBY (Standby)

// Configuración PWM
const int pwmChannel = 0;     // Canal PWM
const int freq = 5000;        // Frecuencia en Hz
const int resolution = 8;     // Resolución de 8 bits (0-255)
const int maxDutyCycle = 255; // Máximo ciclo de trabajo (velocidad máxima)

// Función para controlar el motor
void setMotor(bool forward, int speed)
{
  // Configurar dirección
  if (forward)
  {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  }
  else
  {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  }

  // Configurar velocidad (PWM)
  ledcWrite(pwmChannel, speed);
}

void setup()
{
  // Configurar los pines digitales
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(stbyPin, OUTPUT);

  // Configuración del PWM
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(pwmPin, pwmChannel);

  // Activar el controlador
  digitalWrite(stbyPin, HIGH);

  // Configurar la dirección y velocidad inicial
  setMotor(true, maxDutyCycle); // Dirección hacia adelante, máxima velocidad
}

void loop()
{
  // Ejemplo: Cambiar dirección y velocidad
  setMotor(false, maxDutyCycle); // Dirección hacia atrás, velocidad media
  delay(3000);
  setMotor(true, maxDutyCycle); // Dirección hacia adelante, máxima velocidad
  delay(3000);
}