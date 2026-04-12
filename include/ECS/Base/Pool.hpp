#ifndef POOL_HPP
#define POOL_HPP

#include "Common.hpp"
#include <unordered_map>
#include <vector>

namespace Engine::ECS {
class IPool {
public:
  virtual ~IPool() = default;
  virtual void removeEntityId(usize entityId) = 0;
};

template <class T> struct Pool : public IPool {
  explicit Pool(const usize capacity = 128) {
    m_size = 0;
    resize(capacity);
  }

  virtual ~Pool() = default;

  bool isEmpty() const { return m_size == 0; }

  usize size() const { return m_size; }

  void resize(usize n) { m_data.resize(n); }

  void clear() {
    m_data.clear();
    m_size = 0;
  }

  void add(T element) { m_data.emplace_back(element); }

  void set(usize entityId, T value) {
    if (m_entityIdToIndex.contains(entityId)) {
      const auto index = m_entityIdToIndex[entityId];
      m_data[index] = value;
    } else {
      usize index = m_size;
      m_entityIdToIndex.emplace(entityId, index);
      m_indexToEntityId.emplace(index, entityId);
      if (index >= m_data.capacity()) {
        resize(m_size << 1);
      }
      m_data[index] = value;
      ++m_size;
    }
  }

  T &get(usize entityId) {
    const auto index = m_entityIdToIndex[entityId];
    return m_data[index];
  }

  T &operator[](usize index) { return m_data[index]; }

  auto remove(usize entityId) -> void {
    auto willRemoveIndex = m_entityIdToIndex[entityId];
    auto lastIndex = m_size - 1;
    m_data[willRemoveIndex] = m_data[lastIndex];
    auto lastEntityId = m_indexToEntityId[lastIndex];
    m_entityIdToIndex[lastEntityId] = willRemoveIndex;
    m_indexToEntityId[willRemoveIndex] = lastEntityId;
    m_entityIdToIndex.erase(entityId);
    m_indexToEntityId.erase(lastIndex);
    --m_size;
  }

  void removeEntityId(usize entityId) override {
    if (m_entityIdToIndex.contains(entityId)) {
      remove(entityId);
    }
  }

private:
  std::vector<T> m_data;
  usize m_size;
  std::unordered_map<usize, usize> m_entityIdToIndex;
  std::unordered_map<usize, usize> m_indexToEntityId;
};
} // namespace Engine::ECS

#endif
