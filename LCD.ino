void initLcd() {
  myGLCD.InitLCD();
  myGLCD.setFont(TinyFont);
}

void CheckInput(){
 // serprint(String(digitalRead(11))+"  -  "+String(digitalRead(12)));
  strucInfo.PlayScala=digitalRead(11);
  }

void displayPotStatus() {
  byte cont = 0;
  for (int i = 11; i < 12; i++)  {
    //nota
    if (PotAverage[i] != OldPOTVal[i]) {
      printPot(i);
      cont++;
    }
  }
  if (cont > 0)   myGLCD.update();
}


void printPot(byte numpot) {
  resetScreen();
  scalapos = PotAverage[numpot];
  printScala();
  myGLCD.printNumI(scalapos, 18, 32, 2);
  myGLCD.print(scoreN[scalapos] + " ", 32, 32);
  myGLCD.print(score[scalapos] + " ", 48, 32);
}



void resetScreen() {
  //raddrizza schermo e riparte col conteggio
  millStart = millis(); //azzero conteggio ss
  myGLCD.invert(false);

}

void layout(boolean clean) {
  if (clean == true)  myGLCD.clrScr();
  myGLCD.drawRoundRect(0, 38, 83, 47); //basso
  myGLCD.drawRoundRect(0, 0, 83, 38); //centrale main

  //label
  myGLCD.print(F("Key"), 3, 32);
  myGLCD.print(F("M:"), 62, 32);
  HowToPlay(0);
  myGLCD.update();
}

void screenSaver() {
  myGLCD.invert(statusblink);
  statusblink = !statusblink;
}

void logo(int del) {
  myGLCD.clrScr();
  myGLCD.drawBitmap(0, 0, hard_logo, 84, 48);
  myGLCD.print(F("H-Ard BSim 0.b"), 28, 0);
  myGLCD.update();
  delay(del);
}
