#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/SystemClock.h>
#include <Ntp.h>

void connectFail();
void connectOk();
void onNtpReceive(NtpClient& client, time_t timestamp);

NtpClient ntpClient("pool.ntp.org", 28800, onNtpReceive); // 8 hours

NtpClass::NtpClass()
{
	System.onReady(this);
}

void connectFail()
{
	debugf("Wifi not connected (ntp).");
	WifiStation.waitConnection(connectOk, 30, connectFail); // Repeat and check again
}

void connectOk()
{
	Serial.println("Wifi connected. Configuring Ntp client.");

	// Set client to do automatic time requests.
	ntpClient.setAutoQuery(true);
	ntpClient.setAutoUpdateSystemClock(true);
	// Request to update time now. Otherwise the set interval will pass before time is updated.
	ntpClient.requestTime();
}

void onNtpReceive(NtpClient& client, time_t timestamp) {
	//SystemClock.setTime(timestamp); // May not need to explicitly set the system clock

	debugf("Time synchronized: %s", SystemClock.getSystemTimeString().c_str());
}

void NtpClass::start()
{
	debugf("Starting ntp...");

	// Set timezone hourly difference to UTC.
	SystemClock.setTimeZone(0);

	// Run our method when the station is connected to the AP (or not connected).
	WifiStation.waitConnection(connectOk, 30, connectFail);
}

void NtpClass::onSystemReady()
{
	debugf("System ready (ntp)...");
}
