

 
// Test Sequencer PA3AXA

// Constants

const int DC12V = 8;
const int DC28V = 5;
const int DC6V = 10;
const int TXFWD = 11;

const int RL1 = 3;
const int RL2 = 2;
const int RL3 = 4;
const int TXS = 12;
const int HB = 13;    // on board LED

const char ADC7 = A7;

// Variables

int adcr = 0;

// Use with millis()

uint32_t hb_time = 0; // time in msec
uint32_t hb = 2000;   // periode time in msec

boolean hbstate = LOW; 
 
void setup() { 

  digitalWrite(DC12V, LOW);   // do this first to make sure output is of
  pinMode (DC12V, OUTPUT);

  digitalWrite(DC28V, LOW);   // do this first to make sure output is of
  pinMode (DC28V, OUTPUT);

  digitalWrite(RL1, LOW);   // do this first to make sure output is of
  pinMode (RL1, OUTPUT);
  
  digitalWrite(RL2, LOW);   // do this first to make sure output is of
  pinMode (RL2, OUTPUT);
   
  digitalWrite(RL3, LOW);   // do this first to make sure output is of
  pinMode (RL3, OUTPUT);
  
  digitalWrite(TXS, LOW);   // do this first to make sure output is of
  pinMode (TXS, OUTPUT);

  digitalWrite(HB, LOW);   // do this first to make sure output is of
  pinMode (HB, OUTPUT);

// Default PowerUp WaveLab

  //digitalWrite(DC28V, LOW);
  //digitalWrite(DC12V, HIGH);
  //digitalWrite(DC6V, HIGH);
  
 // Serial.begin (9600);
 } 

void hbwdg() {
  
  if ( (millis() - hb_time) >= hb ){
        digitalWrite(HB,!digitalRead(HB));
            hb_time = millis();  
  }
}


void loop() {

adcr = analogRead(ADC7);  // read the input pin

  // TX mode selected > 18V

  if ( adcr > 675) {
  digitalWrite(TXS, HIGH);
  delay (50); 
  digitalWrite(DC28V, HIGH);
  digitalWrite(RL3, HIGH);
  delay (500);
  digitalWrite(DC12V, HIGH);

  }

  // RX mode selected < 18V

  else if ( adcr < 675 ) { 
  digitalWrite(DC12V, LOW);
  delay (500);
  digitalWrite(RL3, LOW);
  digitalWrite(DC28V, LOW);
  digitalWrite(TXS, LOW);
  
  }



hbwdg();
  
} // Loop End
