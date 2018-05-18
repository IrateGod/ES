#include <DueTimer.h>
#define CWCCW true
#define CCWCW false
#define ROT_CW 0
#define ROT_CCW 1
#define ROT_STOP 2

auto motor_pwm = 6;
auto dec_button_pin = 31;
auto inc_button_pin = 43;
auto motor_input_1 = 47;
auto motor_input_2 = 49;
volatile int dec_button_state;
volatile int inc_button_state;
volatile int previous_dec_state;
volatile int previous_inc_state;
bool motor_mode;
int incButtonActiveMSCount;
int decButtonActiveMSCount;
char intensity;
int intensity_step = 16;
int minThreshold = 10;
int rotation;

DueTimer dec_timer;
DueTimer inc_timer;

void changeDecButtonState()
{
  Serial.println("changedecstate");
  previous_dec_state = dec_button_state;
  dec_button_state = !dec_button_state;
}

void changeIncButtonState()
{
  Serial.println("changeincstate");
  previous_inc_state = inc_button_state;
  inc_button_state = !inc_button_state;
}

void changeMode()
{
  Serial.println("changeMode");
  Serial.println(motor_mode);
  motor_mode = !motor_mode;
}

void checkDecButtonState(void)
{
  int current_button_state = digitalRead(dec_button_pin);
  if (current_button_state != previous_dec_state)
  {
    decButtonActiveMSCount = decButtonActiveMSCount + 1;
  }
  else
  {
    decButtonActiveMSCount = 0;
  }
  if (decButtonActiveMSCount > minThreshold)
  {
    changeDecButtonState();
    if (current_button_state == HIGH && inc_button_state == HIGH && previous_inc_state == LOW)
    {
      changeMode();
      return;
    }
    if (current_button_state == HIGH)
    {
      if (motor_mode == CWCCW)
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
       intensity = intensity - intensity_step; 
      }
    }
    decButtonActiveMSCount = 0;
  }
}

void checkIncButtonState(void)
{
  int current_button_state = digitalRead(inc_button_pin);
  if (current_button_state != previous_inc_state)
  {
    incButtonActiveMSCount = incButtonActiveMSCount + 1;
  }
  else
  {
    incButtonActiveMSCount = 0;
  }
  if (incButtonActiveMSCount > minThreshold)
  {
    if (dec_button_state == HIGH && current_button_state == HIGH)
    {
      return;
    }
    changeIncButtonState();
    if (current_button_state == HIGH)
    {
      if (motor_mode = CWCCW)
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
        intensity = intensity + intensity_step;
      }
    }
    incButtonActiveMSCount = 0;
  }
}

void setup() {
  dec_button_state = HIGH;
  inc_button_state = HIGH;
  previous_dec_state = HIGH;
  previous_inc_state = HIGH;
  motor_mode = CWCCW;
  incButtonActiveMSCount = 0;
  decButtonActiveMSCount = 0;
  intensity = 0;
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
}

void loop() {
  analogWrite(motor_pwm, intensity);
  switch (rotation)
  {
    case ROT_CW: digitalWrite(motor_input_1, HIGH); break;
    case ROT_CCW: digitalWrite(motor_input_2, HIGH); break;
    case ROT_STOP: digitalWrite(motor_input_1, LOW); digitalWrite(motor_input_2, LOW); break;
    default: break;
  }
}
