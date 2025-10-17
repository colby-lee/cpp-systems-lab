#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <optional>
#include <cassert>

template <typename DataType, typename KeyType>
class Node {
public:
	Node(const KeyType& key,
		const DataType& data,
		std::shared_ptr<Node<DataType, KeyType>> next = nullptr,
		std::weak_ptr<Node<DataType, KeyType>> previous = std::weak_ptr<Node<DataType, KeyType>>())
		: data(data), key(key), previous(previous), next(next) {}

	DataType data;
	KeyType key;
	std::weak_ptr<Node<DataType, KeyType>> previous;
	std::shared_ptr<Node<DataType, KeyType>> next;
};

template <typename KeyType, typename ValueType>
class LRUCache {
public:
	LRUCache(int capacity = 10) : capacity(capacity), size(0) {}

	void setCapacity(int c) {
		capacity = c;
		if (capacity <= 0) {
			map.clear();
			head.reset();
			tail.reset();
			size = 0;
		}

		while (size > capacity) {
			if (tail) { map.erase(tail->key); }
			removeTail();
			--size;
		}
	}

	ValueType& operator[](const KeyType& key) {
		// TODO: revisit this logic, 
		// I'm not sure this is the expected behavior
		// of [] operator in std::unordered_map

		assert(capacity > 0);

		if (auto it = map.find(key); it != map.end()) {
			auto node = it->second;
			setNodeToHead(node);
			return node->data;
		}

		// if key exists, put() will move to head of list
		put(key, ValueType{});

		return map[key]->data;
	}

	void put(KeyType key, ValueType value) {
		if (capacity <= 0) return;
		// if key exists, update and reposition
		if (map.find(key) != map.end()) {
			auto node = map[key];
			node->data = value;
			setNodeToHead(node);
			return;
		}

		auto node = std::make_shared<Node<ValueType, KeyType>>(key, value);

		if (atCapacity()) {
			if (tail) {
				map.erase(tail->key);
			}
			removeTail();
			--size;
		}

		map[key] = node;
		setNodeToHead(node);
		size++;
	}

	std::optional<ValueType> get(KeyType key) {
		auto it = map.find(key);
		if (it == map.end()) return std::nullopt;

		auto node = it->second;
		setNodeToHead(node);
		return node->data;
	}

private:
	int capacity;
	int size;
	std::shared_ptr<Node<ValueType, KeyType>> head;
	std::shared_ptr<Node<ValueType, KeyType>> tail;
	std::unordered_map<KeyType, std::shared_ptr<Node<ValueType, KeyType>>> map;

	bool atCapacity() {
		return size >= capacity;
	}

	void setNodeToHead(std::shared_ptr<Node<ValueType, KeyType>> node) {
		// if node is already head, don't do anything
		if (node == head) {
			return;
		}

		// detach node and clean up pointers for list
		{
			if (auto prev = node->previous.lock()) {
				prev->next = node->next;
			}
			if (node->next) {
				node->next->previous = node->previous;
			}
		}

		// if node is tail, update
		if (node == tail) {
			tail = node->previous.lock();
		}

		// move the ndoe to head
		auto oldHead = head;
		node->previous.reset();
		node->next = oldHead;

		if (oldHead) {
			oldHead->previous = node;
		}

		head = node;
		// if list was empty, update tail
		if (!tail) {
			tail = head;
		}
	}

	void removeTail() {
		// if tail doesn't exist, do nothing
		if (!tail) {
			return;
		}

		// convert weak_ptr to shared_ptr, increments ref count
		auto newTail = tail->previous.lock();

		if (newTail) {
			newTail->next = nullptr;
		}
		// update to point to new tail
		tail = newTail;
	}
};