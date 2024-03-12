#include "consumer.h"

unsigned int Consumer::idCounter = 0;
std::map<int, Consumer *> Consumer::consumersMap = {};

Consumer::Consumer() {
	id = idCounter;
	idCounter++;
	alive = true;
	messagePool = nullptr;
	consumersMap.insert({ id, this });
}

Consumer::~Consumer() {
	if (messagePool) {
		delete[] messagePool;
	}
}

void Consumer::addPoolCapacity(int capacity) {
	messagePoolCapacity = capacity;
	messagePool = new Message[messagePoolCapacity];
	messagePoolSize = 0;
}

void Consumer::run() {
	while (true) {
		unsigned long sleepTime = random(1, MAX_SLEEP);
		DEBUG("Consumer " + str(id) + ": sleep for " + str(sleepTime) + "ms");
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		if (!alive) {
			break;
		}
		mutex.lock();
		if (messagePoolSize > 0) {
			Message message = messagePool[--messagePoolSize];
			LOG("Consumer " + str(id) + " recieve message \"" + message.data + "\"");
		}
		mutex.unlock();
	}
}

void Consumer::end() {
	alive = false;
	DEBUG("Consumer " + str(id) + ": end");
}

bool Consumer::addMessage(Message message) {
	mutex.lock();
	if (messagePoolSize == messagePoolCapacity) {
		mutex.unlock();
		return false;
	} else {
		messagePool[messagePoolSize++] = message;
		mutex.unlock();
		return true;
	}
}
