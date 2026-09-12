# API Reference

[English](#english) | [中文](#中文)

---

## English

### Runtime Debug Printing

| Function                                     | Description                                                          |
| -------------------------------------------- | -------------------------------------------------------------------- |
| `print(message)`                             | Prints text on screen.                                               |
| `set_print_pos_xy(pos_x, pos_y)`             | Sets the print position using base-resolution coordinates.           |
| `set_print_pos(pos)`                         | Sets the print position using base-resolution `Vector2` coordinates. |
| `set_print_abs_pos_xy(abs_pos_x, abs_pos_y)` | Sets the print position using actual viewport coordinates.           |
| `set_print_abs_pos(abs_pos)`                 | Sets the print position using actual viewport `Vector2` coordinates. |
| `set_print_color_rgba(r, g, b, a = 255)`     | Sets the print color.                                                |
| `set_print_color(color)`                     | Sets the print color using `Color`.                                  |

`pos` uses a fixed base resolution and is scaled to the current viewport size. This is useful when the position should scale with the resolution.

`abs_pos` uses the actual viewport coordinates and is not scaled based on the base resolution. This is useful when the position should remain at a fixed pixel position.

`pos` uses GDMYDebug's own base-resolution scaling rather than directly following all of Godot's viewport and stretch settings. Because Godot provides various viewport, window, and content-scaling configurations, the result of `pos` may not always match the coordinate behavior of other Godot nodes or UI elements.

If you need to specify an exact position in the current viewport, use `abs_pos` instead.


### Predefined Colors

```gdscript
GDMYDebug.white()
GDMYDebug.black()
GDMYDebug.red()
GDMYDebug.green()
GDMYDebug.blue()
GDMYDebug.cyan()
GDMYDebug.yellow()
GDMYDebug.magenta()
```

Returns the corresponding `Color`.

### Performance Statistics

| Function                                                | Description                                                          |
| ------------------------------------------------------- | -------------------------------------------------------------------- |
| `set_perf_stats_enabled(is_enable_now)`                 | Enables or disables performance statistics.                          |
| `set_perf_stats_font_size(font_size)`                   | Sets the font size.                                                  |
| `set_perf_stats_font_color_rgba(r, g, b, a = 255)`      | Sets the font color.                                                 |
| `set_perf_stats_font_color(color)`                      | Sets the font color using `Color`.                                   |
| `set_perf_stats_print_abs_pos_xy(pos_x, pos_y)`         | Sets the print position using actual viewport coordinates.           |
| `set_perf_stats_print_abs_pos(pos)`                     | Sets the print position using actual viewport `Vector2` coordinates. |
| `reset_perf_stats_config(is_reset_enable_flag = false)` | Resets the performance statistics configuration.                     |

Default performance statistics configuration:

* Enabled: `false`
* Font size: `12`
* Font color: `RGB(221, 34, 136)`
* Position: `(0, 0)`

`reset_perf_stats_config(true)` also resets the enabled state.

---

## 中文

### 运行时调试文字

| 函数                                           | 说明                                    |
| -------------------------------------------- | ------------------------------------- |
| `print(message)`                             | 在屏幕上显示文字。                             |
| `set_print_pos_xy(pos_x, pos_y)`             | 使用基准分辨率坐标设置文字显示位置。                    |
| `set_print_pos(pos)`                         | 使用基准分辨率的 `Vector2` 坐标设置文字显示位置。        |
| `set_print_abs_pos_xy(abs_pos_x, abs_pos_y)` | 使用实际 Viewport 坐标设置文字显示位置。             |
| `set_print_abs_pos(abs_pos)`                 | 使用实际 Viewport 的 `Vector2` 坐标设置文字显示位置。 |
| `set_print_color_rgba(r, g, b, a = 255)`     | 设置文字颜色。                               |
| `set_print_color(color)`                     | 使用 `Color` 设置文字颜色。                    |
| `print_font_size()`                          | 返回默认文字大小（`12`）。                       |

`pos` 使用固定的基准分辨率坐标，并根据当前 Viewport 大小进行缩放。适合希望文字位置随分辨率一起缩放的情况。

`abs_pos` 使用实际的 Viewport 坐标，不会根据基准分辨率进行缩放。适合希望文字保持固定像素位置的情况。

`pos` 使用 GDMYDebug 自己定义的基准分辨率缩放规则，并不会直接跟随 Godot 的所有 Viewport、Window 和 Content Scaling 设置。由于 Godot 提供了多种 Viewport、窗口和缩放配置，`pos` 的坐标行为不一定始终与其他 Godot Node 或 UI 元素保持一致。
如果需要指定当前 Viewport 中的实际位置，请使用 `abs_pos`。


### 预定义颜色

```gdscript
GDMYDebug.white()
GDMYDebug.black()
GDMYDebug.red()
GDMYDebug.green()
GDMYDebug.blue()
GDMYDebug.cyan()
GDMYDebug.yellow()
GDMYDebug.magenta()
```

返回对应的 `Color`。

### 性能统计

| 函数                                                      | 说明                                  |
| ------------------------------------------------------- | ----------------------------------- |
| `set_perf_stats_enabled(is_enable_now)`                 | 启用或关闭性能统计。                          |
| `set_perf_stats_font_size(font_size)`                   | 设置字体大小。                             |
| `set_perf_stats_font_color_rgba(r, g, b, a = 255)`      | 设置文字颜色。                             |
| `set_perf_stats_font_color(color)`                      | 使用 `Color` 设置文字颜色。                  |
| `set_perf_stats_print_abs_pos_xy(pos_x, pos_y)`         | 使用实际 Viewport 坐标设置显示位置。             |
| `set_perf_stats_print_abs_pos(pos)`                     | 使用实际 Viewport 的 `Vector2` 坐标设置显示位置。 |
| `reset_perf_stats_config(is_reset_enable_flag = false)` | 重置性能统计配置。                           |

默认性能统计配置：

* Enabled：`false`
* Font Size：`12`
* Font Color：`RGB(221, 34, 136)`
* Position：`(0, 0)`

`reset_perf_stats_config(true)` 会同时重置启用状态。

---
