#pragma once

#include <atomic>
#include <mutex>
#include "consumer.h"
#include "message.h"
#include "sharedQueue.h"
#include "logger.h"

#define MAX_WAIT 1000

class Broker {
	static unsigned int idCounter;
	unsigned int id;
	std::atomic_bool alive;
	std::mutex mutex;

public:
	Broker();
	~Broker() = default;
	void run();
	void end();

};

