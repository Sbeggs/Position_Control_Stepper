#define PanMove 4
#define PanDir 3
#define TiltMove 5
#define TiltDir 6
#define Steps 48
#define Speed 2500 // see Max Speed is faster

// Sets Our Variables
int PosTilt = 0;
int PosPan = 0;
int long val = 0;
int oldPan = 0;
int oldTilt = 0;
unsigned int y;

#include "AccelStepper.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// AccelStepper Setup
AccelStepper Pan1(1, PanMove, PanDir);
AccelStepper Tilt1(1, TiltMove, TiltDir);

//RF24 initiialise
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "000125";

// Position Array
long posPan ;
long posTilt ;
long pos[2];

void setup() {
  Serial.begin(9600);


  // Sets up Stepper motors and assigns them classes/names
  digitalWrite(PanMove, LOW); // was having issues of them spinning up at start up
  digitalWrite(TiltMove, LOW); // these lines should fix this until 1st command is given
  Pan1.setMaxSpeed(Speed);
  Pan1.setAcceleration(Speed);
  Tilt1.setMaxSpeed(Speed);
  Tilt1.setAcceleration(Speed);

  //Initialises Radio Stuff
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); // sets this as a receiver


  while (!Serial) {}
}


void loop() {


  GetPos();

//    Serial.print(posPan);
//    Serial.print(",");
//    Serial.println(posTilt);


  Pan1.moveTo(posPan);
  Pan1.run();// waits till we get to this new position, seems to be weird out by


  Tilt1.moveTo(posTilt);
  Tilt1.run();
}


void GetPos() {
  if (radio.available()) {

    radio.read(&pos, sizeof(pos));
    posPan = pos[0];
    posTilt = pos[1];
    Serial.print(posPan);
    Serial.print(",");
    Serial.println(posTilt);

    if (posPan > oldPan + 7 || posPan < oldPan - 7) {
      oldPan = posPan;

      posPan = map (posPan, 0, 1023, 0, Steps);

    }
    else posPan = map (oldPan, 0, 1023, 0, Steps);

    if (posTilt > oldTilt + 7 || posTilt < oldTilt - 7) {
      oldTilt = posTilt;
      posTilt = map (posTilt, 0, 1023, 0, Steps);

    }
    else posTilt = map (oldTilt, 0, 1023, 0, Steps);


  }
  else Serial.println("Fail");

}
