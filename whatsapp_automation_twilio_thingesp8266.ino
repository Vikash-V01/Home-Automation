#include <ESP8266WiFi.h>
#include <ThingESP.h>

#define SWITCH1 D1
#define SWITCH2 D0

ThingESP8266 thing("vikash", "EDAWhatsapp", "Edaisawesome");

void setup()
{
    Serial.begin(115200);
    pinMode(SWITCH1, OUTPUT);
    pinMode(SWITCH2, OUTPUT);
    thing.SetWiFi("SECE-TEST", "Sece@1234");
    thing.initDevice();
}

String HandleResponse(String query)
{
    if (query == "1 on")
    {
        digitalWrite(SWITCH1, HIGH);
        return "Done: SWITCH1 Turned ON";
    }

    else if (query == "1 off")
    {
        digitalWrite(SWITCH1, LOW);
        return "Done: SWITCH1 Turned OFF";
    }
    else if (query == "2 on")
    {
        digitalWrite(SWITCH2, HIGH);
        return "Done: SWITCH2 Turned ON";
    }

    else if (query == "2 off")
    {
        digitalWrite(SWITCH2, LOW);
        return "Done: SWITCH2 Turned OFF";
    }
    else if (query == "1 status"){
        return digitalRead(SWITCH1) ? "LED is ON" : "LED is OFF";
    }
    else if (query == "2 status"){
        return digitalRead(SWITCH1) ? "LED is ON" : "LED is OFF";
    }
    else
        return "Your query was invalid..";
}

void loop()
{
    thing.Handle();
}