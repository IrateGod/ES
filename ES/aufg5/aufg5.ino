#include <SPI.h>

int reset_pin = 9;
int sce_pin = 10;
int dc_pin = 3;
int freq = 1000000;
uint8_t display[48][84]; 

void setup() {
  Serial.begin(115200);
  pinMode(reset_pin, OUTPUT);
  pinMode(dc_pin, OUTPUT);
  digitalWrite(reset_pin, LOW);
  delay(500);
  digitalWrite(reset_pin, HIGH);
  SPI.begin();
  SPI.beginTransaction(sce_pin, SPISettings(freq, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_pin, LOW);
  SPI.transfer(sce_pin, 0x21);
  SPI.transfer(sce_pin, 0x13);
  SPI.transfer(sce_pin, 0xB4);
  SPI.transfer(sce_pin, 0x04);
  SPI.transfer(sce_pin, 0x20);
  SPI.transfer(sce_pin, 0x0C);
  SPI.transfer(sce_pin, 0x40);
  SPI.transfer(sce_pin, 0x80);
  digitalWrite(dc_pin, HIGH);
  
  SPI.endTransaction();
  
  setPixel(0, 0, 1);
  setPixel(0, 1, 1);
  setPixel(0, 2, 1);
  setPixel(0, 3, 1);
  setPixel(1, 0, 1);
  setPixel(2, 0, 1);
  setPixel(3, 0, 1);
  setPixel(1, 3, 1);
  setPixel(2, 3, 1);
  setPixel(3, 3, 1);
  setPixel(3, 1, 1);
  setPixel(3, 2, 1);
  setPixel(4, 0, 1);
  setPixel(5, 0, 1);
  setPixel(6, 0, 1);
  setPixel(4, 3, 1);
  setPixel(5, 3, 1);
  setPixel(6, 3, 1);
  setPixel(6, 1, 1);
  setPixel(6, 2, 1);

  drawDisplay();

}

void setPixel(int y, int x, int value)
{
  if (y < 0 || y > 47)
  {
    Serial.println("Argument y out of bounds");
    return;
  }
  if (x < 0 || x > 83)
  {
    Serial.println("Argument x out of bounds");
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

void loop() {
  
}
