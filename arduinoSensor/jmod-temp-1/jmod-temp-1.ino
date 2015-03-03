#include <Wire.h>      
int temp_addr = 0x48;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(temp_addr);
  Wire.write(byte(0x01));
  Wire.write(byte(0x00));
  Wire.endTransmission();
}

void loop()
{
  Wire.beginTransmission(temp_addr);
  Wire.write(byte(0x00));
  Wire.requestFrom(temp_addr,2);
  while(Wire.available()<2);
  byte c = Wire.read();
  byte d = Wire.read();
  Wire.endTransmission();
  
  word e = (c<<8) | d;
  if( (e & 0x8000) == 0x8000)
  {
    Serial.print('-');
    e = ~e +1;
  }
  c = ((e & 0x7f00) >> 8);
  d = ((e & 0x0080) >> 7) ? 5: 0;
  Serial.print(c,DEC);
  Serial.print('.');
  Serial.println(d,DEC);
  delay(500);
}
