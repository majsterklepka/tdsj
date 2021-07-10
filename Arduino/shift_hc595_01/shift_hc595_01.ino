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
int dataIn[] = {0,1,2,4,8,16,32,64,128,256,255};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  for (int numberDisplay = 0; numberDisplay < 11; numberDisplay++)
  {
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin,MSBFIRST, dataIn[numberDisplay]);
    digitalWrite(latchPin, HIGH);
    delay(250);

  }
}
