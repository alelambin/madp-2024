#pragma once

#include <map>
#include <atomic>
#include <mutex>
#include "rand.h"
#include "message.h"
#include "sharedQueue.h"
#include "logger.h"

class Consumer {
	static unsigned int idCounter;
	unsigned int id;
	std::atomic_bool alive;
	Message *messagePool;
	int messagePoolSize;
	int messagePoolCapacity;
	std::mutex mutex;

public:
	Consumer();
	~Consumer();
	void addPoolCapacity(int capacity);
	void run();
	void end();
	bool addMessage(Message message);

	static std::map<int, Consumer *> consumersMap;

};

