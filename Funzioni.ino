//================ Default e init ================

int PotValue(char PotNumber) {
  int ret;
  ret = PotAverage[PotNumber];
  return ret;
}

void inizializzaDigital() {
  //mettere qui le confugurazioni diverse ogni volta
  pinMode(LED, OUTPUT);
  pinMode(SPEAKERPIN, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(11, HIGH);
  pinMode(12, INPUT_PULLUP);
}

void inizializzaVar() {
  strucInfo.howtoplay = 0;
}


//================ Joystick ==============
void LeggiPulsanti() {
  // Lettura dei tasti analogici e digitali
  //NB: imposto a 1 se non premuto e a 0 quando premuto per mantenere compatibilita' con la logica fisica dei tasti
  byte readkey;
  readkey = 0;
  int leggiser = 0;

  //LETTURA ANALOGICA
  leggiser = analogRead(0);
  // serprint(String(leggiser));
  if (leggiser > 900)
    TastoNota[11] = 0;
  else if (leggiser < 100)
    TastoNota[11] = 1; //sx
  else if (leggiser < 200)
    TastoNota[11] = 5; //sel
  else if (leggiser < 400)
    TastoNota[11] = 2; //down
  else if (leggiser < 600)
    TastoNota[11] = 3;//dx
  else if (leggiser < 900)
    TastoNota[11] = 4;//up
}


void ElaboraPulsantiAnalog() {
  static unsigned long timevis = millis();
  if (( TastoNota[11] != TastoNotaOld[11]) || (lastread > 0)) {
    if (TastoNota[11] == 1)  {
      HowToPlay(-1);
      //  serprint(F("sx"));
    }
    else if (TastoNota[11] == 2)  {
      // serprint(F("down"));
      PotAverage[11] =  (PotAverage[11] + 1) % 12;
      displayPotStatus();
    }
    else if (TastoNota[11] == 3)  {
      HowToPlay(1);
      // serprint(F("dx"));
    }
    else if (TastoNota[11] == 4)  {
      //  serprint(F("up"));
      PotAverage[11] =  (PotAverage[11] + 11) % 12;
      displayPotStatus();
    }
    else if (TastoNota[11] == 5)  {
      // serprint(F("Sel"));
      PlayScale();
    }
    lastread = 0;
    timevis = millis();
  }
  else {
    if ( (timevis + 500 < millis()) && (TastoNota[11] > 0)) {
      lastread = 1;
      timevis = millis();
    }
  }
}

void HowToPlay(char valmov) {
  //0-9:  30/60/90 bpm con s:suono M e m in base scelta, T:suono M e n, T: scala scelta su cambio scala
  String toPrint;
  String strApp[] = {F("S"), F("A"), F("C")};
  strucInfo.howtoplay = (strucInfo.howtoplay + valmov + 9) % 9;
  toPrint = String((((strucInfo.howtoplay / 3) + 1) * 30)) + strApp[strucInfo.howtoplay % 3];
  myGLCD.print(toPrint, 70, 32);
}

//================ Random ==============
int goodRandomseed() {
  long tempBits = 0;  // create a long of random bits to use as seed
  for (int i = 1; i <= 32 ; i++) {
    tempBits = ( tempBits | ( analogRead( 0 ) & analogRead( 1 ) & 1 ) ) << 1;
  }
  randomSeed(tempBits);
}

int RandNum(int rmin, int rmax) {
  int result;
  result = random(rmin, rmax + 1);
  //serprint(String(result));
  return result;
}

//================ Visualizzazioni ==============

void serprint(int valprint) {
  serprint(String(valprint));
}

void serprint(String valprint) {
  if (debug) Serial.println(valprint);
}

void lcdprint(String valprint, byte xpos, char cleanmode ) {
  xpos = max(xpos, 39);
  if (joystickComm == true) {
    switch (cleanmode) {
      case -1:
        myGLCD.drawBitmap(37, 39, emptySquare, xpos, 7);
        break;
      case 99:
        myGLCD.drawBitmap(37, 39, emptySquare, 75, 7);
        break;
      case 1:
        myGLCD.drawBitmap(xpos, 39, emptySquare, 75 - xpos, 7);
        break;
      case 0:
        // myGLCD.clrRect(xpos, 40, xpos + (valprint.length() * 4) - 1, 45);
        // myGLCD.drawBitmap(xpos, 40,  emptySquare, valprint.length()*4, 7);
        break;
    }
    myGLCD.setFont(TinyFont);
    myGLCD.print(valprint, xpos, 40);

  }
}



//================ Varie ==============
void OnOff() {
  acceso = !acceso;
  serprint((String)F("Acceso ") + (acceso ? F("On") : F("Off")));
}

unsigned int beat2time(int beat) {
  int ret;
  ret = float((60000.0 / beat) + 0.5);
  // necessario da IDE 1.5.8 CONTROLLARE SEMPRE
  ret = ret + timeOffset;
  return ret;
}


void swap(byte &value1, byte &value2)
{
  byte temp;
  temp = value1;
  value1 = value2;
  value2 = temp;
}

char segnoRandom() {
  char randn = RandNum(0, 1);
  if (randn == 0) return -1;
  else return 1;
}

word sommatoria(int numstart, int numend) {
  //numstart: numero di partenza
  //numend: numero arrivo
  return float((numend + numstart) / 2.0) * float(numend - numstart + 1.0);
}

int mcm (int a, int b) {
  //minimo comune multiplo
  int ret;
  int cont1, cont2;
  if (a == b) ret = a;
  else {
    cont1 = a;
    cont2 = b;
    while (cont1 != cont2) {
      if (cont1 < cont2) cont1 = cont1 + a;
      if (cont1 > cont2) cont2 = cont2 + b;
    }
    ret = cont1;
  }
  return ret;
}

String padD(String sstt, int lens, String charsub) {
  //pad a dx
  String spaces = "";
  for (int i = sstt.length(); i < lens; i++) spaces = spaces + charsub;
  return sstt + spaces;
}
String padS(String sstt, int lens, String charsub) {
  //pad a sx
  String spaces = "";
  for (int i = sstt.length(); i < lens; i++) spaces = spaces + charsub;
  return spaces + sstt;
}
