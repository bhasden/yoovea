#ifndef INCLUDE_IRRIGATION_H_
#define INCLUDE_IRRIGATION_H_

class IrrigationClass : protected ISystemReadyHandler
{
public:
	IrrigationClass();
	~IrrigationClass() {}

	int currentZone;
	DateTime startTime;

	void turnOff();
	void turnOn(int zone);

protected:
	virtual void onSystemReady();

private:
};

extern IrrigationClass Irrigation;

#endif /* INCLUDE_IRRIGATION_H_ */
