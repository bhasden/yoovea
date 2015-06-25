#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <SmingCore/SystemClock.h>
#include <Schedule.h>

ControllerSchedule nextSchedule;
Timer timer;

ScheduleClass::ScheduleClass()
{
	System.onReady(this);
	onSystemReady();
}

// Publish our message
void checkSchedule()
{
	debugf("Checking whether it's time to turn on the sprinkler system");

	// The time to run the sprinkler system has passed
	if (SystemClock.now().toUnixTime() > nextSchedule.date.toUnixTime())
	{
		debugf("The next scheduled sprinkler runtime has passed.");
	}
}

void ScheduleClass::onSystemReady()
{
	debugf("System ready (schedule)...");

	timer.initializeMs(300000, checkSchedule).start(); // 5 minutes
}
