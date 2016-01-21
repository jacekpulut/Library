/*
 * MyMQTT2.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: root
 */

#include "MQTT2.h"

std::vector<MQTT2::ASubscriber*> MQTT2::subscribers;

MQTT2::MQTT2() :
		mqttServer(192, 168, 1, 50), mqttClient(client) {
	mqttClient.setServer(mqttServer, port);
	mqttClient.setCallback(callback);
}

void MQTT2::mqttConnect() {
	// Loop until we're reconnected
	while (!mqttClient.connected()) {
		String id = String(ESP.getChipId());
		Serial.printf("Attempting MQTT2 connection to %s ... id: '%s'\n",
				mqttServer.toString().c_str(), id.c_str());
		// Attempt to connect
		if (mqttClient.connect(id.c_str())) {
			Serial.println("connected");
			subscribe();
		} else {
			Serial.print("failed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in .5 seconds");
			// Wait .5 seconds before retrying
			delay(500);
		}
	}
}

MQTT2::~MQTT2() {
}

boolean MQTT2::publish(String channel, String txt) {
	return mqttClient.publish(channel.c_str(), txt.c_str(), true);
}

void MQTT2::addSubscriber(MQTT2::ASubscriber *sub) {
	subscribers.push_back(sub);
	mqttClient.subscribe(sub->getTopic().c_str());
}

void MQTT2::subscribe() {
	std::vector<MQTT2::ASubscriber*> subs = MQTT2::getSubscribers();
	for (std::vector<MQTT2::ASubscriber*>::iterator it = subs.begin();
			it != subs.end(); ++it) {
		Serial.printf("subscribing to channel %s\n",(*it)->getTopic().c_str());
		mqttClient.subscribe((*it)->getTopic().c_str());
	}
}

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.printf("received %s\n", topic);
	String t = String(topic);
	String p = "";
	for (unsigned int i = 0; i < length; i++) {
		p += (char) payload[i];
	}
	std::vector<MQTT2::ASubscriber*> subs = MQTT2::getSubscribers();
	for (std::vector<MQTT2::ASubscriber*>::iterator it = subs.begin();
			it != subs.end(); ++it) {
		if ((*it)->getTopic().equals(t))
			(*it)->callback(t, p);
	}
}
