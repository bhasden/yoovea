#ifndef INCLUDE_AUTOUPDATE_H_
#define INCLUDE_AUTOUPDATE_H_

class AutoUpdateClass : protected ISystemReadyHandler
{
public:
	AutoUpdateClass(String url);
	~AutoUpdateClass() {}

	void start();

protected:
	virtual void onSystemReady();

private:
};

extern AutoUpdateClass AutoUpdate;

#endif /* INCLUDE_AUTOUPDATE_H_ */
