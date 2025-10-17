#include <iostream>
#include "lru/LRUCache.hpp"

int main() {
	LRUCache<int, std::string> cache(2);
	cache.put(1, "one");
	cache.put(2, "two");

	auto val = cache.get(1);
	if (val) {
		std::cout << "Got: " << *val << "\n";
	}
	else {
		std::cout << "Miss\n";
	}

	return 0;
}