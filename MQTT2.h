/*
 * MyMQTT2.h
 *
 *  Created on: Jan 17, 2016
 *      Author: root
 */

#ifndef MYMQTT_MQTT2_H_
#define MYMQTT_MQTT2_H_
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <vector>

void callback(char* topic, byte* payload, unsigned int length);

class MQTT2 {
public:
	class ASubscriber
	{
	public:
		virtual void callback(String topic, String payload);
		virtual String getTopic();
	};
private:
	const IPAddress mqttServer;
	const int port = 1883;
	PubSubClient mqttClient;
	WiFiClient client;
	static std::vector<MQTT2::ASubscriber*> subscribers;
	void subscribe();
	void mqttConnect();
public:
	MQTT2();
	virtual ~MQTT2();
	boolean handle() {
		if (!mqttClient.connected()) {
			mqttConnect();
		}
		return mqttClient.loop();
	}
	boolean publish(String channel, String txt);

	void addSubscriber(ASubscriber *sub);

	static const std::vector<ASubscriber*>& getSubscribers() {
		return subscribers;
	}
};

#endif /* MYMQTT_MQTT2_H_ */
