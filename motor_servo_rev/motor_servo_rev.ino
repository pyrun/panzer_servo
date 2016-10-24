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

#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <printf.h>

#define BAUDRATE 9600

#define PYRUN_PIPE_HOST 0xC0F0E0F0E1LL
#define PYRUN_PIPE_NODE 0xA0ECCCF0D2LL

RF24 m_radio( 9, 10);

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

  m_radio.openWritingPipe( PYRUN_PIPE_NODE);
  m_radio.openReadingPipe(1, PYRUN_PIPE_HOST );

  printf_begin();
}


String inputString;         // a string to hold incoming data
boolean stringComplete = false;
    
// the loop function runs over and over again forever
void loop() {

  /*m_radio.stopListening();
  char msg2[64];
  sprintf( msg2,"testtest");
  m_radio.write( &msg2, strlen(msg2));
  m_radio.startListening();*/
  m_radio.startListening();
  
  delay(100);
  
  while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n') {
        stringComplete = true;
      }
    }

  if( stringComplete) {
        char msg3[64];
        inputString.toCharArray(msg3, 64);
        m_radio.stopListening();
        m_radio.write( &msg3, strlen(msg3));
        m_radio.startListening();
        Serial.println( inputString);
        inputString = "";
        stringComplete = false;
    }

  if( m_radio.available() ) {
    char t_message[64];
    int t_len = m_radio.getDynamicPayloadSize();
    
    // auslesen
    m_radio.read( &t_message, t_len);
    Serial.println( t_message);
    m_radio.startListening();
  }
  
}
