#include <DueTimer.h>
#define CH_ROT true
#define CH_INT false
#define ROT_CW 0
#define ROT_CCW 1
#define ROT_STOP 2

auto motor_pwm = 6;
auto dec_button_pin = 31;
auto inc_button_pin = 43;
auto motor_input_1 = 47;
auto motor_input_2 = 49;
bool motor_mode;
int incButtonActiveMSCount;
int decButtonActiveMSCount;
int bothButtonActiveMSCount;
int incButtonReleasedMSCount;
int decButtonReleasedMSCount;
int bothButtonReleasedMSCount;
bool isIncButtonPressed;
bool isDecButtonPressed;
bool isBothButtonPressed;
bool inc_button_pressed;
bool dec_button_pressed;
bool both_button_pressed;
char intensity;
int intensity_step = 16;
int minThreshold = 10;
int rotation;

DueTimer dec_timer;
DueTimer inc_timer;
DueTimer both_timer;


void changeMode()
{
  Serial.println("changeMode");
  Serial.println(motor_mode);
  Serial.println(rotation);
  Serial.println((int) intensity);
  motor_mode = !motor_mode;
}

void checkDecButtonState(void)
{
  int current_button_state = digitalRead(dec_button_pin);
  if (current_button_state == LOW)
  {
    decButtonActiveMSCount += 1;
  }
  else if (current_button_state == HIGH && isDecButtonPressed)
  {
    decButtonReleasedMSCount += 1;
  }
  else
  {
    decButtonActiveMSCount = 0;
    decButtonReleasedMSCount = 0;
  }
  if (decButtonActiveMSCount > minThreshold)
  {
    isDecButtonPressed = true;
    if (isIncButtonPressed)
    {
      isBothButtonPressed = true;
      if (decButtonReleasedMSCount > minThreshold && incButtonReleasedMSCount > minThreshold)
      {
        Serial.println("both released");
        both_button_pressed = true;
        inc_button_pressed = false;
        dec_button_pressed = false;
        isBothButtonPressed = false;
      }
    }
  }
  if (decButtonReleasedMSCount > minThreshold && !isBothButtonPressed)
  {
    dec_button_pressed = true;
    isDecButtonPressed = false;
  }
}

void checkIncButtonState(void)
{
  int current_button_state = digitalRead(inc_button_pin);
  if (current_button_state == LOW)
  {
    incButtonActiveMSCount += 1;
  }
  else if (current_button_state == HIGH && isIncButtonPressed)
  {
    incButtonReleasedMSCount += 1;
  }
  else
  {
    incButtonActiveMSCount = 0;
    incButtonReleasedMSCount = 0;
  }
  if (incButtonActiveMSCount > minThreshold)
  {
    isIncButtonPressed = true;
  }
  if (incButtonReleasedMSCount > minThreshold && !isBothButtonPressed)
  {
    inc_button_pressed = true;
    isIncButtonPressed = false;
  }
}

void checkBothButtonState(void)
{
  int current_inc_state = digitalRead(inc_button_pin);
  int current_dec_state = digitalRead(dec_button_pin);
  if (current_inc_state == LOW && current_dec_state == LOW)
  {
    bothButtonActiveMSCount += 1;
  }
  else if (current_inc_state == HIGH && current_dec_state == HIGH && isBothButtonPressed)
  {
    bothButtonReleasedMSCount += 1;
  }
  else
  {
    bothButtonActiveMSCount = 0;
    bothButtonReleasedMSCount = 0;
  }
  if (bothButtonActiveMSCount > minThreshold)
  {
    isBothButtonPressed = true;
    bothButtonActiveMSCount = 0;
  }
  if (bothButtonReleasedMSCount > minThreshold)
  {
    both_button_pressed = true;
    isBothButtonPressed = false;
    bothButtonReleasedMSCount = 0;
  }
}

void setup() {
  motor_mode = CH_INT;
  incButtonActiveMSCount = 0;
  incButtonReleasedMSCount = 0;
  decButtonActiveMSCount = 0;
  decButtonReleasedMSCount = 0;
  isIncButtonPressed = false;
  isDecButtonPressed = false;
  isBothButtonPressed = false;
  inc_button_pressed = false;
  dec_button_pressed = false;
  both_button_pressed = false;
  intensity = 64;
  rotation = ROT_CW;
  Serial.begin(115200);
  pinMode(motor_pwm, OUTPUT);
  pinMode(dec_button_pin, INPUT);
  pinMode(inc_button_pin, INPUT);
  pinMode(motor_input_1, INPUT);
  pinMode(motor_input_2, INPUT);
  if (dec_timer.configure(1000, checkDecButtonState))
  {
    dec_timer.start();
  }
  if (inc_timer.configure(1000, checkIncButtonState))
  {
    inc_timer.start();
  }
  /*if (both_timer.configure(1000, checkBothButtonState))
  {
    both_timer.start();
  }*/
}

void loop() {
  if (both_button_pressed)
  {
    changeMode();
    dec_button_pressed = false;
    inc_button_pressed = false;
    both_button_pressed = false;
    decButtonActiveMSCount = 0;
    decButtonReleasedMSCount = 0;
    incButtonActiveMSCount = 0;
    incButtonReleasedMSCount = 0;
  }
  else if (dec_button_pressed && !inc_button_pressed)
  {
   Serial.println("press dec button");
    if (motor_mode == CH_ROT)
    {
      if (rotation == ROT_CCW)
      {
        rotation = ROT_STOP;
      }
      else if (rotation == ROT_STOP)
      {
        rotation = ROT_CW;
      }
      else
      {
        rotation = ROT_CCW;
      }
    }
    else
    {
      intensity = intensity - intensity_step;
    }
    dec_button_pressed = false;
    decButtonActiveMSCount = 0;
    decButtonReleasedMSCount = 0;
  }
  else if (!dec_button_pressed && inc_button_pressed)
  {
    Serial.println("press inc button");
    if (motor_mode == CH_ROT)
    {
      if (rotation == ROT_CW)
      {
        rotation = ROT_STOP;
      }
      else if (rotation == ROT_STOP)
      {
        rotation = ROT_CCW;
      }
      else
      {
        rotation = ROT_CW;
      }
    }
    else
    {
     intensity = intensity + intensity_step; 
    }
    inc_button_pressed = false;
    incButtonActiveMSCount = 0;
    incButtonReleasedMSCount = 0;
  }
  analogWrite(motor_pwm, intensity);
  /*switch (motor_mode)
  {
    case CH_ROT: analogWrite(led, GREEN); break;
    case CH_INT: analogWrite(led, RED); break;
    default: break;
  }*/
  switch (rotation)
  {
    case ROT_CW: digitalWrite(motor_input_2, HIGH); break;
    case ROT_CCW: digitalWrite(motor_input_1, HIGH); break;
    case ROT_STOP: digitalWrite(motor_input_1, LOW); digitalWrite(motor_input_2, LOW); break;
    default: break;
  }
}
