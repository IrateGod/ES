#include <DueTimer.h>
#include <SPI.h>
#include <Servo.h>

int x_val;
int y_val;
int x_map = 90;
int y_map = 90;
int x_pin = 8;
int y_pin = 9;
int laser_power_pin = 27;
int replay_pin = 35;
int laser_switch_pin = 43;
int joystick_pin = 47;
int reset_pin = 9;
int dc_pin = 3;
int sce_pin = 10;
int minThreshold = 10;
int laserSwitchActiveMSCount = 0;
int laserSwitchReleasedMSCount = 0;
int replayActiveMSCount = 0;
int replayReleasedMSCount = 0;
int joystickActiveMSCount = 0;
int joystickReleasedMSCount = 0;
int coordinates[128];
int coord_len = 0;
int interpol_step = 5;
boolean isLaserSwitchPressed = false;
boolean laser_switch_pressed = false;
boolean isReplayPressed = false;
boolean replay_pressed = false;
boolean replay_started = false;
boolean isJoystickPressed = false;
boolean joystick_pressed = false;
boolean isBothButtonPressed;
boolean both_button_pressed;
Servo pan;
Servo tilt;
DueTimer laserTimer;
DueTimer replayTimer;
DueTimer joystickTimer;
DueTimer servoTimer;
int freq = 1000000;
uint8_t display[48][84];
unsigned char font[95][6] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // space
  { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 }, // !
  { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, // "
  { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 }, // #
  { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 }, // $
  { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, // %
  { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, // &
  { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 }, // '
  { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 }, // (
  { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 }, // )
  { 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00 }, // *
  { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, // +
  { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, // ,
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, // -
  { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, // .
  { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, // slash
  { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 }, // 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 }, // 1
  { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, // 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 }, // 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 }, // 4
  { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, // 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 }, // 6
  { 0x03, 0x71, 0x09, 0x05, 0x03, 0x00 }, // 7
  { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, // 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 }, // 9
  { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, // :
  { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, // ;
  { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, // <
  { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, // =
  { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
  { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, // ?
  { 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00 }, // @
  { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 }, // A
  { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, // B
  { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 }, // C
  { 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // D
  { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, // E
  { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, // F
  { 0x3E, 0x41, 0x41, 0x49, 0x7A, 0x00 }, // G
  { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, // H
  { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, // I
  { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 }, // J
  { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, // K
  { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, // L
  { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 }, // M
  { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, // N
  { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // O
  { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, // P
  { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 }, // Q
  { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, // R
  { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 }, // S
  { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 }, // T
  { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, // U
  { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, // V
  { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 }, // W
  { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // X
  { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Y
  { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Z
  { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 }, // [
  { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, // backslash
  { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
  { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, // ^
  { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, // _
  { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, // `
  { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, // a
  { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 }, // b
  { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, // c
  { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 }, // d
  { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, // e
  { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 }, // f
  { 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00 }, // g
  { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 }, // h
  { 0x00, 0x48, 0x7D, 0x40, 0x00, 0x00 }, // i
  { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 }, // j
  { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 }, // k
  { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 }, // l
  { 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 }, // m
  { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 }, // n
  { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, // o
  { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 }, // p
  { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 }, // q
  { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 }, // r
  { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, // s
  { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 }, // t
  { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 }, // u
  { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 }, // v
  { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 }, // w
  { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, // x
  { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 }, // y
  { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 }, // z
  { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, // {
  { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 }, // |
  { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, // }
  { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 } // ~
};
String gwsString;

void checkLaserSwitch(void)
{
  int current_button_state = digitalRead(laser_switch_pin);
  if (current_button_state == LOW)
  {
    laserSwitchActiveMSCount += 1;
  }
  else if (current_button_state == HIGH && isLaserSwitchPressed)
  {
    laserSwitchReleasedMSCount += 1;
  }
  else
  {
    laserSwitchActiveMSCount = 0;
    laserSwitchReleasedMSCount = 0;
  }
  if (laserSwitchActiveMSCount > minThreshold)
  {
    isLaserSwitchPressed = true;
  }
  if (laserSwitchReleasedMSCount > minThreshold)
  {
    laser_switch_pressed = true;
    isLaserSwitchPressed = false;
  }
}

void checkReplaySwitch(void)
{
  int current_button_state = digitalRead(replay_pin);
  if (current_button_state == LOW)
  {
    replayActiveMSCount += 1;
  }
  else if (current_button_state == HIGH && isReplayPressed)
  {
    replayReleasedMSCount += 1;
  }
  else
  {
    replayActiveMSCount = 0;
    replayReleasedMSCount = 0;
  }
  if (replayActiveMSCount > minThreshold)
  {
    isReplayPressed = true;
    if (isJoystickPressed)
    {
      isBothButtonPressed = true;
      if (replayReleasedMSCount > minThreshold && joystickReleasedMSCount > minThreshold)
      {
        both_button_pressed = true;
        replay_pressed = false;
        joystick_pressed = false;
        isBothButtonPressed = false;
      }
    }
  }
  if (replayReleasedMSCount > minThreshold && !isBothButtonPressed)
  {
    replay_pressed = true;
    isReplayPressed = false;
  }
}

void checkJoystickSwitch(void)
{
  int current_button_state = digitalRead(joystick_pin);
  if (current_button_state == LOW)
  {
    joystickActiveMSCount += 1;
  }
  else if (current_button_state == HIGH && isJoystickPressed)
  {
    joystickReleasedMSCount += 1;
  }
  else
  {
    joystickActiveMSCount = 0;
    joystickReleasedMSCount = 0;
  }
  if (joystickActiveMSCount > minThreshold)
  {
    isJoystickPressed = true;
  }
  if (joystickReleasedMSCount > minThreshold && !isBothButtonPressed)
  {
    joystick_pressed = true;
    isJoystickPressed = false;
  }
}

void handleServo(void)
{
  if (!replay_started)
  {
    x_val = analogRead(x_pin);
    y_val = analogRead(y_pin);
    if (x_val < 450 || x_val > 550)
    {
      x_map += map(x_val, 1, 1023, -10, 10);
    }
    if (y_val < 450 || y_val > 550)
    {
      y_map += map(y_val, 1, 1023, -10, 10);
    }
    if (x_map < 0)
    {
      x_map = 0;
    }
    if (y_map < 0)
    {
      y_map = 0;
    }
    if (x_map > 180)
    {
      x_map = 180;
    }
    if (y_map > 180)
    {
      y_map = 180;
    }
    gwsString = String(String(x_map) + "/" + String(y_map));
    pan.write(y_map);
    tilt.write(x_map);
  }
  printStatus();
}

void setPixel(int y, int x, int value)
{
  if (y < 0 || y > 47)
  {
    Serial.print("Argument y out of bounds, expected to be 0 >= y < 48, got: ");
    Serial.println(y);
    return;
  }
  if (x < 0 || x > 83)
  {
    Serial.println("Argument x out of bounds, expected to be 0 >= x < 84, got: ");
    Serial.println(x);
    return;
  }
  if (value == 0)
  {
    display[y][x] = 0;
  }
  else
  {
    display[y][x] = 1;
  }
}

void setChar(int y, int x, char c)
{
  int offset = (int) c - 0x20;
  unsigned char* fontChar = font[offset];

  for (int xAddr = 0; xAddr < 6; xAddr++)
  {
    setPixel(y, x + xAddr, fontChar[xAddr] & 1);
    setPixel(y + 1, x + xAddr, fontChar[xAddr] & 2);
    setPixel(y + 2, x + xAddr, fontChar[xAddr] & 4);
    setPixel(y + 3, x + xAddr, fontChar[xAddr] & 8);
    setPixel(y + 4, x + xAddr, fontChar[xAddr] & 16);
    setPixel(y + 5, x + xAddr, fontChar[xAddr] & 32);
    setPixel(y + 6, x + xAddr, fontChar[xAddr] & 64);
    setPixel(y + 7, x + xAddr, fontChar[xAddr] & 128);
  }
}

void setString(int y, int x, char *c_str)
{
  int i = 0;
  while (c_str[i++] != 0x00)
  {
    setChar(y, x + ((i - 1) * 6), c_str[i - 1]);
  }
}

void setArduinoString(int y, int x, String str)
{
  const char* c_str = str.c_str();
  int i = 0;
  while (c_str[i++] != 0x00)
  {
    setChar(y, x + ((i - 1) * 6), c_str[i - 1]);
  }
}

void drawDisplay()
{
  uint8_t displayBuffer = 0; 
  SPI.beginTransaction(sce_pin, SPISettings(freq, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_pin, HIGH);
  for (int y = 0; y < 48; y += 8)
  {
    for (int x = 0; x < 84; x++)
    {
      displayBuffer |= display[y][x];
      displayBuffer |= display[y + 1][x]  << 1;
      displayBuffer |= display[y + 2][x] << 2;
      displayBuffer |= display[y + 3][x] << 3;
      displayBuffer |= display[y + 4][x] << 4;
      displayBuffer |= display[y + 5][x] << 5;
      displayBuffer |= display[y + 6][x] << 6;
      displayBuffer |= display[y + 7][x] << 7;
      SPI.transfer(sce_pin, displayBuffer);
      displayBuffer = 0;
    }
  }
  SPI.endTransaction();
}

void clearDisplay()
{
  for (int x = 0; x < 84; x++)
  {
    for (int y = 0; y < 48; y++)
    {
      setPixel(y, x, 0);
    }
  }
}

void printStatus(void)
{
  clearDisplay();
  if (replay_started)
  {
    setString(0, 0, "M: Wiedergabe");
  }
  else
  {
    setString(0, 0, "M: Aufnahme");
  }
  setString(8, 0, "GWS: ");
  setArduinoString(8, 30, gwsString);
  setArduinoString(16, 0, String("C: " + String(coord_len / 2) + "/64"));
  drawDisplay();
}

void setup() {
  Serial.begin(115200);
  pinMode(laser_switch_pin, INPUT);
  pinMode(laser_power_pin, OUTPUT);
  pinMode(joystick_pin, INPUT_PULLUP);
  pan.attach(7);
  tilt.attach(8);
  pinMode(reset_pin, OUTPUT);
  pinMode(dc_pin, OUTPUT);
  digitalWrite(reset_pin, LOW);
  delay(500);
  digitalWrite(reset_pin, HIGH);
  SPI.begin(sce_pin);
  SPI.beginTransaction(sce_pin, SPISettings(freq, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_pin, LOW);
  SPI.transfer(sce_pin, 0x21);
  SPI.transfer(sce_pin, 0x13);
  SPI.transfer(sce_pin, 0xB0);
  SPI.transfer(sce_pin, 0x04);
  SPI.transfer(sce_pin, 0x20);
  SPI.transfer(sce_pin, 0x0C);
  SPI.transfer(sce_pin, 0x40);
  SPI.transfer(sce_pin, 0x80);
  digitalWrite(dc_pin, HIGH);
  if (servoTimer.configure(30, handleServo))
  {
    servoTimer.start();
  }
  if (laserTimer.configure(1000, checkLaserSwitch))
  {
    laserTimer.start();
  }
  if (replayTimer.configure(1000, checkReplaySwitch))
  {
    replayTimer.start();
  }
  if (joystickTimer.configure(1000, checkJoystickSwitch))
  {
    joystickTimer.start();
  }
}

void loop() {
  if (laser_switch_pressed)
  {
    Serial.println("Laser switch pressed");
    int state = digitalRead(laser_power_pin);
    if (state == HIGH)
    {
      digitalWrite(laser_power_pin, LOW);
    }
    else
    {
      digitalWrite(laser_power_pin, HIGH);
    }
    laser_switch_pressed = false;
  }
  if (replay_pressed && !joystick_pressed)
  {
    Serial.println("Replay switch pressed");
    replay_started = true;
    replay_pressed = false;
    for (int i = 0; i < coord_len; i += 2)
    {
      int x_dif;
      int y_dif;
      int x_prev;
      int y_prev;
      int x_step;
      int y_step;
      int x_total;
      int y_total;
      if (i == 0)
      {
        x_prev = x_map;
        y_prev = y_map;
      }
      else
      {
        y_prev = coordinates[i - 2];
        x_prev = coordinates[i - 1];
      }
      y_dif = (coordinates[i] - y_prev);
      x_dif = (coordinates[i + 1] - x_prev);
      y_step = (int) abs(y_dif) / interpol_step;
      x_step = (int) abs(x_dif) / interpol_step;
      if (y_step > x_step)
      {
        x_step = y_step;
      }
      else
      {
        y_step = x_step;
      }
      y_total = y_dif / y_step;
      x_total = x_dif / x_step;
      for (int s = 0; s < y_step; s++)
      {
        pan.write(y_prev + (s * y_total));
        tilt.write(x_prev + (s * x_total));
        delay(30);
      }
      pan.write(coordinates[i]);
      tilt.write(coordinates[i + 1]);
      gwsString = String(String(coordinates[i]) + "/" + String(coordinates[i+1]));
      printStatus();
      delay(500);
    }
    replay_started = false;
    replayActiveMSCount = 0;
    replayReleasedMSCount = 0;
  }
  if (joystick_pressed && !replay_pressed)
  {
    Serial.println("Joystick pressed");
    if (coord_len > 127)
    {
      Serial.println("Replay mode unable to store more than 64 positions");
    }
    else
    {
      coordinates[coord_len++] = y_map;
      coordinates[coord_len++] = x_map;
    }
    joystick_pressed = false;
    joystickActiveMSCount = 0;
    joystickReleasedMSCount = 0;
  }
  if (both_button_pressed)
  {
    Serial.println("both buttons pressed");
    for (int i = 0; i < coord_len; i++)
    {
      coordinates[i] = 0;
    }
    coord_len = 0;
    both_button_pressed = false;
    joystick_pressed = false;
    replay_pressed = false;
    replayActiveMSCount = 0;
    replayReleasedMSCount = 0;
    joystickActiveMSCount = 0;
    joystickReleasedMSCount = 0;
  }
  /*Serial.print("Xout voltage: ");
  Serial.println(x_val);
  Serial.print("Yout voltage: ");
  Serial.println(y_val);*/
}
