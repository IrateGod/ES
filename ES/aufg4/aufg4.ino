#include <DueTimer.h>

auto motor_pwm = 6;
auto dec_button_pin = 31;
auto inc_button_pin = 43;
auto motor_input_1 = 47;
auto motor_input_2 = 49;
volatile auto dec_button_state = HIGH;
volatile auto inc_button_state = HIGH;
volatile auto previous_dec_state = HIGH;
volatile auto previous_inc_state = HIGH;
boolean motor_mode = true;
int incButtonActiveMSCount = 0;
int decButtonActiveMSCount = 0;
char intensity = 0;
int intensity_step = 16;
int minThreshold = 10;

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
    if (dec_button_state == LOW && inc_button_state == LOW)
    {
      changeMode();
    }
    if (current_button_state == HIGH)
    {
      intensity = intensity - intensity_step;
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
    changeIncButtonState();
    if (current_button_state == HIGH)
    {
       intensity = intensity + intensity_step;
    }
    incButtonActiveMSCount = 0;
  }
}

void setup() {
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
  digitalWrite(motor_input_1, HIGH);
}
