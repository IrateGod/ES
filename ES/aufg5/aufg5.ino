#include <SPI.h>
#include <DueTimer.h>
#include <SD.h>

int reset_pin = 9;
int sce_pin = 10;
int dc_pin = 3;
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
DueTimer demoTimer;
DueTimer rotatingTimer;
int currentStudent = 0;
int demo_count = 0;
int rot_count = 0;
bool draw_ready = false;


void setup() {
  Serial.begin(115200);
  SD.begin(4);
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
  SPI.transfer(sce_pin, 0xBF);
  SPI.transfer(sce_pin, 0x04);
  SPI.transfer(sce_pin, 0x20);
  SPI.transfer(sce_pin, 0x0C);
  SPI.transfer(sce_pin, 0x40);
  SPI.transfer(sce_pin, 0x80);
  digitalWrite(dc_pin, HIGH);

  drawDisplay();
  
  SPI.endTransaction();

  demoTimer.configure(1, runStudentIdDemo);
  rotatingTimer.configure(10, runRotatingBarDemo);

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
  draw_ready = false;
}

void loadingDemo()
{
  for (int x = 0; x < 84; x++)
  {
    for (int y = 0; y < 48; y++)
    {
      setPixel(y, x, 1);
    }
    drawDisplay();
    delay(20);
  }

  for (int x = 0; x < 84; x++)
  {
    for (int y = 0; y < 48; y++)
    {
      setPixel(y, x, 0);
    }
    drawDisplay();
    delay(20);
  }
}

void runStudentIdDemo()
{
  demo_count++;
  if (demo_count > 4)
  {
    int yOffset = 13;
    setString(yOffset, 0, "              ");
    if (currentStudent)
    {
      setString(yOffset, 3, "Ferris Braatz");
      setString(yOffset + 11, 21, "6814372");
      currentStudent = 0;
    } else
    {
      setString(yOffset, 6, "Artur Alberg");
      setString(yOffset + 11, 21, "6705840");
      currentStudent = 1;
    }
    draw_ready = true;
    demo_count = 0;
  }
}

void help()
{
  Serial.println("setContrast(value): Einstellung  des  Kontrastes  des  LC-Displays. Wertebereich von Value 0,0 - 1,0");
  Serial.println("clearDisplay(): Leert die LCD-Anzeige");
  Serial.println("runRotatingBarDemo(): EIne Demo, die einen Rotierenenden Balken auf dem LCD_Display erzeugt");
  Serial.println("runStudentIdDemo(): Zeigt nacheinander im 5 sek Abstand die Namen und Matrikelnummern der Gruppenmitglieder auf dem LCD Display");
  Serial.println("stopDemo(): Stoppt die Anzeige der Demofunktionen");
  Serial.println("listDirectory(<directory>): listet den Inhalt des angegebenen Verzeichnis auf");
  Serial.println("doesFileExist(<fileName>): gibt an, ob angegebene Datei auf der SD-Karte existiert");
  Serial.println("outputFileToSerial(<fileName>): gibt den Inhalt der angegebenen Datei im seriellen Monitor aus");
  Serial.println("outputFileToLCD(<fileName>): gibt den Inhalt der angegebenen Datei im LCD aus");
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
  drawDisplay();
}

void runRotatingBarDemo()
{
  setString(12, 33, "   ");
  setString(20, 33, "   ");
  setString(28, 33, "   ");
  switch (rot_count)
  {
    case 0: setString(20, 33, "---"); break;
    case 2: setString(12, 39, "|");
            setString(20, 39, "|");
            setString(28, 39, "|"); break;
    case 3: setString(28,33,"/");
            setString(20,39,"/");
            setString(12,45,"/"); break;
    case 1: setString(28,45,"\\");
            setString(20,39,"\\");
            setString(12,33,"\\"); break;
    default: break;
  }
  if (rot_count == 3)
  {
    rot_count = 0;
  }
  else
  {
    rot_count++;
  }
  draw_ready = true;
}

void setContrast(uint8_t contrast)
{
  contrast |= 0x80;
  SPI.beginTransaction(sce_pin, SPISettings(freq, MSBFIRST, SPI_MODE0));
  digitalWrite(dc_pin, LOW);
  SPI.transfer(sce_pin, 0x21);
  SPI.transfer(sce_pin, contrast);
  SPI.transfer(sce_pin, 0x20);
  digitalWrite(dc_pin, HIGH);
  SPI.endTransaction();
}

boolean isInt(char c)
{
  return c >= 48 && c <= 57;
}

