/*
 Name:		BusSeat Tracker Ultrasonic Sensor
 Created:	October 4, 2019 
 Sensor :   UltraSonic sensor =>  HC-SR04 
 Authors:	Joshua Ndemenge, Clinton Oduor, Aron Ayub
*/

#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <SigFox.h>


// defines pins numbers
const int trigPin = 3;
const int echoPin = 4;

// defines variables
long duration;
float distance;

void setup() {

    /*
        Get Device Info
        Run This funtion to get the ID and PAC after which yoi can safely comment
    */
   // getDeviceId();

    pinMode(trigPin, OUTPUT);   // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);    // Sets the echoPin as an Input
    Serial.begin(9600);         // Starts the serial communication
}

void loop() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delay(1000);


    // Sets the trigPin on HIGH state for 1 second
    digitalWrite(trigPin, HIGH);
    delay(1000);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);


    if(duration == 0){
        Serial.println("Warning : No pulse from the sensor");
        return;
    }
    

    // Calculating the distance
    distance= duration*0.034/2;

    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);

    //Send to Distance Data to Sigfox
    sendDataToSigFox(distance);
}

void sendDataToSigFox(float distData){
    /*
    ATTENTION - the structure we are going to send MUST
    be declared "packed" otherwise we'll get padding mismatch
    on the sent data - see http://www.catb.org/esr/structure-packing/#_structure_alignment_and_padding
    for more details
    */

    typedef struct __attribute__ ((packed)) sigfox_message {
    float dist;
    uint8_t lastMessageStatus;
    } SigfoxMessage;

    // stub for message which will be sent
    SigfoxMessage msg;

    if (!SigFox.begin()) {
        Serial.println("Shield error or not present!");
        return;
    }

    //start the module
    SigFox.begin();

    // Wait at least 30ms after first configuration (100ms before)
    delay(100);

    msg.dist=distData;        

    SigFox.status();
    delay(1);

    SigFox.beginPacket();
    SigFox.write((uint8_t*)&msg,12);

    msg.lastMessageStatus=SigFox.endPacket();

    Serial.println("status: "+ String(msg.lastMessageStatus));  
    Serial.println("Change occured with a new distance of  " + String(msg.dist)); 
}

void getDeviceId(){

    Serial.begin(9600);
    while(!Serial) {};
    if (!SigFox.begin()) {
            Serial.println("Shield error or not present!");
    }

    String version = SigFox.SigVersion();
    String ID = SigFox.ID();
    String PAC = SigFox.PAC();

    // Display module informations
    Serial.println("MKRFox1200 Sigfox first configuration");
    Serial.println("SigFox FW version " + version);
    Serial.println("ID  = " + ID);
    Serial.println("PAC = " + PAC);

    Serial.println("");

    Serial.println("Register your board on https://backend.sigfox.com/activate with provided ID and PAC");

    delay(100);

    // Send the module to the deepest sleep
    SigFox.end();

    return;
}