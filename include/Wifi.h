#ifndef INCLUDE_WIFI_H_
#define INCLUDE_WIFI_H_

class WifiClass : protected ISystemReadyHandler
{
public:
	WifiClass();
	~WifiClass() {}

	void start();

protected:
	virtual void onSystemReady();
};

extern WifiClass Wifi;

#endif /* INCLUDE_WIFI_H_ */
