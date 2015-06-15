#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AutoUpdate.h>

HttpFirmwareUpdate airUpdater;
Timer procTimer;
String updateUrl;

AutoUpdateClass::AutoUpdateClass(String url)
{
	updateUrl = url;
	System.onReady(this);
}

void update()
{
	// Configure cloud update
	airUpdater.addItem(0x0000, "https://www.yoovea.com/firmware/v0/eagle.flash.bin");
	airUpdater.addItem(0x9000, "https://www.yoovea.com/firmware/v0/eagle.irom0text.bin");

	Serial.println("Starting update...");
	airUpdater.start();
}

void AutoUpdateClass::start()
{
	debugf("Starting autoUpdate...");

	procTimer.initializeMs(1000, update).start();
}

void AutoUpdateClass::onSystemReady()
{
	debugf("System ready (autoUpdate)...");
}
