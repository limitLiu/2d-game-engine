#include "Event/Bus.hpp"

#include <spdlog/spdlog.h>

using namespace Engine;

Event::Bus::Bus() { spdlog::info("Event Bus constructor called!"); }

Event::Bus::~Bus() { spdlog::info("Event Bus destructor called!"); }

void Event::Bus::reset() { m_subscribers.clear(); }
