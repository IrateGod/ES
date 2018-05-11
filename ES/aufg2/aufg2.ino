#include <DueTimer.h>

auto led_pin = 7;
auto dec_button_pin = 5;
auto inc_button_pin = 3;
volatile auto dec_button_state = HIGH;
volatile auto inc_button_state = HIGH;
volatile auto previous_dec_state = HIGH;
volatile auto previous_inc_state = HIGH;
int incButtonActiveMSCount = 0;
int decButtonActiveMSCount = 0;
char luminosity = 0;
int luminosity_step = 16;
int minThreshold = 10;
DueTimer dec_timer;
DueTimer inc_timer;

void changeDecButtonState()
{
  previous_dec_state = dec_button_state;
  dec_button_state = !dec_button_state;
}

void changeIncButtonState()
{
  previous_inc_state = inc_button_state;
  inc_button_state = !inc_button_state;
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
    if (current_button_state == HIGH)
    {
      luminosity = luminosity - luminosity_step;
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
       luminosity = luminosity + luminosity_step;
    }
    incButtonActiveMSCount = 0;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(inc_button_pin, INPUT);
  pinMode(dec_button_pin, INPUT);
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
  // Serial.println(luminosity);
  analogWrite(led_pin, luminosity);
}
