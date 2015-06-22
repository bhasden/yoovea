#ifndef INCLUDE_SCHEDULE_H_
#define INCLUDE_SCHEDULE_H_

class ZoneSchedule
{
public:
	String name;
	int number;
	int runtime;

	ZoneSchedule() : number(0), runtime(0) {};
	~ZoneSchedule() {}

protected:

private:

};

class ControllerSchedule
{
public:
	time_t date;
	Vector<ZoneSchedule> zones;

	ControllerSchedule() : date(0) {};
	~ControllerSchedule() {}

protected:

private:

};

class ScheduleClass : protected ISystemReadyHandler
{
public:
	ControllerSchedule current;
	ControllerSchedule next;

	ScheduleClass() {};
	~ScheduleClass() {}

protected:
	virtual void onSystemReady();
};

extern ScheduleClass Schedule;

#endif /* INCLUDE_SCHEDULE_H_ */
