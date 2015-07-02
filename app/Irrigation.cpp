#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Irrigation.h>

IrrigationClass::IrrigationClass()
{
	currentZone = 0;
	System.onReady(this);
}

void IrrigationClass::turnOff()
{
	debugf("Turning off zones...");

    // Clear the registers
    digitalWrite(AppSettings.clearPin, LOW);
	digitalWrite(AppSettings.enablePin, HIGH);

	currentZone = 0;
}

void IrrigationClass::turnOn(int zone)
{
    // Turn off any already on zones
	turnOff();

	debugf("Turning on zone %d...", zone);

	// Prepare for data
	digitalWrite(AppSettings.clearPin, HIGH);
	digitalWrite(AppSettings.enablePin, LOW);
	digitalWrite(AppSettings.latchPin, LOW);

	// Send the data
	for (int i = zone; i > 0; i++)
	{
		if (i == zone)
			digitalWrite(AppSettings.dataPin, HIGH);
		else
			digitalWrite(AppSettings.dataPin, LOW);

		digitalWrite(AppSettings.clockPin, HIGH);
		digitalWrite(AppSettings.clockPin, LOW);
	}

    // Make it so
    digitalWrite(AppSettings.latchPin, HIGH);
    currentZone = zone;
}

void IrrigationClass::onSystemReady()
{
	debugf("System ready (irrigation)...");

	AppSettings.load();

	// Initialize pins
	pinMode(AppSettings.clearPin, OUTPUT);
	pinMode(AppSettings.clockPin, OUTPUT);
	pinMode(AppSettings.dataPin, OUTPUT);
	pinMode(AppSettings.enablePin, OUTPUT);
	pinMode(AppSettings.latchPin, OUTPUT);
}
