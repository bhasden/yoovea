#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Mqtt.h>

void connectFail();
void connectOk();
void onMessageReceived(String topic, String message);

MqttClient client = MqttClient("localhost", 1883, onMessageReceived);
Timer procTimer;

MqttClass::MqttClass()
{
	System.onReady(this);
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
	debugf("Wifi not connected (mqtt).");
	WifiStation.waitConnection(connectOk, 30, connectFail); // Repeat and check again
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("Mqtt connected");

	client = MqttClient(AppSettings.mqttServer, 1883, onMessageReceived);

	// Run MQTT client
	client.connect("esp8266");
	client.subscribe("main/status/#");
}

// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
	Serial.print(topic);
	Serial.print(":\r\n\t"); // Prettify alignment for printing
	Serial.println(message);
}

// Publish our message
void publishMessage()
{
	Serial.println("Let's publish message now!");
	client.publish("main/frameworks/sming", "Hello friends, from Internet of things :)"); // or publishWithQoS
}

void MqttClass::start()
{
	debugf("Starting mqtt...");

	// Run our method when the station is connected to the AP (or not connected).
	WifiStation.waitConnection(connectOk, 30, connectFail);
}

void MqttClass::onSystemReady()
{
	debugf("System ready (mqtt)...");

	AppSettings.load();
}
