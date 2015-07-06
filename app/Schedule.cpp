#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/SystemClock.h>
#include <Irrigation.h>
#include <Schedule.h>

Timer scheduleTimer;
Timer zoneTimer;
int zoneScheduleIndex;

ScheduleClass::ScheduleClass()
{
	System.onReady(this);
}

void ScheduleClass::changeZone()
{
	zoneScheduleIndex++;
	debugf("Changing to zone %d.", zoneScheduleIndex);

	if (&currentSchedule != &currentSchedule.invalid() &&
		zoneScheduleIndex < currentSchedule.zones.count())
	{
		ZoneSchedule nextZone = currentSchedule.zones[zoneScheduleIndex];

		debugf("Running zone %s for %d minutes.", nextZone.name.c_str(), nextZone.runtime);
		zoneTimer.initializeMs(nextZone.runtime * 1000, TimerDelegate(&ScheduleClass::changeZone, this)).startOnce();
	}
}

void ScheduleClass::check()
{
	debugf("Checking whether it's time to turn on the sprinkler system.");

	if (&nextSchedule == &nextSchedule.invalid())
	{
		debugf("No schedule available.");
		return;
	}

	// The time to run the sprinkler system has passed
	if (SystemClock.now().toUnixTime() > nextSchedule.date.toUnixTime())
	{
		debugf("The next scheduled sprinkler runtime has passed.");

		currentSchedule = nextSchedule;
		zoneScheduleIndex = -1;
		changeZone();
	}
}

void ScheduleClass::clear()
{
	debugf("Clearing schedule.");

	currentSchedule = ControllerSchedule::invalid();
	nextSchedule = ControllerSchedule::invalid();
}

void ScheduleClass::onSystemReady()
{
	debugf("System ready (schedule)...");

	scheduleTimer.initializeMs(300000, TimerDelegate(&ScheduleClass::check, this)).start(); // 5 minutes
}
