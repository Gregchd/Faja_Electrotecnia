#include <Arduino.h>

// Pines del TB6612
const int pwmPin = 26;  // Pin para PWMA o PWMB
const int in1Pin = 12;  // Pin para AIN1 o BIN1
const int in2Pin = 14;  // Pin para AIN2 o BIN2
const int stbyPin = 27; // Pin para STBY (Standby)

// Pines del sensor de ultrasonido 1
const int trigPin1 = 0; // Pin para el sensor de ultrasonido (TRIG)
const int echoPin1 = 4; // Pin para el sensor de ultrasonido (ECHO)

// Pines del sensor de ultrasonido 2
const int trigPin2 = 2;  // Pin para el sensor de ultrasonido (TRIG)
const int echoPin2 = 15; // Pin para el sensor de ultrasonido (ECHO)

// Configuración PWM para TB6612
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

// Función para leer distancia del sensor ultrasónico
float getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, MAX_DISTANCE * 58);
  if (duration == 0) // No hay eco detectado
    return MAX_DISTANCE + 1;

  return (duration * 0.034 / 2); // Distancia en cm
}

void setup()
{
  // Configurar los pines digitales
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(stbyPin, OUTPUT);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  Serial.begin(115200);

  // Configuración del PWM
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(pwmPin, pwmChannel);

  // Activar el controlador
  digitalWrite(stbyPin, HIGH);

  // Apagar el motor al inicio
  setMotor(true, 0);
}

void loop()
{
  // Sensor 1: Esperar detección para encender el motor
  Serial.println("Esperando detección en Sensor 1...");
  while (true)
  {
    float distance1 = getDistance(trigPin1, echoPin1);
    if (distance1 <= 6) // Activar motor si la distancia es menor a 6 cm
    {
      Serial.println("Objeto detectado en Sensor 1. Activando motor...");
      setMotor(true, maxDutyCycle); // Motor adelante a máxima velocidad
      break;
    }
    delay(100);
  }

  // Sensor 2: Esperar detección para apagar el motor
  Serial.println("Esperando detección en Sensor 2...");
  while (true)
  {
    float distance2 = getDistance(trigPin2, echoPin2);
    if (distance2 <= 6) // Detener motor si la distancia es menor a 6 cm
    {
      Serial.println("Objeto detectado en Sensor 2. Deteniendo motor...");
      setMotor(true, 0); // Apagar el motor
      break;
    }

    if (distance2 > MAX_DISTANCE) // Evitar quedarse atrapado en "fuera de rango"
    {
      Serial.println("Sensor 2 fuera de rango. Continuando...");
    }
    delay(100);
  }

  delay(500); // Pausa antes de reiniciar el ciclo
}
