#pragma once

class Resource {
public:
	const unsigned char* data;
	unsigned int size;

	Resource() = default;

	Resource(const unsigned char* data, const unsigned int size) {
		this->data = data;
		this->size = size;
	}
};