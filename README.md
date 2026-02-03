# ECS 架构学习

## 初始化项目

### 使用 Ninja

```shell
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S ./ -B ./build
ninja -C ./build
```

### 不使用 Ninja

```shell
mkdir build && cd build
cmake .. && make
```
