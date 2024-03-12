#pragma once

#include <mutex>
#include "logger.h"

template<class T> 
class SharedNode {
public:
	T data;
	SharedNode *next;
	SharedNode *prev;
	
	SharedNode(T data) : data(data) {
		next = nullptr;
		prev = nullptr;
	}
};

template<class T>
class SharedQueue {
	static SharedQueue *instance;
	SharedNode<T> *head;
	SharedNode<T> *tail;
	int size;
	std::mutex mutex;
	
	SharedQueue() {
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

public:
	static SharedQueue *getInstance() {
		if (!instance) {
			instance = new SharedQueue();
		}
		return instance;
	}

	~SharedQueue() {
		SharedNode<T> *next = nullptr;
		while (head != nullptr) {
			next = head->next;
			delete head;
			head = next;
		}
		delete instance;
	}

	void insert(T data) {
		mutex.lock();
		DEBUG("SharedQueue: inserting object {" + str(data) + "}...");
		if (isEmpty()) {
			head = new SharedNode<T>(data);
			tail = head;
		} else {
			tail->next = new SharedNode<T>(data);
			tail->next->prev = tail;
			tail = tail->next;
		}
		size++;
		DEBUG("SharedQueue: object inserted (current queue size is " + str(size) + ")");
		mutex.unlock();
	}

	bool get(T& data) {
		mutex.lock();
		if (isEmpty()) {
			mutex.unlock();
			return false;
		}

		data = head->data;
		DEBUG("SharedQueue: rejecting object {" + str(data) + "}...");
		if (head == tail) {
			delete head;
			head = nullptr;
			tail = nullptr;
		} else {
			head->next->prev = nullptr;
			auto next = head->next;
			delete head;
			head = next;
		}
		size--;
		DEBUG("SharedQueue: object rejected (current queue size is " + str(size) + ")");
		mutex.unlock();
		return true;
	}

	bool isEmpty() const {
		return size == 0;
	}

};

template<class T>
SharedQueue<T> *SharedQueue<T>::instance = 0;
