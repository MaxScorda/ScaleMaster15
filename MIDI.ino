
int setnoteSpeed(int RS, byte mult) {
  int result;
  if (RS == 0) result = beat2time(PotValue(9) * mult);
  else result = beat2time(RS);
  return result;
}

void PlayScale() {
  int nota;
  int freqq;
  int lenn = 500*((strucInfo.howtoplay/3)+1) ;

  if ( strucInfo.PlayScala == 1)
    myGLCD.drawBitmap(2, 40, emptySquare, 81, 6);
  else
    myGLCD.drawBitmap(2, 40, fullSquare, 80, 6);
  myGLCD.invertText(!strucInfo.PlayScala);
  for (int i = 0; i < 7; i++)   {
    if ( strucInfo.PlayScala == 1)
      nota = getScala(i, "M") ; //puo' essere >11
    else {
      nota = getScala(i, "m") ; //puo' essere >11
    }
    myGLCD.print(score[nota % 12], 3 + (i * 12) , 40 );
    myGLCD.update();

    freqq = (nota / 12 > 0 ? 2 : 1); //se cambio ottava raddoppio frequenza
    serprint((String)F("nota ") + String(nota % 12));
    tone(SPEAKERPIN, (tones[nota % 12] )*freqq, lenn);
    delay(lenn * 1.3);
  }
  serprint("");
  myGLCD.invertText(false);
}


//================ scale ===========================


byte getScala(char notaf, String  tiposcala) {
  //idx= nota da calcolare NB 0-6
  byte idx = scalapos;
  notaf = constrain(notaf, 0, 6);
  for (int i = 0; i < notaf + 1; i++)  {
    if (tiposcala == "M")     idx = idx +  scalamaggiore[i];
    else idx = idx +  scalaminore[i];
  }
  return idx;
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
