#include <Arduino.h>

// Pines del TB6612
const int pwmPin = 26;  // Pin para PWMA o PWMB
const int in1Pin = 12;  // Pin para AIN1 o BIN1
const int in2Pin = 14;  // Pin para AIN2 o BIN2
const int stbyPin = 27; // Pin para STBY (Standby)

const int trigPin = 0; // Pin para el sensor de ultrasonido (TRIG)
const int echoPin = 4; // Pin para el sensor de ultrasonido (ECHO)

// Configuración PWM
const int pwmChannel = 0;     // Canal PWM
const int freq = 5000;        // Frecuencia en Hz
const int resolution = 8;     // Resolución de 8 bits (0-255)
const int maxDutyCycle = 255; // Máximo ciclo de trabajo (velocidad máxima)

const float SOUND_SPEED = 34300; // Velocidad del sonido en cm/s
const int MAX_DISTANCE = 400;    // Máxima distancia que se puede medir en cm

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

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

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
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo de rebote (en microsegundos)
  long duration = pulseIn(echoPin, HIGH, MAX_DISTANCE * 58);

  // Calcular la distancia en cm
  float distance = duration * 0.034 / 2;

  // Mostrar el resultado
  if (distance >= MAX_DISTANCE || distance <= 0)
  {
    Serial.println("Fuera de rango");
  }
  else
  {
    Serial.print("Distancia: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  // Esperar antes de la siguiente medición
  delay(500);

  /* setMotor(true, maxDutyCycle); // Dirección hacia adelante, máxima velocidad
  delay(3000); */
}