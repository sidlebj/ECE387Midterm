//Arduino Uno
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;
int binCount = 0;
int bin0 = 2;
int bin1 = 3;
int bin2 = 4;
int bin3 = 5;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  //set up LEDs
  pinMode(bin0, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(bin3, OUTPUT);
  
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {


 /* if(binCount % 2 == 0){
    digitalWrite(bin0, LOW);
  }
  else
    digitalWrite(bin0, HIGH);

  if(binCount > 7){
    digitalWrite(bin3, HIGH);
  }
  else
    digitalWrite(bin3, LOW);
*/
  switch(binCount){
    case 0:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, LOW);
      break;
    case 1:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, LOW);
      break;
    case 2:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, LOW);
      break;
    case 3:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, LOW);
      break;
    case 4:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, LOW);
      break;
    case 5:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, LOW);
      break;
    case 6:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, LOW);
      break;
    case 7:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, LOW);
      break;
    case 8:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, HIGH);
      break;
    case 9:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, HIGH);
      break;
    case 10:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, HIGH);
      break;
    case 11:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, LOW);
      digitalWrite(bin3, HIGH);
      break;
    case 12:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, HIGH);
      break;
    case 13:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, HIGH);
      break;
    case 14:
      digitalWrite(bin0, LOW);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, HIGH);
      break;
    case 15:
      digitalWrite(bin0, HIGH);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, HIGH);
      digitalWrite(bin3, HIGH);
      break;
    
  }
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( &start_time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    int receivedButton;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &receivedButton, sizeof(int) );             // Get the payload
      }

      //
      switch(receivedButton){
        case 0:
          
          break;

        case 1:
          if(binCount < 15)
            binCount++;
          break;

        case 2:
          if(binCount > 0)
            binCount--;
          break;

        case 3:
          binCount = 0;
          break;
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &receivedButton, sizeof(int) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(receivedButton);  
      Serial.print("Count: ");
      Serial.println(binCount);
      receivedButton = 0;
   }
 }




/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }


} // Loop

