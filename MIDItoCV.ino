//Basic MIDI to CV converter
//By Robin Stubbs - robin.stubbs@uleth.ca
//Uses atmega32u4 timer 1
//ATMEGA32u4-based microcontrollers ONLY

#include <MIDI.h>  // Add Midi Library
#define LED 13    // Arduino Board LED is on Pin 13
#define CV 11
#define GATE 9

// change the pwm duty cycle as needed (0-255)
int pwm = 0;
int timer = 20;
int mlow = 24;   //low MIDI input
int mhigh = 84;  //high  - these MUST give 5 octaves

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  pinMode(CV, OUTPUT);  // set pins to low-impedance state
  pinMode(GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  
  // set timer1 freq
  DDRB|=(1<<7)|(1<<6)|(1<<5);
  TCCR1A=0xAA;
  TCCR1B=0x19; 
  
  // For 16-bit resolution, we need to program ICR1 with
  // 0xFFFF. - a 16-bit register
  ICR1 = 0xFFFF;
  OCR1A = pwm;
  
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
// OMNI sets it to listen to all channels.. MIDI.begin(2) would set it
// to respond to channel 2 notes only.
  MIDI.setHandleNoteOn(handles); // This is important!! This command
  // tells the Midi Library which function I want called when a Note ON command
  // is received. in this case it's "MyHandleNoteOn".
  
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
}

void loop() { // Main loop
  MIDI.read(); // Continually check what Midi Commands have been received.
}

// Below is my function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void handles(byte channel, byte pitch, byte velocity) {
  if(mlow <= pitch <= mhigh){
    if(velocity > 0){
    //RACS cross multiply to give value out of
    //2^16 (16 bits) instead of 60 (5 octaves)
    //(-1 b/c full value == 0V)
      pwm = ((pitch-mlow)*(65536))/(mhigh-mlow)-1; 
      OCR1A = pwm;
      digitalWrite(GATE, HIGH);
    } else {
      // WILL voice steal poorly
      digitalWrite(GATE, LOW);
    }
  }
}