boolean validFloat(String str)
{
  String base;
  String decimals;
  if (!isInt(str.charAt(0)))
  {
    return false;
  }
  if (str.indexOf('.') > 0)
  {
    base = str.substring(0, str.indexOf('.'));
    decimals = str.substring(str.indexOf('.') + 1);
    Serial.println(base);
    Serial.println(decimals);
    for (int base_i = 0; base_i < base.length(); base_i++)
    {
      if (!isInt(base.charAt(base_i)))
      {
        return false;
      }
    }
    for (int dec_i = 0; dec_i < decimals.length(); dec_i++)
    {
      if (!isInt(decimals.charAt(dec_i)))
      {
        return false;
      }
    }
  }
  else
  {
    for (int str_i = 1; str_i < str.length(); str_i++)
    {
      if (!isInt(str.charAt(str_i)))
      {
        return false;
      }
    }
  }
  return true;
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      dir.rewindDirectory();
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void loop() 
{
  if (Serial.available() > 0)
  {
    String input = Serial.readString();
    String param;
    File entryFile;
    input.trim();
    if (input.equals("runStudentIdDemo()"))
    {
      demo_count = 5;
      rotatingTimer.stop();
      demoTimer.start();
    }
    else if (input.equals("stopDemo()"))
    {
      demo_count = 0;
      clearDisplay();
      demoTimer.stop();
      rotatingTimer.stop();
    }
    else if (input.equals("help()"))
    {
      help();
    }
    else if (input.equals("runRotatingBarDemo()"))
    {
      rot_count = 0;
      demoTimer.stop();
      rotatingTimer.start();
    }
    else if (input.startsWith("setContrast(") && input.endsWith(")"))
    {
      demoTimer.stop();
      rotatingTimer.stop();
      clearDisplay();
      input = input.substring(12);
      if (input.indexOf(")") >= 1)
      {
        param = input.substring(0, input.indexOf(")"));
        if (validFloat(param))
        {
          setContrast((uint8_t) (param.toFloat() * 127));
        }
        else
        {
          Serial.print("Invalid value for parameter: ");
          Serial.print(param);
          Serial.println(", expected to be between 0.0 and 1.0");
        }
      }
      else
      {
        Serial.println("Missing argument for setContrast, expected value between 0.0 and 1.0");
      }
    }
    else if (input.startsWith("listDirectory(\"") && input.endsWith("\")"))
    {
      input = input.substring(15);
      if (input.indexOf(")") >= -1)
      {
        param = input.substring(0, input.indexOf("\")"));
        entryFile = SD.open(param);
        printDirectory(entryFile, 0);
      }
    }
    else if (input.startsWith("doesFileExist(\"") && input.endsWith("\")"))
    {
      input = input.substring(15);
      if (input.indexOf(")") >= -1)
      {
        param = input.substring(0, input.indexOf("\")"));
        entryFile = SD.open(param);
        if (!entryFile)
        {
          Serial.print("Datei ");
          Serial.print(param);
          Serial.println(" existiert nicht.");
        }
        else
        {
          Serial.print("Datei ");
          Serial.print(param);
          Serial.println(" existiert.");
        }
        entryFile.close();
      }
    }
    else if (input.startsWith("outputFileToSerial(\"") && input.endsWith("\")"))
    {
      input = input.substring(20);
      if (input.indexOf(")") >= -1)
      {
        param = input.substring(0, input.indexOf("\")"));
        entryFile = SD.open(param);
        if (entryFile)
        {
          while (entryFile.available())
          {
            Serial.write(entryFile.read());
          }
        }
        entryFile.close();
      }
    }
    else if (input.startsWith("outputFileToLCD(\"") && input.endsWith("\")"))
    {
      input = input.substring(17);
      if (input.indexOf(")") >= -1)
      {
        param = input.substring(0, input.indexOf("\")"));
        char c;
        entryFile = SD.open(param);
        if (entryFile)
        {
          clearDisplay();
          if (param.endsWith(".TXT") || param.endsWith(".txt"))
          {
            if (entryFile.size() > 84)
            {
              setString(0, 0, "TXT_SIZE_ERR");
            }
            else
            {
              int readCount = 0;
              int yOffset = 0;
              while (entryFile.available())
              {
                c = entryFile.read();
                if (c == '\n')
                {
                  break;
                }
                setChar(yOffset, readCount * 6, c);
                if (++readCount == 14)
                {
                  yOffset += 8;
                  readCount = 0;
                }
              }
            }
          }
          else if (param.endsWith(".IMG") || param.endsWith(".img"))
          {
            String xDimension;
            String yDimension;
            while (true)
            {
              c = entryFile.read();
              if (c != ',')
              {
                xDimension += c;
              }
              else
              {
                break;
              }
            }
            while (true)
            {
              c = entryFile.read();
              if (c != '\n')
              {
                yDimension += c;
              }
              else
              {
                break;
              }
            }
            int x_dim = xDimension.toInt();
            int y_dim = yDimension.toInt();
            int x_offset = (84 - x_dim) / 2;
            int y_offset = (48 - y_dim) / 2;
            while(entryFile.available())
            {
              for (int x = 0; x < x_dim; x++)
              {
                for (int y = 0; y < y_dim; y++)
                {
                  c = entryFile.read();
                  setPixel(y_offset + y, x_offset + x, String(c).toInt());
                  entryFile.read();
                }
              }
            }
          }
          else
          {
            setString(0, 0, "invalid format");
          }
        }
        entryFile.close();
        draw_ready = true;
      }
    }
    else
    {
      Serial.print("Invalid command: ");
      Serial.print(input);
      Serial.println(", please use help() to confirm available commands");
    }
  }
  if (draw_ready)
  {
    drawDisplay();
  }
}

