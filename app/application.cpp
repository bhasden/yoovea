#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Mqtt.h>
#include <Ntp.h>
#include <Schedule.h>
#include <Wifi.h>

// Will be called when system initialization was completed
void startUp()
{
	//Mqtt.start();
	//startWebServer();
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Enable debug output to serial
	AppSettings.load();

	// Run servers on system ready
	System.onReady(startUp);
}
