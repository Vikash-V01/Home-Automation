#include "arduino_secrets.h"
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// WiFi credentials and device information
const char DEVICE_LOGIN_NAME[] = "3ca7794a-3cb3-4d52-99e4-10dbf6c01f2d";
const char SSID[] = "Vikash";
const char PASS[] = "vikash@wifi"; 
const char DEVICE_KEY[] = "rC0bjhrhd#kvSx!hWf8PiivWD";

#define RELAY_PIN1 D0  //D0
#define RELAY_PIN2 D1  //D1

#define SwitchPin1 15  //D15
#define SwitchPin2 19  //D19


// State variables
int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 0; //Define integer to remember the toggle state for relay 2

// Switch State
bool SwitchState_1 = LOW;
bool SwitchState_2 = LOW;

// Cloud variables
CloudSwitch light1;
CloudSwitch light2;

// Function declarations
void onLight1Change();
void onLight2Change();

void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(light1, READWRITE, ON_CHANGE, onLight1Change);
  ArduinoCloud.addProperty(light2, READWRITE, ON_CHANGE, onLight2Change);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void relayOnOff(int relay) {
  switch (relay) {
    case 1:
      if (toggleState_1 == 0) {
        digitalWrite(RELAY_PIN1, HIGH); // turn on relay 1
        toggleState_1 = 1;
        light1 = 1; // Update cloud variable
        Serial.println("Device1 ON");
      }
      else {
        digitalWrite(RELAY_PIN1, LOW); // turn off relay 1
        toggleState_1 = 0;
        light1 = 0; // Update cloud variable
        Serial.println("Device1 OFF");
      }
      delay(100);
      break;
    case 2:
      if (toggleState_2 == 0) {
        digitalWrite(RELAY_PIN2, HIGH); // turn on relay 2
        toggleState_2 = 1;
        light2 = 1; // Update cloud variable
        Serial.println("Device2 ON");
      }
      else {
        digitalWrite(RELAY_PIN2, LOW); // turn off relay 2
        toggleState_2 = 0;
        light2 = 0; // Update cloud variable
        Serial.println("Device2 OFF");
      }
      delay(100);
      break;
    default: break;
  }
}

void manual_control() {
  // Handle Switch 1
  if (digitalRead(SwitchPin1) == LOW && SwitchState_1 == LOW) {
    digitalWrite(RELAY_PIN1, LOW);
    toggleState_1 = 1;
    SwitchState_1 = HIGH;
    light1 = 1; // Sync with cloud
    Serial.println("Switch-1 on");
  }
  if (digitalRead(SwitchPin1) == HIGH && SwitchState_1 == HIGH) {
    digitalWrite(RELAY_PIN1, HIGH);
    toggleState_1 = 0;
    SwitchState_1 = LOW;
    light1 = 0; // Sync with cloud
    Serial.println("Switch-1 off");
  }
  
  // Handle Switch 2
  if (digitalRead(SwitchPin2) == LOW && SwitchState_2 == LOW) {
    digitalWrite(RELAY_PIN2, LOW);
    toggleState_2 = 1;
    SwitchState_2 = HIGH;
    light2 = 1; // Sync with cloud
    Serial.println("Switch-2 on");
  }
  if (digitalRead(SwitchPin2) == HIGH && SwitchState_2 == HIGH) {
    digitalWrite(RELAY_PIN2, HIGH);
    toggleState_2 = 0;
    SwitchState_2 = LOW;
    light2 = 0; // Sync with cloud
    Serial.println("Switch-2 off");
  }
}  

// Callback functions
void doThisOnConnect(){
  Serial.println("Board successfully connected to Arduino IoT Cloud");
  digitalWrite(BUILTIN_LED, HIGH); // Turn on WiFi LED
}

void doThisOnSync(){
  Serial.println("Thing Properties synchronised");
}

void doThisOnDisconnect(){
  Serial.println("Board disconnected from Arduino IoT Cloud");
  digitalWrite(wifiLed, LOW); // Turn off WiFi LED
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();
  
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Configure pins
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);

  digitalWrite(RELAY_PIN1, LOW);
  digitalWrite(RELAY_PIN2, LOW);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  ArduinoCloud.update();
  manual_control(); // Manual Control
}

// Cloud callback functions
void onLight1Change() {
  // Control the device
  if (light1 == 1) {
    digitalWrite(RELAY_PIN1, HIGH);
    Serial.println("Device1 ON");
    toggleState_1 = 1;
  }
  else {
    digitalWrite(RELAY_PIN1, LOW);
    Serial.println("Device1 OFF");
    toggleState_1 = 0;
  }
}

void onLight2Change() {
  if (light2 == 1) {
    digitalWrite(RELAY_PIN2, HIGH);
    Serial.println("Device2 ON");
    toggleState_2 = 1;
  }
  else {
    digitalWrite(RELAY_PIN2, LOW);
    Serial.println("Device2 OFF");
    toggleState_2 = 0;
  }
}
