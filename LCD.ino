void initLcd() {
  myGLCD.InitLCD();
  myGLCD.setFont(TinyFont);
}

void displayPotStatus() {
  byte cont = 0;

  for (int i = 11; i < 12; i++)
  {
    //nota
    if (PotAverage[i] != OldPOTVal[i]) {
      printPot(i);
      cont++;
    }
  }

  if (cont > 0)   myGLCD.update();
}

void displayButtonStatus(byte numbut) {
  //per ora continuamo a tenere uno alla volta
  //in alternativa usare il doppio tasto per accendere e spegnere
  //resetScreen();
  //controlli
  byte yvol = 16; //variabile per rendere il volume mobile

  for (int i = 11; i < 12; i++)
  {
    if (i == potaction) myGLCD.drawBitmap(2 + (i % 2) * 74, 32,  fullSquare, 5, 5);
    else  myGLCD.drawBitmap(2 + (i % 2) * 74, 32,  emptySquare, 5, 5);
  }

  for (int i = 10; i < 11; i++)
  {
    if (i == potaction) myGLCD.drawBitmap(2 + (i / 11) * 74, 40,  fullSquare, 6, 6);
    else  myGLCD.drawBitmap(2 + (i / 11) * 74, 40 ,  emptySquare, 6, 6);
  }

  myGLCD.update();
}

void printPot(byte numpot) {
  resetScreen();
  scalapos = PotAverage[numpot];
  printScala();
  myGLCD.printNumI(scalapos, 20, 32, 2);
  myGLCD.print(scoreN[scalapos] + " ", 42, 32);
  myGLCD.print(score[scalapos] + " ", 64, 32);
  //  myGLCD.printNumI(PotAverage[numpot], 45, 32, 3);
  //  myGLCD.printNumI(numpot, 25, 32, 3);
}



void resetScreen() {
  //raddrizza schermo e riparte col conteggio
  millStart = millis(); //azzero conteggio ss
  myGLCD.invert(false);
  //  if ((millis() - millStart) >= 50000) layout(true); //se logo ridesegna lo schermo
}

void layout(boolean clean) {
  if (clean == true)  myGLCD.clrScr();

  // myGLCD.drawRoundRect(74, 0, 83, 38); //rett dx
  // myGLCD.drawRoundRect(0, 0, 9, 47);  //rett sx
  myGLCD.drawRoundRect(0, 38, 83, 47); //basso
  //myGLCD.drawRoundRect(0, 29, 83, 37); //centrale seq
  myGLCD.drawRoundRect(0, 0, 83, 38); //centrale main

  //righe
  // myGLCD.drawLine(42, 0, 42, 24); //centrale
  // myGLCD.drawLine(10, 24, 74, 24); //centrale
  // myGLCD.drawLine(36, 39, 36, 47); //div parameters
  //myGLCD.drawLine(10, 29, 73, 29);

  //label
  myGLCD.print(F("Key"), 3, 32);
  /*
   myGLCD.print(F("Pot0"), 11, 2 );
   myGLCD.print(F("Pot1"), 44, 2);
   myGLCD.print(F("Pot2"), 11, 9);
   myGLCD.print(F("Pot3"), 44, 9);

   myGLCD.print(F("Vel."), 11, 16);
   myGLCD.print(F("BPM "), 44, 16);

   myGLCD.print(F("Time left"), 11, 26);


   //init stati
   for (int i = 0; i < 4; i++) myGLCD.print(String(FunStatus[i]), 12 + (6 * i), 40);
  */
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

void printScala() {
  char posidxs = 0;
  char posidxse = 0;
  String strnota;

  myGLCD.drawBitmap(2, 2, emptySquare, 80, 6);
  myGLCD.drawBitmap(2, 9, emptySquare, 80, 6);
  for (int i = 0; i < 7; i++)  {
    //scala anglosassone
    strnota = score[getScala(i, "M") % 12];
    myGLCD.print(strnota, 3 + posidxs, 2 );
    posidxs = posidxs + 8 + 3;
    //scala naturale
    strnota = scoreN[getScala(i, "M") % 12];
    myGLCD.print(strnota, 3 + posidxse, 9 );
    posidxse = posidxse + strnota.length() * 4 + 2;
  }
  posidxs = 0;
  posidxse = 0;
  myGLCD.drawBitmap(1, 16, fullSquare, 82, 8);
  myGLCD.drawBitmap(1, 24, fullSquare, 82, 6);
  myGLCD.invertText(true);
  for (int i = 0; i < 7; i++)  {
    //scala anglosassone
    strnota = score[getScala(i, "m") % 12];
    myGLCD.print(strnota, 3 + posidxs, 17 );
    posidxs = posidxs + 8 + 3;
    //scala naturale
    strnota = scoreN[getScala(i, "m") % 12];
    myGLCD.print(strnota, 3 + posidxse, 24 );
    posidxse = posidxse + strnota.length() * 4 + 2;
  }
  myGLCD.invertText(false);
  myGLCD.drawRoundRect(0, 0, 83, 38); //centrale main
  myGLCD.update();
}
