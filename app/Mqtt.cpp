#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <AppSettings.h>
#include <Mqtt.h>

void onMessageReceived(String topic, String message); // Forward declaration for our callback

Timer procTimer;

// MQTT client
// For quickly check you can use: http://www.hivemq.com/demos/websocket-client/ (Connection= test.mosquitto.org:8080)
MqttClient client = MqttClient("localhost", 1883, onMessageReceived);

MqttClass::MqttClass()
{
	System.onReady(this);
	onSystemReady();
}

// Will be called when WiFi station timeout was reached
void connectFail()
{
	Serial.println("I'm NOT CONNECTED. Need help :(");

	// .. some you code for device configuration ..
}

// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("Mqtt connected");

	client = MqttClient("test.mosquitto.org", 1883, onMessageReceived);

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

	WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start
}

void MqttClass::onSystemReady()
{
	debugf("System ready (mqtt)...");
}
