#pragma once

#include <vector>
#include <thread>
#include "producer.h"
#include "consumer.h"
#include "broker.h"

class Manager {
	Producer *producers;
	int producersCount;
	std::vector<std::thread> producersThreads;
	Consumer *consumers;
	int consumersCount;
	std::vector<std::thread> consumersThreads;
	Broker *brokers;
	int brokersCount;
	std::vector<std::thread> brokersThreads;

public:
	Manager(int producersCount, int consumersCount, int brokersCount, int poolCapacity);
	~Manager();
	void run();
	void end();

};

