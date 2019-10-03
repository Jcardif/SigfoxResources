#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <SigFox.h>


// defines pins numbers
<<<<<<< HEAD
const int trigPin = 9;
const int echoPin = 10;

=======
const int trigPin = 3;
const int echoPin = 4;
>>>>>>> 5341cae145da1e1035eeb81f24a198e15a6c213a
// defines variables
long duration;
int distance;

void setup() {

    /*Get Device Info
    Run This funtion to get the ID and PAC after which yoi can safely comment
    */
    getDeviceId();

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
}

void loop() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance= duration*0.034/2;

    //Send to Distance Data to SigFox
    

    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);

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