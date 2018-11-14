#define PanMove 3
#define PanDir 4
#define TiltMove 6
#define TiltDir 7
#define Steps 96
#define Speed 4000 // see Max Speed is faster
#define PanIn 1
#define TiltIn 5
// Sets Our Variables
long x_pos = 0;
long y_pos = 0;
long tx = 0;
long ty = 0;

#include "AccelStepper.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";
// AccelStepper Setup
AccelStepper Pan1(1, PanMove, PanDir);
AccelStepper Tilt1(1, TiltMove, TiltDir);


void setup() {

  digitalWrite(PanMove, LOW);
  digitalWrite(TiltMove, LOW);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Sets Max speeds
  Pan1.setMaxSpeed(Speed);
  Pan1.setAcceleration(Speed);

  Tilt1.setMaxSpeed(Speed);
  Tilt1.setAcceleration(Speed);
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Ready to initiate");
  Serial.println("Type Anything to Begin");
  while (Serial.available() == 0) {}

}

void getPos() {
  if (radio.available()) {
      long pos[2];
      radio.read(&pos, sizeof(pos));
      
      x_pos = pos[0];
      y_pos = pos[1];
//      if(pos[0] - tx >7 ||  tx -pos[0] >7) {
//        tx=x_pos;
//        x_pos=pos[0];
//        x_pos = map(x_pos, 0, 1023, 0, Steps);
//      }
//      
//      
//  
//      if( (pos[1] - ty) >7 ||  (ty -pos[1]) >7) {
//        ty = y_pos;
//        y_pos=pos[1];
//        y_pos = map(y_pos, 0, 1023, 0, Steps);
//       }
         
    }
}


void loop() {

  //  unsigned long currentTtime = millis();
  // if{
  getPos();

  //Serial.print("Your Pan Value is");
  //Serial.println(x_pos);
    Pan1.moveTo(x_pos);
    Pan1.run();// waits till we get to this new position, seems to be weird out by
  //  //Pan1.runToNewPosition(PosPan);

  //Serial.print("Your Tilt Value is");
  //Serial.println(y_pos);
  Tilt1.moveTo(y_pos);
  Tilt1.run();
  //  Tilt1.runToNewPosition(PosTilt);
  
}
