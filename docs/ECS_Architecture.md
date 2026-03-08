# ECS 架构代码解读

## 整体架构

这是一个经典的 **Entity-Component-System** 实现，采用 **"面向数据"** 的设计思想。核心设计原则是：

- **Entity**: 只是一个轻量级的 ID
- **Component**: 纯数据结构，无行为
- **System**: 包含行为，处理拥有特定组件的实体

---

## 1. Entity（实体）

```cpp
// include/Core/Base/Entity.hpp
struct Entity {
  Entity(u32 id) : _id(id) {}
  u32 id() const;
  // ... 比较运算符
private:
  u32 _id;
};
```

**代码层面解读：**
- `Entity` 只是一个 `u32` ID 的包装器
- 支持各种比较运算符，用于在容器（如 `std::set`）中存储
- 实体本身没有任何数据和行为，只是一个标识符

---

## 2. Component（组件）

### 2.1 组件类型注册机制（CRTP 模式）

```cpp
// include/Core/Base/Component.hpp
struct IComponent {
protected:
  static u32 nextId;  // 全局静态计数器，初始值为 0
};

template <class T> 
struct Component : public IComponent {
  static u32 GetId() {
    static auto id = nextId++;  // 首次调用时分配唯一 ID
    return id;
  }
};
```

**代码层面解读：**

这是一个巧妙的 **CRTP（Curiously Recurring Template Pattern）** 用法：

| 机制 | 说明 |
|------|------|
| `IComponent::nextId` | 所有组件类型共享的静态计数器 |
| `Component<T>::GetId()` | 每个具体类型 `T` 有独立的静态变量 `id` |
| 延迟初始化 | `static auto id = nextId++` 确保每种类型只分配一次 ID |

**示例流程：**
```cpp
// 假设依次调用：
TransformComponent::GetId()  // 返回 0，nextId 变为 1
RigidBodyComponent::GetId()  // 返回 1，nextId 变为 2
TransformComponent::GetId()  // 返回 0（已初始化，不再递增）
```

### 2.2 组件定义

```cpp
// include/Core/Components/TransformComponent.hpp
struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  f64 rotation;
  // ... 构造函数
};
```

组件就是**纯数据结构**（POD），没有任何方法逻辑。

---

## 3. Signature（签名）

```cpp
// include/Common.hpp
const u32 MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
```

**代码层面解读：**
- 使用 `std::bitset<32>` 表示实体拥有哪些组件
- 第 `i` 位为 1 表示拥有 `id == i` 的组件
- 支持位运算（`&`, `|`, `^`），用于快速匹配

**示例：**
```cpp
Signature sig;
sig.set(0);  // 拥有 Transform（假设其 ID 为 0）
sig.set(2);  // 拥有 Sprite（假设其 ID 为 2）
// sig = 0b000...0101
```

---

## 4. Pool（内存池）

```cpp
// include/Core/Base/Pool.hpp
template <class T> 
struct Pool : public IPool {
  std::vector<T> _data;
  
  void set(usize index, T value) { _data[index] = value; }
  T &get(usize index) { return _data[index]; }
  void resize(usize n) { _data.resize(n); }
  // ...
};
```

**代码层面解读：**
- 每个组件类型有一个独立的 `Pool<T>`
- 使用 `std::vector` 连续存储，保证缓存友好
- 通过 `entity.id()` 作为索引直接访问（O(1)）
- 非类型擦除：每个 Pool 存储具体类型，避免 cast 开销

---

## 5. System（系统）

```cpp
// include/Core/Base/System.hpp
struct System {
  std::vector<Entity> _entities;        // 该系统关心的实体列表
  Signature _componentSignature;        // 系统需要的组件签名
  
  void requireComponent<T>() {
    const auto componentId = Component<T>::GetId();
    _componentSignature.set(componentId);
  }
};
```

**代码层面解读：**
- 系统声明自己**关心哪些组件**（通过 `requireComponent`）
- 运行时，Registry 将匹配的实体添加到 `_entities`
- 具体系统（如 `MovementSystem`）继承 `System`，实现 `update()` 方法

---

## 6. Registry（注册表/中央管理器）

这是整个 ECS 的核心，负责协调所有部分：

```cpp
// include/Core/Base/Registry.hpp
struct Registry {
  u32 _entitiesCount = 0;
  
  // 组件存储：索引是 componentId，值是对应类型的 Pool
  std::vector<std::shared_ptr<IPool>> _componentPools;
  
  // 每个实体的组件签名：索引是 entityId
  std::vector<Signature> _entityComponentSignatures;
  
  // 系统存储：key 是类型索引，value 是 System 实例
  std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;
  
  // 延迟添加/删除队列
  std::set<Entity> _entitiesToBeAdded;
  std::set<Entity> _entitiesToBeKilled;
};
```

### 6.1 创建实体

```cpp
// src/Core/Registry.cc
Entity Registry::createEntity() {
  auto entityId = _entitiesCount++;
  Entity entity(entityId);
  _entitiesToBeAdded.insert(entity);  // 延迟处理
  
  if (entityId >= _entityComponentSignatures.size()) {
    _entityComponentSignatures.resize(entityId + 1);
  }
  return entity;
}
```

**设计要点：** 使用**延迟添加**模式，避免在迭代过程中修改容器。

### 6.2 添加组件

