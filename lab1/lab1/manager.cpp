#include "manager.h"

Manager::Manager(int producersCount, int consumersCount, int brokersCount, int poolCapacity) : producersCount(producersCount), consumersCount(consumersCount), brokersCount(brokersCount) {
	producers = new Producer[producersCount];
	consumers = new Consumer[consumersCount];
	brokers = new Broker[brokersCount];

	for (int i = 0; i < producersCount; i++) {
		producers[i].addConsumersCount(consumersCount);
	}
	for (int i = 0; i < consumersCount; i++) {
		consumers[i].addPoolCapacity(poolCapacity);
	}

	producersThreads.reserve(producersCount);
	consumersThreads.reserve(consumersCount);
	brokersThreads.reserve(brokersCount);
}

Manager::~Manager() {
	delete[] producers;
	delete[] consumers;
	delete[] brokers;
}

void Manager::run() {
	for (int i = 0; i < producersCount; i++) {
		producersThreads.push_back(std::thread(&Producer::run, &producers[i]));
	}
	for (int i = 0; i < consumersCount; i++) {
		consumersThreads.push_back(std::thread(&Consumer::run, &consumers[i]));
	}
	for (int i = 0; i < brokersCount; i++) {
		brokersThreads.push_back(std::thread(&Broker::run, &brokers[i]));
	}
}

void Manager::end() {
	for (int i = 0; i < producersCount; i++) {
		producers[i].end();
		producersThreads[i].join();
	}
	for (int i = 0; i < consumersCount; i++) {
		consumers[i].end();
		consumersThreads[i].join();
	}
	for (int i = 0; i < brokersCount; i++) {
		brokers[i].end();
		brokersThreads[i].join();
	}
}
