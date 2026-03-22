#ifndef BUS_HPP
#define BUS_HPP

#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "Event/Base.hpp"

namespace Engine::Event {
class IEventCallback {
public:
  virtual ~IEventCallback() = default;

  void execute(Base &event) { call(event); }

private:
  virtual void call(Base &event) = 0;
};

template <class Owner, class T> struct EventCallback : public IEventCallback {
  using Callback = void (Owner::*)(const T &);

  EventCallback(Owner *owner, const Callback callback) : owner{owner}, callback{callback} {}

  ~EventCallback() override = default;

private:
  Owner *owner;

  Callback callback;

  void call(Base &event) override { std::invoke(callback, owner, static_cast<T &>(event)); }
};

using HandlerList = std::list<std::unique_ptr<IEventCallback>>;

struct Bus {
  Bus();

  ~Bus();

  template <class T, class Owner> void subscribe(Owner *owner, void (Owner::*callback)(const T &)) {
    if (m_subscribers[typeid(T)].get() == nullptr) {
      m_subscribers[typeid(T)] = std::make_unique<HandlerList>();
    }
    auto subscriber = std::make_unique<EventCallback<Owner, T>>(owner, callback);
    m_subscribers[typeid(T)]->emplace_back(std::move(subscriber));
  }

  template <class T, class... Args> void emit(Args &&...args);

  void reset();

private:
  std::map<std::type_index, std::unique_ptr<HandlerList>> m_subscribers;
};

template <class T, class... Args> void Bus::emit(Args &&...args) {
  auto it = m_subscribers.find(typeid(T));
  if (it != m_subscribers.end() && it->second != nullptr) {
    for (auto &handler : *(it->second)) {
      T event(std::forward<Args>(args)...);
      handler->execute(event);
    }
  }
}
} // namespace Engine::Event

#endif
