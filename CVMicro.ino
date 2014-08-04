//RACS - Simple 16-bit Arduino CV Example
//Uses atmega32u4 timer 1
//ATMEGA32u4 ONLY
#define LED 13

// change the pwm duty cycle as needed (0-255)
int pwm = 0;
int timer = 10;

void setup(){
 // TCCR1 speed
  DDRB| = (1<<7) | (1<<6) | (1<<5);
  TCCR1A = 0xAA;
  TCCR1B = 0x19; 
  
 // For 16-bit resolution, we need to program ICR1 with
 // 0xFFFF. - a 16-bit register. 0xFFF gives 12-bit, etc.
  ICR1 = 0xFFFF;
  
  OCR1A = pwm; //CV output Timer: 1 Channel: A - Pin 9 on Leonardo/Micro
  
  //Flash LED on successful setup
  digitalWrite(LED, HIGH);
  delay(300);
  digitalWrite(LED, LOW);
}

void loop(){
  //Example loop through duty cycle in 16-bits
  //Will ascend from 0-5V
  //Change to sequence/MIDI/whatever for specific notes
  for(int i = 0; i < 2^16; i+=50){
    pwm = i;
    OCR1A = pwm;
    delay(timer);
    Serial.println(i);
  }
}
