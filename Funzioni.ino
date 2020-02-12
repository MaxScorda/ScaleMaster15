//================ Default e init ================

void setDefault() {
  //evitabile
  for (int i = 3; i < 6; i++) {
    PotAverage[i] = potminmax[i][0];
  }

  PotAverage[0] = 60; // nota
  PotAverage[1] = 0; //channel
  PotAverage[2] = 1; //vettore
  PotAverage[8] = 75; // velocity
  PotAverage[9] = 20; //per ora beat

  //azzero vettore cc
  for (int i = 0; i < 10; i++) {
    ccVal[i][0] = 0;
    ccVal[i][1] = 0;
  }

}

void azzeraStacks() {
  //Stack Start
  for (int i = 0; i < playEle; i++) {
    playSt[i][0] = 255;
    playSt[i][3] = 255;
    playSt[i][4] = 0;
  }
  //Stack End
  for (int i = 0; i < endEle; i++) {
    endSt[i][0] = 255;
    endSt[i][2] = 255;
  }
  //vettore song
  for (int i = 0; i < 128; i++) {
    basePat[i] = 0;
  }
}


// centrale valori potenziometri
int PotValue(char PotNumber) {
  int ret;
  ret = PotAverage[PotNumber];
  // ret = map(PotAverage[PotNumber], 0, 127, potminmax[PotNumber][0], potminmax[PotNumber][1]);
  return ret;
}

void inizializzaDigital() {
  //mettere qui le confugurazioni diverse ogni volta
  pinMode(led, OUTPUT);
}

void inizializzaPulsanti() {
  if (hardware == 5) {
    for (int i = 2; i <= 8; i++)  {
      pinMode(i, INPUT);
      digitalWrite(i, HIGH);
    }
  }
  else {
    for (int i = 8; i < 13; i++)  {
      pinMode(i, INPUT);
      digitalWrite(i, HIGH);
    }
  }
}


//================ Gestione Pulsanti =================
void LeggiPulsanti() {
  if (hardware == 5) {
    for (int i = 0; i < 7; i++)  {
      TastoNota[i] = digitalRead(i + 2);
    }
  }
  else {

    for (int i = 8; i < 13; i++)  {
      TastoNota[i - 8] = digitalRead(i);
    }
  }
}

void ElaboraPulsanti() {
  // tutti e 6 i tasti
  float subb;

  for (int i = 0; i < 7; i++)  {
    if (TastoNota[i] != TastoNotaOld[i])  {
      if (TastoNota[i] == 0) {
        switch (i) {
          case 0: //su
            potaction = potaction - 1;
            if (potaction == -1) potaction = 10;
            if (potaction == 7) potaction = 3; //salta 2, togliere se si mettono
            displayButtonStatus(potaction);
            break;
          case 1: //+1
            if (potaction < 10) {
              subb = constrain(PotAverage[potaction] + 1 , potminmax[potaction][0] , potminmax[potaction][1] );
              PotAverage[potaction] = subb;
              displayPotStatus();
            }
            else {
              posidx++;
              if (posidx == 4) posidx = 0;
              statusfun(posidx + 10);
            }
            break;
          case 2: //giu
            potaction = potaction + 1;
            if (potaction == 11) potaction = 0;
            if (potaction == 4) potaction = 8;  // salta 2, togliere se si mettono
            displayButtonStatus(potaction);
            break;
          case 3: //-1
            if (potaction < 10) {
              subb = constrain(PotAverage[potaction] - 1 , potminmax[potaction][0] , potminmax[potaction][1] );
              PotAverage[potaction] = subb;
              displayPotStatus();
            }
            else {
              posidx--;
              if (posidx == -1) posidx = 3;
              statusfun(posidx + 10);
            }
            break;
          case 4: //E
            if (acceso == true) {
              diffstop = true;
            }
            break;
          case 5: //F
            //MIDIsoftreset();
            //statusfun(posidx);
            break;
          case 6: //select
            OnOff();
            break;
          default:
            // gestione primi 4 tatsi
            potaction = i;
        }
      }
    }
  }
}



