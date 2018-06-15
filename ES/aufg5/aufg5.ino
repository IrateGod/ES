#include <SPI.h>

int reset_pin = 9;
int sce_pin = 10;
int dc_pin = 3;
int freq = 1000000;

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
}

void loop() {
  
}
