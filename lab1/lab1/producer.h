#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "rand.h"
#include "message.h"
#include "sharedQueue.h"
#include "logger.h"

class Producer {
	static unsigned int idCounter;
	unsigned int id;
	std::atomic_bool alive;
	int consumersCount;

public:
	Producer();
	~Producer() = default;
	void addConsumersCount(int count);
	void run();
	void end();

};

