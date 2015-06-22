#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Schedule.h>

ControllerSchedule nextSchedule;

ScheduleClass::ScheduleClass()
{
	System.onReady(this);
	onSystemReady();
}

void ScheduleClass::onSystemReady()
{
	debugf("System ready (schedule)...");
}
