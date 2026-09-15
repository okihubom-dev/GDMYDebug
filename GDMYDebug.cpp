// pair header
#include "GDMYDebug.h"
// others
#include "GDMYDebugImpl.h"
#include "GDMYDebugSharedUtility.h"
#include "core/config/engine.h"
#include "core/core_bind.h"
#include "core/object/callable_mp.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "scene/main/window.h"
#include "scene/theme/theme_db.h"
#include "servers/rendering/rendering_server.h"

namespace GDMYDebug_CPP {
} //namespace GDMYDebug_CPP

GDMYDebug::GDMYDebug() {
	singleton = this;
#if GDMYDEBUG_ENABLE_IMPL
	p_impl = std::make_unique<GDMYDebugImpl>();
#else
#endif
}

GDMYDebug::~GDMYDebug() {
	if (singleton == this) {
#if GDMYDEBUG_ENABLE_IMPL
#else
#endif
		singleton = nullptr;
	}
}

void GDMYDebug::flush() {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl* impl = p_impl.get()) {
		impl->flush();
	}
#else
#endif
}

void GDMYDebug::set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_color(r, g, b, a);
	}
#else
#endif
}

void GDMYDebug::set_print_color(const Color &color) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_color(color);
	}
#else
#endif
}

void GDMYDebug::set_print_pos(const int32_t pos_x, const int32_t pos_y) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_pos(pos_x, pos_y, false);
	}
#else
#endif
}

void GDMYDebug::set_print_pos(const Vector2 &pos) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_pos(pos, false);
	}
#else
#endif
}

void GDMYDebug::set_print_abs_pos(const int32_t abs_pos_x, const int32_t abs_pos_y) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_pos(abs_pos_x, abs_pos_y, true);
	}
#else
#endif
}

void GDMYDebug::set_print_abs_pos(const Vector2 &abs_pos) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_print_pos(abs_pos, true);
	}
#else
#endif
}

void GDMYDebug::print(const String &p_text, const bool is_with_background) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->print(p_text, is_with_background);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_enabled(const bool enable_flag) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_enabled(enable_flag);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_font_size(const int32_t font_size) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_font_size(font_size);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_font_color(r, g, b, a);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_font_color(const Color &color) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_font_color(color);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_print_abs_pos(const int32_t pos_x, const int32_t pos_y) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_print_abs_pos(pos_x, pos_y);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_print_abs_pos(const Vector2 &pos) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_print_abs_pos(pos);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_background_alpha(const float alpha)
{
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_background_alpha(alpha);
	}
#else
#endif
}

void GDMYDebug::set_perf_stats_background_enabled(const bool enable_flag)
{
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->set_perf_stats_background_enabled(enable_flag);
	}
#else
#endif
}

void GDMYDebug::reset_perf_stats_config(const bool is_reset_enable_flag) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->reset_perf_stats_config(is_reset_enable_flag);
	}
#else
#endif
}

void GDMYDebug::draw_rect(const Rect2 &p_rect, const Color &p_color, bool p_filled, real_t p_width) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->draw_rect(p_rect, p_color, p_filled, p_width, false);
	}
#else
#endif
}

void GDMYDebug::draw_rect_abs(const Rect2 &p_rect, const Color &p_color, bool p_filled, real_t p_width) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		impl->draw_rect(p_rect, p_color, p_filled, p_width, true);
	}
#else
#endif
}

Color GDMYDebug::white() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->white();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::black() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->black();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::red() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->red();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::green() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->green();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::blue() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->blue();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::cyan() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->cyan();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::yellow() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->yellow();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebug::magenta() const {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		return impl->magenta();
	}
#else
#endif
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

