#ifndef INCLUDE_MQTT_H_
#define INCLUDE_MQTT_H_

class MqttClass : protected ISystemReadyHandler
{
public:
	MqttClass();
	~MqttClass() {}

	void start();

protected:
	virtual void onSystemReady();

private:
	//softap_config* runConfig;
};

extern MqttClass Mqtt;

#endif /* INCLUDE_MQTT_H_ */
