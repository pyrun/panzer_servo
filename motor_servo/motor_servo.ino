/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <printf.h>

#define pin_motor_links 7
#define pin_motor_rechts 8

#define BAUDRATE 9600

#define PYRUN_PIPE_HOST 0xC0F0E0F0E1LL
#define PYRUN_PIPE_NODE 0xA0ECCCF0D2LL

#define id '7'

RF24 m_radio( 9, 10);

Servo motor_links;
Servo motor_rechts;

// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(BAUDRATE);
  
  m_radio.begin();
  m_radio.enableDynamicPayloads();
  m_radio.setAutoAck( true ) ;
  // Optionally, increase the delay between retries & # of retries
  //m_radio->setRetries(2,15);
  // set speed
  m_radio.setDataRate( RF24_250KBPS );
  m_radio.powerUp();

  m_radio.openWritingPipe( PYRUN_PIPE_HOST);
  m_radio.openReadingPipe(1, PYRUN_PIPE_NODE);
  
  // initialize digital pin 13 as an output.
  pinMode( 13, OUTPUT);

  printf_begin();
  
  motor_stop();
  motor_forward();
}

void blink( int t) {
  for( int i = 0; i < t; i++) {
    delay( 100);
    digitalWrite( 13, HIGH);
    delay( 100);
    digitalWrite( 13, LOW);
  }
}

void motor_forward() {
  motor_links.attach( pin_motor_links);
  motor_rechts.attach( pin_motor_rechts);
  
  motor_links.write(175);
  motor_rechts.write(90 );
}

void motor_left() {
  motor_links.attach( pin_motor_links);
  motor_rechts.attach( pin_motor_rechts);
  
  motor_links.write(5);
  motor_rechts.write(5);
}

void motor_right() {
  motor_links.attach( pin_motor_links);
  motor_rechts.attach( pin_motor_rechts);
  
  motor_links.write(175);
  motor_rechts.write(175);
}


void motor_backward() {
  motor_links.attach(pin_motor_links);
  motor_rechts.attach(pin_motor_rechts);
 
  motor_links.write(5);
  motor_rechts.write(175);
}


void motor_stop() {
  motor_links.detach();
  motor_rechts.detach();
}

// the loop function runs over and over again forever
void loop() {
  m_radio.stopListening();
  char msg2[64];
  sprintf( msg2,"%c", id);
  m_radio.write( &msg2, strlen(msg2));
  m_radio.startListening();
  delay(100);
  if( m_radio.available() ) {
    char t_message[64];
    int t_len = m_radio.getDynamicPayloadSize();
    
    // auslesen
    m_radio.read( &t_message, t_len);
    Serial.println( t_message);

    if( t_message[0] == id) {
      switch( t_message[1]) {
        case 'w': motor_forward();  break;
        case 'b': motor_backward(); break;
        case 'l': motor_left(); break;
        case 'r': motor_right(); break;
        case 's':
        default: motor_stop(); break;
      }
    }
    
    m_radio.startListening();
  }
  
/*  blink(1);
  motor_forward();
  delay(2000);              // wait for a second

  
  blink(2);
  motor_stop();
  delay(2000);          // wait for a second

  blink(3);
  motor_backward();
  delay(2000);              // wait for a second
  
  blink(4);
  motor_stop();
  delay(2000);          // wait for a second*/
}