void GDMYDebug::_bind_methods() {
	// common
	ClassDB::bind_method(D_METHOD("white"), &GDMYDebug::white);
	ClassDB::bind_method(D_METHOD("black"), &GDMYDebug::black);
	ClassDB::bind_method(D_METHOD("red"), &GDMYDebug::red);
	ClassDB::bind_method(D_METHOD("green"), &GDMYDebug::green);
	ClassDB::bind_method(D_METHOD("blue"), &GDMYDebug::blue);
	ClassDB::bind_method(D_METHOD("cyan"), &GDMYDebug::cyan);
	ClassDB::bind_method(D_METHOD("yellow"), &GDMYDebug::yellow);
	ClassDB::bind_method(D_METHOD("magenta"), &GDMYDebug::magenta);
	// print related
	ClassDB::bind_method(D_METHOD("print", "message", "is_with_background"), &GDMYDebug::print, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("set_print_pos_xy", "pos_x", "pos_y"), (void(GDMYDebug::*)(const int32_t, const int32_t)) & GDMYDebug::set_print_pos);
	ClassDB::bind_method(D_METHOD("set_print_pos", "pos"), (void(GDMYDebug::*)(const Vector2 &)) & GDMYDebug::set_print_pos);
	ClassDB::bind_method(D_METHOD("set_print_abs_pos_xy", "abs_pos_x", "abs_pos_y"), (void(GDMYDebug::*)(const int32_t, const int32_t)) & GDMYDebug::set_print_abs_pos);
	ClassDB::bind_method(D_METHOD("set_print_abs_pos", "abs_pos"), (void(GDMYDebug::*)(const Vector2 &)) & GDMYDebug::set_print_abs_pos);
	ClassDB::bind_method(D_METHOD("set_print_color_rgba", "r", "g", "b", "a"), (void(GDMYDebug::*)(const int32_t, const int32_t, const int32_t, const int32_t)) & GDMYDebug::set_print_color, DEFVAL(255));
	ClassDB::bind_method(D_METHOD("set_print_color", "color"), (void(GDMYDebug::*)(const Color &)) & GDMYDebug::set_print_color);
	// perf stats related
	ClassDB::bind_method(D_METHOD("set_perf_stats_enabled", "enable_flag"), &GDMYDebug::set_perf_stats_enabled);
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_size", "font_size"), &GDMYDebug::set_perf_stats_font_size);
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_color_rgba", "r", "g", "b", "a"), (void(GDMYDebug::*)(const int32_t, const int32_t, const int32_t, const int32_t)) & GDMYDebug::set_perf_stats_font_color, DEFVAL(255));
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_color", "color"), (void(GDMYDebug::*)(const Color &)) & GDMYDebug::set_perf_stats_font_color);
	ClassDB::bind_method(D_METHOD("set_perf_stats_print_abs_pos_xy", "pos_x", "pos_y"), (void(GDMYDebug::*)(const int32_t, const int32_t)) & GDMYDebug::set_perf_stats_print_abs_pos);
	ClassDB::bind_method(D_METHOD("set_perf_stats_print_abs_pos", "pos"), (void(GDMYDebug::*)(const Vector2 &)) & GDMYDebug::set_perf_stats_print_abs_pos);
	ClassDB::bind_method(D_METHOD("set_perf_stats_background_alpha", "alpha"), &GDMYDebug::set_perf_stats_background_alpha);
	ClassDB::bind_method(D_METHOD("set_perf_stats_background_enabled", "enable_flag"), &GDMYDebug::set_perf_stats_background_enabled);
	ClassDB::bind_method(D_METHOD("reset_perf_stats_config", "is_reset_enable_flag"), &GDMYDebug::reset_perf_stats_config, DEFVAL(false));
	// draw related
	ClassDB::bind_method(D_METHOD("draw_rect", "rect", "color", "is_filled", "width"), &GDMYDebug::draw_rect, DEFVAL(-1.0), DEFVAL(true));
	ClassDB::bind_method(D_METHOD("draw_rect_abs", "rect", "color", "is_filled", "width"), &GDMYDebug::draw_rect_abs, DEFVAL(-1.0), DEFVAL(true));
}

void GDMYDebug::_notification(int p_what) {
#if GDMYDEBUG_ENABLE_IMPL
	if (GDMYDebugImpl *impl = p_impl.get()) {
		switch (p_what) {
			case NOTIFICATION_POSTINITIALIZE: {
				// MEMO: the most proper place to attach I can find, but scene tree is still not created ...
				// so we also try_attach_scene_tree in flush (process_frame)
				impl->try_attach_scene_tree();
				if (auto *rs = RenderingServer::get_singleton()) {
					if (!rs->is_connected("frame_pre_draw", callable_mp(this, &GDMYDebug::flush))) {
						rs->connect("frame_pre_draw", callable_mp(this, &GDMYDebug::flush));
					}
				}
			} break;
			case NOTIFICATION_PREDELETE: {
				if (auto *rs = RenderingServer::get_singleton()) {
					if (rs->is_connected("frame_pre_draw", callable_mp(this, &GDMYDebug::flush))) {
						rs->disconnect("frame_pre_draw", callable_mp(this, &GDMYDebug::flush));
					}
				}
			} break;
		}
	}
#else
#endif
}

