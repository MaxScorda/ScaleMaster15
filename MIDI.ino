
int setnoteSpeed(int RS, byte mult) {
  int result;
  if (RS == 0) result = beat2time(PotValue(9) * mult);
  else result = beat2time(RS);
  return result;
}

void PlayScale() {
  int nota;
  for (int i = 0; i < 7; i++)   {
    nota = getScala(i, "M") ;
    tone(SPEAKERPIN, tones[nota], 200);
    delay(500);
  }
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
