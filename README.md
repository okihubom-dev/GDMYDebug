# GDMYDebug

(*AI generated documentation*)

[![YouTube | DEMO GDMYDebug](https://img.youtube.com/vi/BpAqvI6E5eE/maxresdefault.jpg)](https://www.youtube.com/watch?v=BpAqvI6E5eE)

[English](#english) | [中文](#中文)

---

## English

GDMYDebug is a native C++ module for Godot Engine that provides
runtime on-screen debug output and performance statistics.

It is designed for projects built from the Godot Engine source code.

### Features

- Runtime on-screen debug printing
- Custom print position and color
- FPS / RAM / VRAM performance statistics
- Global `GDMYDebug` Engine singleton
- Automatically disabled in non-Debug/Development builds (NOT tested yet, but this is the expected behavior)

### Quick Example

```gdscript
# Feature: Runtime on-screen debug printing

# Add the GDScript code below to a function that is called every frame,
# such as _process() or _physics_process().

GDMYDebug.set_print_pos_xy(20, 20)
GDMYDebug.print("Hello, GDMYDebug!")

GDMYDebug.set_print_pos_xy(20, 40)
GDMYDebug.set_print_color_rgba(255, 0, 0)
GDMYDebug.print("Red text")


# Feature: Performance statistics

# Add the GDScript code below to a function that is called once,
# such as _ready().

GDMYDebug.set_perf_stats_enabled(true)
```

### Documentation

- [Getting Started](docs/getting-started.md)
- [API Reference](docs/api-reference.md)
- [Demo](docs/demo.md)
- [Advanced Usage](docs/advanced-usage.md)
- [FAQ](docs/faq.md)

### Requirements

- Godot Engine source code
- A source build of Godot with the GDMYDebug module included

GDMYDebug is currently implemented as a native Godot Engine Module
and does NOT support GDExtension.

---

## 中文

GDMYDebug 是一个基于 C++ 的 Godot Engine 原生 Module，
用于提供运行时屏幕调试输出和性能统计功能。

### 功能

- 运行时屏幕调试文字输出
- 自定义文字位置和颜色
- FPS / RAM / VRAM 性能统计
- 全局 `GDMYDebug` Engine Singleton
- 在非 Debug/Development 构建中自动禁用 (其实还**未测试**, 但预计应该会是这样的行为)

### 快速使用

```gdscript
# 功能: 运行时屏幕调试文字输出

# 把下列 GDScript代码添加到某个每帧会被执行的函数里，
# 例如，_process() 或者 _physics_process()。

GDMYDebug.set_print_pos_xy(20, 20)
GDMYDebug.print("Hello, GDMYDebug!")

GDMYDebug.set_print_pos_xy(20, 40)
GDMYDebug.set_print_color_rgba(255, 0, 0)
GDMYDebug.print("Red text")

# 功能: 性能统计

# 把下列 GDScript代码添加到某个只执行一次的函数里，
# 例如 _ready()。

GDMYDebug.set_perf_stats_enabled(true)
```

### 文档

- [快速开始](docs/getting-started.md)
- [API 参考](docs/api-reference.md)
- [常见问题](docs/faq.md)
- [高级用法](docs/advanced-usage.md)

### 环境要求

- Godot Engine 源码
- 包含 GDMYDebug Module 的 Godot 源码构建环境

GDMYDebug 目前是直接编译进 Godot 的原生 Module，
暂不支持 GDExtension。
