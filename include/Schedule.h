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
	DateTime date;
	Vector<ZoneSchedule> zones;

	ControllerSchedule() : date(0) {};
	~ControllerSchedule() {}

	static ControllerSchedule &invalid() { return _invalid; }

protected:

private:
	static ControllerSchedule _invalid;

};

class ScheduleClass : protected ISystemReadyHandler
{
public:
	ScheduleClass();
	~ScheduleClass() {}

	void check();
	void clear();

protected:
	ControllerSchedule currentSchedule;
	ControllerSchedule nextSchedule;

	virtual void changeZone();
	virtual void onSystemReady();

private:

};

extern ScheduleClass Schedule;

#endif /* INCLUDE_SCHEDULE_H_ */
