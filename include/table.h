#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <typename TypeKey, typename T>
class Iterator;

template <typename TypeKey, typename TypeData>
class baseTable {
 public:
  virtual Iterator<TypeKey, TypeData> find(const TypeKey& key) = 0;
  virtual Iterator<TypeKey, TypeData> insert(const TypeKey& key, const TypeData& data) = 0;
  virtual bool remove(const TypeKey& key) = 0;
  virtual Iterator<TypeKey, TypeData> begin() = 0;
  virtual Iterator<TypeKey, TypeData> end() = 0;
  virtual int size() = 0;
};

template <typename TypeKey, typename TypeData>
class Table : public baseTable<TypeKey, TypeData> {
 protected:
  vector<pair<TypeKey, TypeData>> storage;

 public:
  Table() { storage = vector<pair<TypeKey, TypeData>>(); }

  Table(int size) { storage = vector<pair<TypeKey, TypeData>>(size); }

  Table(const Table& other) { storage = other.storage; }

  virtual ~Table() { storage.clear(); }

  Table& operator=(const Table& other) {
    if (this == &other) return *this;
    storage = other.storage;
    return *this;
  }

  virtual TypeData& operator[](const TypeKey& key) {
    for (int i = 0; i < storage.size(); i++) {
      if (storage[i].first == key) return storage[i].second;
    }

    storage.push_back(make_pair(key, TypeData()));

    return storage.back().second;
  }

  Iterator<TypeKey, TypeData> find(const TypeKey& key) {
    for (int i = 0; i < storage.size(); i++) {
      if (storage[i].first == key) return begin() + i;
    }
    return end();
  }
  int size() { return storage.size(); }

  Iterator<TypeKey, TypeData> insert(const TypeKey& key, const TypeData& data) {
    int i = storage.size();
    storage.push_back(make_pair(key, data));
    i--;
    return begin() + i;
  }

  bool remove(const TypeKey& key) {
    for (int i = 0; i < storage.size(); i++) {
      if (storage[i].first == key) {
        storage.erase(storage.begin() + i);
        return true;
      }
    }
    return false;
  }

  Iterator<TypeKey, TypeData> begin() { return Iterator<TypeKey, TypeData>(storage[0]); }

  Iterator<TypeKey, TypeData> end() {
    Iterator<TypeKey, TypeData> a = begin();
    return a + storage.size();
  }
};

template <typename TypeKey, typename T>
class Iterator {
  friend class Table<TypeKey, T>;

 protected:
  pair<TypeKey, T>* iterator;

 public:
  Iterator(pair<TypeKey, T>& data) { iterator = &data; }

  Iterator(const Iterator& other) { iterator = other.iterator; }

  T& operator*() const { return iterator->second; }

  T* operator->() const { return iterator.second; }

  virtual Iterator& operator++() {
    iterator++;
    return *this;
  }

  Iterator& operator--() {
    iterator--;
    return *this;
  }

  Iterator operator+(int offset) {
    Iterator<TypeKey, T> tmp = *this;
    tmp.iterator += offset;
    return tmp;
  }

  bool operator==(const Iterator& other) { return iterator == other.iterator; }

  bool operator!=(const Iterator& other) { return iterator != other.iterator; }
};