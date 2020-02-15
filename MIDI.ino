
int setnoteSpeed(int RS, byte mult) {
  int result;
  if (RS == 0) result = beat2time(PotValue(9) * mult);
  else result = beat2time(RS);
  return result;
}


void playTone(int nota, int tempdur) {
  int tono = tones[nota];
  Serial.println("Nota: " + String(nota));

  for (long i = 0; i < tempdur * 1000L; i += tono * 2)   {
    //tono
    digitalWrite(SPEAKERPIN, HIGH);
    delayMicroseconds(tono);
    digitalWrite(SPEAKERPIN, LOW);
    delayMicroseconds(tono);
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
