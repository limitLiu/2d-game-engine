#ifndef POOL_HPP
#define POOL_HPP

#include "Common.hpp"
#include <vector>

namespace ecs {

class IPool {
public:
  virtual ~IPool() {}
};

template <class T> struct Pool : public IPool {
  Pool(usize n = 128) { resize(n); }
  virtual ~Pool() = default;

  bool isEmpty() const { return _data.empty(); }

  usize size() { return _data.size(); }

  void resize(usize n) { _data.resize(n); }

  void clear() { _data.clear(); }

  void add(T element) { _data.emplace_back(element); }

  void set(usize index, T value) { _data[index] = value; }

  T &get(usize index) { return _data[index]; }

  T &operator[](usize index) { return _data[index]; }

private:
  std::vector<T> _data;
};
} // namespace ecs

#endif
