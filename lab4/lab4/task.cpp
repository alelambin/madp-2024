#include "task.h"

int64_t task::getSequenceNext(int64_t value) {
	return value % 2 == 0 ? value / 2 : 3 * value + 1;
}

uint64_t task::findSequenceLength(int64_t value) {
	uint64_t length = 1;
	while (value != 1) {
		value = getSequenceNext(value);
		length++;
	}
	return length;
}
