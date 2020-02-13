
void carica() {
  playScala();
}

void playScala() {
  //MIDImessage(NOTAON + 1 , 60 , 80 ) ;
  myGLCD.printNumI(contick , 75 , 40, 2 );
  if (contick == 0) {
    for (int i = 0; i < 7; i++)    {
      basePat[i] = 60 + getScala(i, "M");
      basePat[i + 7] = 60 + getScala(i, "m");
      //   myGLCD.print(String(getScala(i, "M")) , 3 + i * 8, 39 );
      //   myGLCD.update();
    }
    //   moduloBase(0, 14 , 0);
  }
  if (contick == 14)  {
    OnOff();
    azzeraStacks();
    contick = 0;
    carica();
    myGLCD.printNumI(contick , 75 , 40, 2 );
  }

}
