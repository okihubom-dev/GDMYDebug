# Getting Started

[English](#english) | [中文](#中文)

---

## English

### 1. Requirements

- Godot Engine source code
- A C++ build environment supported by Godot
- GDMYDebug

### 2. Installation

Copy the `GDMYDebug` directory into the `modules` directory of
your Godot Engine source tree.

Or, add it as a Git submodule.

```text
godot/
└── modules/
    └── GDMYDebug/
````

### 3. Build Godot

Build Godot from source as usual.

After the build completes, GDMYDebug will be compiled into the
Godot Engine.

### 4. Verify the Installation

Create or open a project using the newly built Godot executable.

The `GDMYDebug` Engine singleton should be available from GDScript:

```gdscript
func _process(_delta):
    GDMYDebug.set_print_pos_xy(20, 20)
    GDMYDebug.print("Hello, GDMYDebug!")
```

If the text appears on the screen, GDMYDebug has been installed
successfully.

### 5. Basic Usage

For runtime debug printing, add the code to a function that is
called every frame, such as `_process()` or `_physics_process()`.

```gdscript
func _process(_delta):
    GDMYDebug.set_print_pos_xy(20, 20)
    GDMYDebug.print("Hello, GDMYDebug!")

    GDMYDebug.set_print_pos_xy(20, 40)
    GDMYDebug.set_print_color_rgba(255, 0, 0)
    GDMYDebug.print("Red text")
```

Performance statistics only need to be enabled once:

```gdscript
func _ready():
    GDMYDebug.set_perf_stats_enabled(true)
```

For more available functions, see
[API Reference](api-reference.md).

---

## 中文

### 1. 环境要求

* Godot Engine 源码
* Godot 支持的 C++ 构建环境
* GDMYDebug

### 2. 安装

将 GDMYDebug 目录放入 Godot Engine 源码的 modules 目录中。

或者将其作为 Git submodule 添加。

```text
godot/
└── modules/
    └── GDMYDebug/
```

### 3. 编译 Godot

按照正常方式从源码编译 Godot。

编译完成后，GDMYDebug 会被直接编译进 Godot Engine。

### 4. 确认安装

使用刚刚编译出的 Godot 可执行文件创建或打开一个项目。

此时应该可以在 GDScript 中直接访问 `GDMYDebug` Engine Singleton：

```gdscript
func _process(_delta):
    GDMYDebug.set_print_pos_xy(20, 20)
    GDMYDebug.print("Hello, GDMYDebug!")
```

如果文字成功显示在屏幕上，就说明 GDMYDebug 已经安装成功。

### 5. 基本使用

如果要进行运行时调试文字输出，
请将代码添加到每帧都会执行的函数中，例如 `_process()` 或 `_physics_process()`。

```gdscript
func _process(_delta):
    GDMYDebug.set_print_pos_xy(20, 20)
    GDMYDebug.print("Hello, GDMYDebug!")

    GDMYDebug.set_print_pos_xy(20, 40)
    GDMYDebug.set_print_color_rgba(255, 0, 0)
    GDMYDebug.print("Red text")
```

性能统计只需要启用一次：

```gdscript
func _ready():
    GDMYDebug.set_perf_stats_enabled(true)
```

更多可用功能请参考
[API 参考](api-reference.md)。

---
