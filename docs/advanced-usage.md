## Advanced Usage

### Display Debug Information Above a 3D Object

`GDMYDebug` draws debug text in screen space, so a 3D object's world position needs to be converted to a 2D viewport position first.

For example, in GDScript:

```gdscript
var camera := get_viewport().get_camera_3d()

if camera.is_position_behind(global_position):
    return

var screen_pos := camera.unproject_position(global_position + Vector3.UP * 2.0)

GDMYDebug.set_print_abs_pos(screen_pos)
GDMYDebug.print("Debug Info")
```

`unproject_position()` converts a 3D world position into a 2D viewport position.

You should check `is_position_behind()` before calling `unproject_position()`. If the object is behind the camera, the resulting screen position may be invalid or behave unexpectedly. This can cause the debug information to move off-screen.

When displaying debug information for multiple 3D objects, it is also a good idea to limit the display distance. Otherwise, too many objects may generate too much debug text and make the screen difficult to read.

For example:

```gdscript
var distance = get_distance_to_player/camera/...()

if distance > MAX_DEBUG_DISTANCE:
    continue
```

This is just an example. The appropriate distance depends on the project and the type of debug information being displayed.

`abs_pos` is used here because `unproject_position()` returns a position in viewport coordinates.

---

## 高级用法

### 在 3D 物体上方显示调试信息

`GDMYDebug` 在屏幕空间中绘制调试文字，因此需要先将 3D 物体的世界坐标转换为 2D Viewport 坐标。

例如，在 GDScript 中：

```gdscript
var camera := get_viewport().get_camera_3d()

if camera.is_position_behind(global_position):
    return

var screen_pos := camera.unproject_position(global_position + Vector3.UP * 2.0)

GDMYDebug.set_print_abs_pos(screen_pos)
GDMYDebug.print("Debug Info")
```

`unproject_position()` 会将 3D 世界坐标转换为 2D Viewport 坐标。

在调用 `unproject_position()` 之前，应该先检查 `is_position_behind()`。如果物体位于摄像机后方，得到的屏幕坐标可能无效或产生异常，从而导致调试信息飞到屏幕外。

当需要为多个 3D 物体显示调试信息时，也建议限制显示距离。否则，过多的物体可能会产生大量调试文字，使屏幕变得难以阅读。

例如：

```gdscript
var distance = get_distance_to_player/camera/...()

if distance > MAX_DEBUG_DISTANCE:
    continue
```

这里只是一个示例。合适的距离取决于项目以及所显示的调试信息类型。

这里使用 `abs_pos` 是因为 `unproject_position()` 返回的是 Viewport 坐标。
