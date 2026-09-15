#pragma once

#include "GDMYDebugMacro.h"
#include "GDMYDebugSharedDefines.h"
#include "core/object/object.h"
#include "core/os/mutex.h"
#include "core/templates/vector.h"
#include <memory>

// forward declaration
class GDMYDebugImpl;
class Font;

// this is more like a custom server?
class GDMYDebug : public Object {
	GDCLASS(GDMYDebug, Object);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	GDMYDebug();
	~GDMYDebug();

	// methods to be exposed to GDScript
	Color white() const;
	Color black() const;
	Color red() const;
	Color green() const;
	Color blue() const;
	Color cyan() const;
	Color yellow() const;
	Color magenta() const;
	void set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_print_color(const Color &color);
	void set_print_pos(const int32_t pos_x, const int32_t pos_y);
	void set_print_pos(const Vector2 &pos);
	void set_print_abs_pos(const int32_t abs_pos_x, const int32_t abs_pos_y);
	void set_print_abs_pos(const Vector2 &abs_pos);
	void print(const String &p_text, const bool is_with_background = false);
	void set_perf_stats_enabled(const bool enable_flag);
	void set_perf_stats_font_size(const int32_t font_size);
	void set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_perf_stats_font_color(const Color &color);
	void set_perf_stats_print_abs_pos(const int32_t pos_x, const int32_t pos_y);
	void set_perf_stats_print_abs_pos(const Vector2 &pos);
	void set_perf_stats_background_alpha(const float alpha);
	void set_perf_stats_background_enabled(const bool enable_flag);
	void reset_perf_stats_config(const bool is_reset_enable_flag = false);
	void draw_rect(const Rect2 &p_rect, const Color &p_color, bool p_filled = true, real_t p_width = -1.0);
	void draw_rect_abs(const Rect2 &p_rect, const Color &p_color, bool p_filled = true, real_t p_width = -1.0);

private:
	inline static GDMYDebug *singleton = nullptr;

	void flush();

	// std::unique_ptr is not neccessarily required here
	// because the main purpose of GDMYDebugImpl is not the traditional purpose of pimpl pattern (decompling, shorten compile time, ...)
	// but to disable the implemention in non-dev build (release build)
#if GDMYDEBUG_ENABLE_IMPL
	std::unique_ptr<GDMYDebugImpl> p_impl{};
#endif
};
