#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/SystemClock.h>
#include <Irrigation.h>
#include <Schedule.h>

//ControllerSchedule nextSchedule;
Timer timer;

ScheduleClass::ScheduleClass()
{
	System.onReady(this);
}

// Publish our message
void ScheduleClass::checkSchedule()
{
	debugf("Checking whether it's time to turn on the sprinkler system");

	// The time to run the sprinkler system has passed
	if (SystemClock.now().toUnixTime() > nextSchedule.date.toUnixTime())
	{
		debugf("The next scheduled sprinkler runtime has passed.");

		currentSchedule = nextSchedule;

		for (int i = 0; i < currentSchedule.zones.count(); i++)
		{
			ZoneSchedule currentZone = currentSchedule.zones[i];

			time_t nextZoneTime = SystemClock.now().toUnixTime() + (currentZone.runtime * 1000);

			Irrigation.turnOn(currentZone.number);
		}
	}
}

void timerElapsed()
{
	Schedule.checkSchedule();
}

void ScheduleClass::onSystemReady()
{
	debugf("System ready (schedule)...");

	timer.initializeMs(300000, timerElapsed).start();
}
