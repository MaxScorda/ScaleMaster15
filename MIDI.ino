
int setnoteSpeed(int RS, byte mult) {
  int result;
  if (RS == 0) result = beat2time(PotValue(9) * mult);
  else result = beat2time(RS);
  return result;
}

void PlayScale() {
  int nota;
  int freqq;
   myGLCD.print(padD(" ", 20, " "), 2 , 41 );
  for (int i = 0; i < 7; i++)   {
    myGLCD.print(score[getScala(i, "M") % 12], 2 +(i*12) , 41 );
    myGLCD.update();
    nota = getScala(i, "M") ; //puo' essere >11
    freqq = (nota / 12 > 0 ? 2 : 1); //se cambio ottava raddoppio frequenza
    serprint((String)F("nota ") + String(nota%12));
    tone(SPEAKERPIN, (tones[nota% 12] )*freqq, 500);
    delay(500*1.3);
     
  }
  serprint("");
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
