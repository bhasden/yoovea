#ifndef INCLUDE_NTP_H_
#define INCLUDE_NTP_H_

class NtpClass : protected ISystemReadyHandler
{
public:
	NtpClass();
	~NtpClass() {}

	void start();

protected:
	virtual void onSystemReady();

private:
	//softap_config* runConfig;
};

extern NtpClass Ntp;

#endif /* INCLUDE_NTP_H_ */