```cpp
template <class T, class... Args>
void Registry::addComponent(Entity entity, Args &&...args) {
  const auto componentId = Component<T>::GetId();
  const auto entityId = entity.id();

  // 1. 确保 Pool 容器足够大
  if (componentId >= _componentPools.size()) {
    _componentPools.resize(componentId + 1, nullptr);
  }

  // 2. 延迟创建 Pool（首次使用时创建）
  if (_componentPools[componentId] == nullptr) {
    _componentPools[componentId] = std::make_shared<Pool<T>>();
  }

  // 3. 获取/创建具体类型的 Pool
  auto pool = std::static_pointer_cast<Pool<T>>(_componentPools[componentId]);

  // 4. 确保 Pool 能容纳该实体
  if (entityId >= pool->size()) {
    pool->resize(_entitiesCount);
  }

  // 5. 放置组件数据
  T newComponent(std::forward<Args>(args)...);
  pool->set(entityId, newComponent);

  // 6. 更新实体的组件签名
  _entityComponentSignatures[entityId].set(componentId);
}
```

**代码层面解读：**

| 步骤 | 目的 |
|------|------|
| 延迟创建 Pool | 只有真正使用的组件类型才会分配内存 |
| `std::static_pointer_cast` | 从 `IPool` 安全转换为 `Pool<T>` |
| `entityId` 作为索引 | O(1) 直接访问，无哈希查找开销 |
| 完美转发 `Args` | 支持任意构造函数参数 |

### 6.3 添加系统

```cpp
template <class T, class... Args>
void Registry::addSystem(Args &&...args) {
  std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<Args>(args)...);
  _systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}
```

**关键点：**
- 使用 `std::type_index` 作为 key，实现**类型安全的运行时识别**
- 每种系统类型只能有一个实例（`unordered_map` 的特性）

### 6.4 实体-系统匹配（核心算法）

```cpp
// src/Core/Registry.cc
void Registry::addEntity2Systems(Entity entity) {
  const auto entityId = entity.id();
  const auto entityComponentSignature = _entityComponentSignatures[entityId];
  
  for (auto &system : _systems) {
    const auto &systemComponentSignature = system.second->componentSignature();
    
    // 关键：位运算判断系统是否关心该实体
    bool isInterested = (entityComponentSignature & systemComponentSignature) 
                        == systemComponentSignature;
    
    if (isInterested) {
      system.second->addEntity(entity);
    }
  }
}
```

**算法解析：**

假设：
- 实体签名：`0b000011`（有 Transform + RigidBody）
- 系统 A 签名：`0b000001`（需要 Transform）→ 匹配 ✓
- 系统 B 签名：`0b000101`（需要 Transform + Collider）→ 不匹配 ✗

```
实体:    000011
系统A:   000001
& 运算:  000001  == 系统A签名 → 匹配

系统B:   000101
& 运算:  000001  != 系统B签名 → 不匹配
```

### 6.5 Update 循环

```cpp
void Registry::update() {
  // 处理延迟添加的实体
  for (auto entity : _entitiesToBeAdded) {
    addEntity2Systems(entity);
  }
  _entitiesToBeAdded.clear();
}
```

---

## 7. 使用流程示例

```cpp
// 1. 创建 Registry
auto registry = std::make_unique<ecs::Registry>();

// 2. 注册系统
registry->addSystem<ecs::MovementSystem>();
auto& movementSystem = registry->getSystem<ecs::MovementSystem>();
movementSystem.requireComponent<ecs::TransformComponent>();

// 3. 创建实体并添加组件
auto entity = registry->createEntity();
registry->addComponent<ecs::TransformComponent>(
    entity, 
    glm::vec2(100, 100),  // position
    glm::vec2(1, 1),      // scale
    0.0                   // rotation
);

// 4. 更新 Registry（将实体添加到匹配的系统）
registry->update();

// 5. 系统更新
for (auto& entity : movementSystem.entities()) {
    // 处理逻辑...
}
```

---

## 8. 设计亮点与限制

### 亮点

| 特性 | 实现方式 |
|------|---------|
| 类型安全 | 模板 + `std::type_index` |
| O(1) 组件访问 | Entity ID 作为数组索引 |
| 缓存友好 | `std::vector` 连续存储 |
| 延迟修改 | `entitiesToBeAdded/Killed` 队列 |
| 自动组件 ID | CRTP + 静态变量 |

### 限制

| 限制 | 说明 |
|------|------|
| 最大 32 种组件 | `MAX_COMPONENTS = 32`，可改 |
| 实体 ID 只增不减 | 暂未实现 ID 复用 |
| 无组件删除验证 | `removeComponent` 只修改 bitset |
| 系统间通信 | 需要额外机制（如 Event Bus）|

---

## 文件结构

```
include/
├── Common.hpp              # 基础类型定义、Signature
├── Core/
│   ├── Base/
│   │   ├── Component.hpp   # 组件基类、CRTP ID 分配
│   │   ├── Entity.hpp      # 实体（轻量级 ID）
│   │   ├── Pool.hpp        # 组件内存池
│   │   ├── Registry.hpp    # ECS 中央管理器
│   │   └── System.hpp      # 系统基类
│   ├── Components/
│   │   └── TransformComponent.hpp
│   ├── Systems/
│   │   └── MovementSystem.hpp
│   └── Game.hpp
└── src/
    └── Core/
        ├── Component.cc    # nextId 初始化
        ├── Entity.cc       # 运算符实现
        ├── Game.cc
        ├── Registry.cc     # 核心逻辑实现
        └── System.cc       # 实体列表管理
```

---

> 这是一个**简洁、高效、易于理解**的 ECS 实现，适合作为 2D 游戏引擎的基础。
