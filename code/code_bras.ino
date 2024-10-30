/// Libraries
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>


/* ------------------------------------------- */
/* ------------   DECLARATIONS   ------------- */
/* ------------------------------------------- */

/// Pins
#define SERVO_BASE_PIN 12
#define SERVO_SHOULDER_PIN 27
#define SERVO_ELBOW_PIN 26
#define SERVO_WRIST_PIN 25
#define SERVO_HEAD_PIN 33

#define POT_BASE_PIN 32
#define POT_SHOULDER_PIN 35
#define POT_ELBOW_PIN 34
#define POT_WRIST_PIN 39
#define POT_HEAD_PIN 36

#define SERVO_LOCK_PIN 13
#define RFID_SS_PIN 5
#define MODULE_IO1_PIN 2
#define MODULE_IO2_PIN 4


/// Constants
#define BASE 0
#define SHOULDER 1
#define ELBOW 2
#define WRIST 3
#define HEAD 4

#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500

#define ARM_SERVO_PERIOD_HERTZ 240
#define ARM_SERVO_MIN_ANGLE 0
#define ARM_SERVO_MAX_ANGLE 270

#define LOCK_SERVO_PERIOD_HERTZ 50
#define LOCK_SERVO_MIN_ANGLE 0
#define LOCK_SERVO_MAX_ANGLE 180
#define LOCKED_SERVO_ANGLE 90
#define UNLOCKED_SERVO_ANGLE 0

#define SERIAL_BAUD 115200


/// Variables
Servo servo_base;
Servo servo_shoulder;
Servo servo_elbow;
Servo servo_wrist;
Servo servo_head;

Servo servos_lock;

MFRC522 rfid(RFID_SS_PIN);


/// Arrays
Servo arm_servos[5] = {servo_base, servo_shoulder, servo_elbow, \
                      servo_wrist, servo_head};
int arm_servos_pins[5] = {SERVO_BASE_PIN, SERVO_SHOULDER_PIN, SERVO_ELBOW_PIN, \
                          SERVO_WRIST_PIN, SERVO_HEAD_PIN};
int arm_pots_pins[5] = {POT_BASE_PIN, POT_SHOULDER_PIN, POT_ELBOW_PIN, \
                        POT_WRIST_PIN, POT_HEAD_PIN};


/// Functions
int readRFID(void);
void moveServo(int servo, int angle);
void lockHead(void);
void unlockHead(void);


/* ------------------------------------------- */
/* ----------------   CODE   ----------------- */
/* ------------------------------------------- */

void setup()
{
  // Init Serial USB
  Serial.begin(SERIAL_BAUD);

  // Init Servos
  for (int i = 0; i < 5; i++)
  {
    arm_servos[i].setPeriodHertz(ARM_SERVO_PERIOD_HERTZ);
    arm_servos[i].attach(arm_servos_pins[i], SERVO_MIN_PULSE, SERVO_MAX_PULSE);
  }
  servos_lock.setPeriodHertz(LOCK_SERVO_PERIOD_HERTZ);
  servos_lock.attach(SERVO_LOCK_PIN, SERVO_MIN_PULSE, SERVO_MAX_PULSE);

  // Init RFID
  SPI.begin();
  rfid.PCD_Init();
}

 // NOTE: this code is just for testing during build
 //       it will be updated when hardware is finished
void loop()
{
  if (!Serial.available())
    return ;
  char command = Serial.read();
  if (Serial.read() != '\n')
  {
    Serial.println("Command Unknown");
    while (Serial.available())
      Serial.read();
    return ;
  }
  switch (command)
  {
    case 'R': // read RFID
      Serial.println(readRFID());
      break ;
    case 'L': // lock module
      lockHead();
      break ;
    case 'U': // unlock module
      unlockHead();
      break ;
    case 'I': // display all servo positions
      for (int i = 0; i < 5; i++) {Serial.println(analogRead(arm_pots_pins[i]));}
      break ;
    case '0': // move servo_base to 0°
      moveServo(BASE, 0);
      break ;
    case 'l': // test
      servo_head.write(0);
      break ;
    case 'm': // test
      servo_head.write(180);
      break ;
    case '5': // move servo_base to 135°
      moveServo(BASE, 135);
      break ;
    case 'a': // move servo_base to 270°
      moveServo(BASE, 270);
      break ;
    case '1': // move servo_shoulder to 0°
      moveServo(SHOULDER, 0);
      break ;
    case '6': // move servo_shoulder to 135°
      moveServo(SHOULDER, 135);
      break ;
    case 'b': // move servo_shoulder to 270°
      moveServo(SHOULDER, 270);
      break ;
    case '2': // move servo_elbow to 0°
      moveServo(ELBOW, 0);
      break ;
    case '7': // move servo_elbow to 135°
      moveServo(ELBOW, 135);
      break ;
    case 'c': // move servo_elbow to 270°
      moveServo(ELBOW, 270);
      break ;
    case '3': // move servo_wrist to 0°
      moveServo(WRIST, 0);
      break ;
    case '8': // move servo_wrist to 135°
      moveServo(WRIST, 135);
      break ;
    case 'd': // move servo_wrist to 270°
      moveServo(WRIST, 270);
      break ;
    case '4': // move servo_head to 0°
      moveServo(HEAD, 0);
      break ;
    case '9': // move servo_head to 135°
      moveServo(HEAD, 135);
      break ;
    case 'e': // move servo_head to 270°
      moveServo(HEAD, 270);
      break ;
    default : // not a command
      Serial.println("Command Unknown");
      break ;
  }
}

/*
Return the id of the tag close to the RFID module
Return 0 if there is no tag close to the module
*/
int readRFID(void)
{
  if (!rfid.PICC_IsNewCardPresent() && !rfid.PICC_IsNewCardPresent())
    return (0);
  if (!rfid.PICC_ReadCardSerial())
    return (0);
  rfid.PCD_StopCrypto1();
  return (rfid.uid.uidByte[1]);
}

/*
Move a servo to the angle specified
TODO : make it smooth
*/
void moveServo(int servo, int angle)
{
  arm_servos[servo].write(map(angle, ARM_SERVO_MIN_ANGLE, ARM_SERVO_MAX_ANGLE, 0, 180));
  // arm_servos[servo].writeMicroseconds(map(angle, ARM_SERVO_MIN_ANGLE, ARM_SERVO_MAX_ANGLE, \
                                          SERVO_MIN_PULSE, SERVO_MAX_PULSE));
}


// Lock the head to grab the module
void lockHead(void) {servos_lock.write(LOCKED_SERVO_ANGLE);}

// Unlock the head to release the module
void unlockHead(void) {servos_lock.write(UNLOCKED_SERVO_ANGLE);}