//================ Joystick ==============
void setfromJoystick() {
  int z;
  x_direction = 0;
  y_direction = 0;

  switch (hardware) {
    case 4:
      x_speed = 0;
      y_speed = 0;
      z = analogRead(0);
      if (z == 0) //left
      {
        x_direction = -1;
        x_speed = 1;
      }
      else if (z > 0 && z < 150) //ok
      {
        // pressione tasto centrale ad emulare il tasto 6
        TastoNota[6] = false;
      }
      else if (z > 150 && z < 350) //down
      {
        y_direction = 1;
        y_speed = 1;
      }
      else if (z > 350 && z < 510) //right
      {
        x_direction = 1;
        x_speed = 1;
      }
      else if (z > 510 && z < 750) //up
      {
        y_direction = -1;
        y_speed = 1;
      }
      else if (z > 750) //nil
      {
        //simulo il valore alto del tasto portando ad alto il tasto
        TastoNota[6] = true;
      }
      PotAverage[potaction] = PotAverage[potaction] + x_direction ;
      break;

    case 5:
      x_position = analogRead(PIN_ANALOG_X);
      y_position = analogRead(PIN_ANALOG_Y);

      x_speed = map(x_position + (X_THRESHOLD_HIGH - X_THRESHOLD_LOW), 0, 1023, -2, 2);
      y_speed = map(y_position + (Y_THRESHOLD_HIGH - Y_THRESHOLD_LOW), 0, 1023, -2, 2);

      if (x_position > X_THRESHOLD_HIGH) {
        x_direction = 1;
      }
      else if (x_position < X_THRESHOLD_LOW) {
        x_direction = -1;
      }

      if (y_position > Y_THRESHOLD_HIGH) {
        y_direction = 1;
      }
      else if (y_position < Y_THRESHOLD_LOW) {
        y_direction = -1;
      }
      //senza map del'analogico, normalizzare i valori (?? serve)
      y_speed = min(abs(y_speed), ((potminmax[potaction][1] - potminmax[potaction][0]) / 10) + 1) * (y_speed / abs(y_speed));

      //genialata sta a vedere
      break;
  }

  PotAverage[potaction] = constrain(PotAverage[potaction], potminmax[potaction][0], potminmax[potaction][1]);


  // eccezione per potaction=10
  if  (potaction == 10) {
    // asse y
    if ((y_speed != 0) && (lasty == 0)) {
      statusfun(posidx);
      lasty = y_direction;
    }
    else lasty = 0;
  }

  // asse x NB: mettere ultimo per scavalcare errori di movimento
  if (x_speed != 0) {
    if (x_direction == 1) OldPOTVal[potaction] = 255;
    else OldPOTVal[potaction] = 254;
  }
  else lastx = 0;

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

void controlBlink() {
  if (joystickComm == false) {
    digitalWrite(10, 0);
    digitalWrite(11, 0);
    digitalWrite(12, 0);
  }
  else {
    //Midi Led
    if ((luciArrayback[0] != luciArray[0]) || ((luciArray[0] == true) )) {
      luciArrayback[0] = luciArray[0]; //va messo qui cosi' forzo poi lo spegnimento
      if (luciArray[0] == true) {
        myGLCD.drawBitmap(75, 40,  fullSquare, 7, 6);
        luciArray[0] = false; //forzo spegnimento
      }
      else  myGLCD.drawBitmap(75, 40,  emptySquare, 7, 6);
    }
  }
  digitalWrite(led, LOW);
  //myGLCD.update(); // lo metto solo qui col refresh al tick
}


void statusPot(String extra) {
  serprint(extra);
  //lcdprint(extra, 32, Dx);
  //serprint(String(PotAverage[0]) + F(" - ") + String(PotAverage[1]) + F(" - ") + String(PotAverage[2]) + F(" - ") + String(PotAverage[3]));
}


void statusfun(char poss) {
  resetScreen();
  if (poss >= 10) {
    poss = poss - 10;
    myGLCD.clrLine(11, 46, 35, 46);
    myGLCD.drawLine(11 + (poss * 6), 46, 17 + (poss * 6), 46);
  }
  else {
    switch (poss) {
      case 0:
        //N: esegui note
        if (FunStatus[poss] == ' ')   {
          FunStatus[poss] = 'N' ;
        }
        else     {
          FunStatus[poss] = ' ' ;
        }
        // setta lo stato prima ma parti solo se acceso
        if (acceso == true) {
        }
        break;

      case 1:
        //C: esegui CC
        if (FunStatus[poss] == ' ')   FunStatus[poss] = 'C' ;
        else FunStatus[poss] = ' ' ;
        break;

      case 2:
        if (FunStatus[poss] == ' ')   FunStatus[poss] = 'M' ;
        else FunStatus[poss] = ' ' ;
        break;

      case 3:
        break;
    }
    myGLCD.print(String(FunStatus[poss]), 12 + (6 * poss), 40);
    displayPotStatus();
  }
}

void serprint(String valprint) {
  if (debug) Serial.println(valprint);
}

void lcdprint(String valprint, byte xpos, char cleanmode ) {
  xpos = max(xpos, 39);
  if (joystickComm == true) {
    switch (cleanmode) {
      case Sx:
        myGLCD.drawBitmap(37, 39, emptySquare, xpos, 7);
        break;
      case Tutto:
        myGLCD.drawBitmap(37, 39, emptySquare, 75, 7);
        break;
      case Dx:
        myGLCD.drawBitmap(xpos, 39, emptySquare, 75 - xpos, 7);
        break;
      case Nulla:
        // myGLCD.clrRect(xpos, 40, xpos + (valprint.length() * 4) - 1, 45);
        // myGLCD.drawBitmap(xpos, 40,  emptySquare, valprint.length()*4, 7);
        break;
    }
    myGLCD.setFont(TinyFont);
    myGLCD.print(valprint, xpos, 40);
    //myGLCD.update(); //lo lascio solo al tick
  }
}



//================ Varie ==============
void OnOff() {
  acceso = !acceso;
  serprint(String(acceso));
  diffstop = false;
  if (acceso == true) {
    if (FunStatus[0] == 'N') {};
    lcdprint(F(" S/ON"), 32, Dx);
  }
  else {
    spengoNote();
    if (debug)  lcdprint(F("S/OFF DBG"), 32, Dx);
    else lcdprint(F("  S/OFF"), 32, Dx);
  }
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
