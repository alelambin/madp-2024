long get_sequence_next(long value) {
	return value % 2 == 0 ? value / 2 : 3 * value + 1;
}

__kernel void find_sequence_length(int data_size, __global long* data) {
	ulong global_id = get_global_id(0);
	if (global_id < data_size) {
		long value = data[global_id];
		ulong length = 1;
		while (value != 1) {
			value = get_sequence_next(value);
			length++;
		}
		data[global_id] = length;
	}
}