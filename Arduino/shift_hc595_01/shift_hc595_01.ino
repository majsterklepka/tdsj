/*!
 * @file shift_hc595_01.ino
 * @author Pawel Sobótka 
 * @license GNU GPLv3
 * @date 2021-07-10
 * 
 */

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int dataIn[] = {B00000000,
                B00000001,
                B00000010,
                B00000100,
                B00001000,
                B00010000,
                B00100000,
                B01000000,
                B10000000,
                B11111111};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int numberDisplay = 0; numberDisplay < 10; numberDisplay++)
  {
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin,MSBFIRST, dataIn[numberDisplay]);
    digitalWrite(latchPin, HIGH);
    delay(250);

  }
}
