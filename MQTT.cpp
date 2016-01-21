/*
 * MyMQTT.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: root
 */

#include "MQTT.h"

MQTT::MQTT(String c) :
		mqttServer(192, 168, 1, 50), channel(c), mqttClient(client) {
	mqttClient.setServer(mqttServer, 1883);
	mqttClient.setSubscriber(this);
	newMessage = false;
}

void MQTT::mqttConnect() {
	// Loop until we're reconnected
	while (!mqttClient.connected()) {
		String id = String(ESP.getChipId()) + "_" + channel;
		Serial.printf("Attempting MQTT connection to %s ... id: '%s'\n",
				mqttServer.toString().c_str(), id.c_str());
		// Attempt to connect
		if (mqttClient.connect(id.c_str())) {
			Serial.println("connected");
			mqttClient.subscribe(channel.c_str());
		} else {
			Serial.print("failed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

MQTT::~MQTT() {
}

String MQTT::getMessage() {
	newMessage = false;
	return payload;
}

void MQTT::callback(char* topic, uint8_t* payload, unsigned int lenght) {
	this->topic = String(topic);
	String x = "";
	for (unsigned int i = 0; i < lenght; i++) {
		x += (char) payload[i];
	}
	this->payload = x;
	newMessage = true;
}
boolean MQTT::publish(String txt) {
	return mqttClient.publish(channel.c_str(), txt.c_str(), true);
}

