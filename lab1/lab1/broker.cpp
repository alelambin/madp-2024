#include "broker.h"

unsigned int Broker::idCounter = 0;

Broker::Broker() {
	id = idCounter;
	idCounter++;
	alive = true;
}

void Broker::run() {
	while (alive) {
		Message message;
		if (!SharedQueue<Message>::getInstance()->get(message)) {
			continue;
		}

		bool send = false;
		if (Consumer::consumersMap.find(message.targetId) != Consumer::consumersMap.end()) {
			auto &target = Consumer::consumersMap[message.targetId];
			auto start = std::chrono::high_resolution_clock::now();
			while (true) {
				send = target->addMessage(message);
				auto time = std::chrono::high_resolution_clock::now() - start;
				if (send || std::chrono::duration_cast<std::chrono::milliseconds>(time).count() > MAX_WAIT) {
					break;
				}
			}
		}
		
		if (send) {
			LOG("Broker " + str(id) + " send message \"" + message.data + "\" to consumer " + str(message.targetId));
		} else {
			LOG("Broker " + str(id) + " drop message \"" + message.data + "\" to consumer " + str(message.targetId));
		}

	}
}

void Broker::end() {
	alive = false;
	DEBUG("Broker " + str(id) + ": end");
}
