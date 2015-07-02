#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/SystemClock.h>
#include <Ntp.h>

void connectFail();
void connectOk();
void onNtpReceive(NtpClient& client, time_t timestamp);

NtpClient ntpClient("pool.ntp.org", 28800, onNtpReceive);

NtpClass::NtpClass()
{
	System.onReady(this);
}

void connectFail()
{
	debugf("Ntp not connected");
	WifiStation.waitConnection(connectOk, 30, connectFail); // Repeat and check again
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("Ntp connected");

	// Set client to do automatic time requests (configured in the constructor for an 8 hour interval).
	ntpClient.setAutoQuery(true);
	ntpClient.setAutoUpdateSystemClock(true);
	// Request to update time now. Otherwise the set interval will pass before time is updated.
	ntpClient.requestTime();
}

void onNtpReceive(NtpClient& client, time_t timestamp) {
	SystemClock.setTime(timestamp);

	Serial.print("Time synchronized: ");
	Serial.println(SystemClock.getSystemTimeString());
}

void NtpClass::start()
{
	debugf("Starting ntp...");

	// set timezone hourly difference to UTC
	SystemClock.setTimeZone(0);

	// Run our method when station was connected to AP (or not connected)
	WifiStation.waitConnection(connectOk, 30, connectFail);
}

void NtpClass::onSystemReady()
{
	debugf("System ready (ntp)...");
}
