#ifndef POOL_HPP
#define POOL_HPP

#include "Common.hpp"
#include <vector>

namespace Engine::ECS {
class IPool {
public:
  virtual ~IPool() {}
};

template <class T> struct Pool : public IPool {
  explicit Pool(const usize n = 128) { resize(n); }
  virtual ~Pool() = default;

  bool isEmpty() const { return m_data.empty(); }

  usize size() { return m_data.size(); }

  void resize(usize n) { m_data.resize(n); }

  void clear() { m_data.clear(); }

  void add(T element) { m_data.emplace_back(element); }

  void set(usize index, T value) { m_data[index] = value; }

  T &get(usize index) { return m_data[index]; }

  T &operator[](usize index) { return m_data[index]; }

private:
  std::vector<T> m_data;
};
} // namespace Engine::ECS

#endif
