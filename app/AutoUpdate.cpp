#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <AutoUpdate.h>

HttpClient httpClient;
HttpFirmwareUpdate airUpdater;
Timer procTimer;
String updateUrl;

AutoUpdateClass::AutoUpdateClass(String url)
{
	updateUrl = url;
	System.onReady(this);
}

void onRequestSent(HttpClient& client, bool successful)
{
	if (successful)
	{
		debugf("Update check successful.");

		StaticJsonBuffer<200> jsonBuffer;

		String response = client.getResponseString();
		char json[response.length()+1];//as 1 char space for null is also required
		strcpy(json, response.c_str());
		JsonObject& root = jsonBuffer.parseObject(json);

		if (root != root.invalid())
		{
			const char* version = root["version"];

			if (strcmp(version, AppSettings.version.c_str()) == 0)
			{
				debugf("No new version available.");
			}
			else
			{
				debugf("New version available. Staging files...");

				JsonArray& files = root["files"];

				for (int i = 0; i < 10; i++)
				{
					JsonObject& file = files[i];

					if (file == file.invalid())
						break; // We're at the end of the file list

					int address = file["address"];
					const char* url = file["url"];

					// Configure cloud update
					airUpdater.addItem(address, url);

					debugf("String file %i from %s.", address, url);
				}

				debugf("Update files staged. Starting update...");
				airUpdater.start();
			}
		}
	}
	else
	{
		debugf("Update check failed.");
	}
}

void update()
{
	debugf("Requesting update information...");

	httpClient.downloadString("http://api.yoovea.com/firmware?v=latest", onRequestSent);
}

void AutoUpdateClass::start()
{
	debugf("Starting autoUpdate...");

	procTimer.initializeMs(86400000, update).start(); // 24 hours
}

void AutoUpdateClass::onSystemReady()
{
	debugf("System ready (autoUpdate)...");

	AppSettings.load();
}
