float red = 0.0;
float green = 0.0;
float blue = 0.0;

boolean isInt(char c)
{
  return c >= 48 && c <= 57;
}

boolean validRGB(String str)
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

void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  boolean error = false;
  String redString;
  String greenString;
  String blueString;
  if (Serial.available() > 0)
  {
    String input = Serial.readString();
    //Serial.println(input);
    input.trim();
    if (input.startsWith("setRGB(") && (input.endsWith(")") || input.endsWith(");")))
    {
      input = input.substring(7);
      if (input.indexOf(",") >= 1) {
        redString = input.substring(0, input.indexOf(","));
        redString.trim();
        if (validRGB(redString))
        {
          red = redString.toFloat();
          input = input.substring(input.indexOf(",") + 1);
          input.trim();
        }
        else
        {
          Serial.println("Invalid format for red");
          error = true;
        }
      }
      else
      {
        Serial.println("Invalid input at argument for red");
        error = true;
      }
      //Serial.println("red");
      //Serial.println(red);
      //Serial.println(input);
      if (input.indexOf(",") >= 1) {
        greenString = input.substring(0, input.indexOf(","));
        greenString.trim();
        if (validRGB(greenString))
        {
          green = input.substring(0, input.indexOf(",")).toFloat();
          input = input.substring(input.indexOf(",") + 1);
          input.trim(); 
        }
        else
        {
          Serial.println("Invalid format for green");
          error = true;
        }
      }
      else
      {
        Serial.println("Invalid input at argument for green");
        error = true;
      }
      //Serial.println("green");
      //Serial.println(green);
      //Serial.println(input);
      if (input.indexOf(")") >= 1)
      {
        blueString = input.substring(0, input.indexOf(")"));
        blueString.trim();
        if (validRGB(blueString))
        {
          blue = input.substring(0, input.indexOf(")")).toFloat(); 
        }
        else
        {
          Serial.println("Invalid format for blue");
          error = true;
        }
      }
      else
      {
        Serial.println("Invalid input at argument for blue");
        error = true;
      }
      //Serial.println("blue");
      //Serial.println(blue);
    }
    else
    {
      Serial.println("Invalid input format");
      error = true;
    }
    if (red < 0 || red > 1)
    {
      Serial.println("Argument for red out of range");
      error = true;
    }
    if (green < 0 || green > 1)
    {
      Serial.println("Argument for green out of range");
      error = true;
    }
    if (blue < 0 || blue > 1)
    {
      Serial.println("Argument for blue out of range");
      error = true;
    }
    if (!error)
    {
      int led_red = (int) (red * 255);
      int led_green = (int) (green * 255);
      int led_blue = (int) (blue * 255);
      Serial.println(led_red);
      Serial.println(led_green);
      Serial.println(led_blue);
      analogWrite(9, 255 - led_red);
      analogWrite(8, 255 - led_green);
      analogWrite(7, 255 - led_blue);
    }
  }
}
