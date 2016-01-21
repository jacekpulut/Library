/*
 * MyMQTT.h
 *
 *  Created on: Jan 17, 2016
 *      Author: root
 */

#ifndef MYMQTT_MQTT_H_
#define MYMQTT_MQTT_H_
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

class MQTT: PubSubClient::ASubscriber {
private:
	const IPAddress mqttServer;
	const int port = 1883;
	String channel;
	PubSubClient mqttClient;
	WiFiClient client;
	boolean newMessage;

protected:
	void mqttConnect();
	String topic;
	String payload;
public:
	MQTT(String c);
	virtual ~MQTT();
	String getChannel() {
		return channel;
	}
	boolean handle() {
		if (!mqttClient.connected()) {
			mqttConnect();
		}
		return mqttClient.loop();
	}
	boolean publish(String txt);
	String getMessage() ;

	virtual void callback(char*, uint8_t*, unsigned int);

	boolean isNewMessage() const {
		return newMessage;
	}

	const String getTopic() const {
		return topic;
	}
};

#endif /* MYMQTT_MQTT_H_ */
