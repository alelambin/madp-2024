#include "producer.h"

unsigned int Producer::idCounter = 0;

Producer::Producer() {
	id = idCounter;
	idCounter++;
	alive = true;
}

void Producer::addConsumersCount(int count) {
	consumersCount = count;
}

void Producer::run() {
	while (true) {
		unsigned long sleepTime = random(1, MAX_SLEEP);
		DEBUG("Producer " + str(id) + ": sleep for " + str(sleepTime) + "ms");
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		if (!alive) {
			break;
		}
		Message message = { random(0, consumersCount - 1), std::string("Message from ") + std::to_string(id) };
		SharedQueue<Message>::getInstance()->insert(message);
		LOG("Producer " + str(id) + " send message \"" + message.data + "\" to consumer " + str(message.targetId));
	}
}

void Producer::end() {
	alive = false;
	DEBUG("Producer " + str(id) + ": end");
}
