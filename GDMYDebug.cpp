// pair header
#include "GDMYDebug.h"
// others
#include "core/config/engine.h"
#include "core/core_bind.h"
#include "core/object/callable_mp.h"
#include "core/object/class_db.h"
#include "core/os/main_loop.h"
#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "scene/main/canvas_item.h"
#include "scene/main/window.h"
#include "scene/theme/theme_db.h"
#include "scene/gui/control.h"
#include "servers/display/display_server.h"
#include "servers/rendering/rendering_server.h"

#if GDMYDEBUG_ENABLE_IMPL
#define GDMYDEBUG_IS_GAME_NOW() (GDMYDebug_CPP::is_game_now())
#else
#define GDMYDEBUG_IS_GAME_NOW() (false)
#endif

namespace GDMYDebug_CPP {
struct PerfStats {
	PerfStats() = default;

	String get_one_line_text() const {
		const String fps_value_text = fps.has_value()
				? vformat("%.1f", static_cast<float>(fps.value()))
				: ("-");
		const String ram_now_value_text = ram_now_bytes.has_value()
				? vformat("%.3f", ram_now_bytes.value() / (1024.0 * 1024.0 * 1024.0))
				: ("-");
		const String ram_max_value_text = ram_max_bytes.has_value()
				? vformat("%.3f", ram_max_bytes.value() / (1024.0 * 1024.0 * 1024.0))
				: ("-");
		const String ram_peak_value_text = ram_peak_bytes.has_value()
				? vformat("%.3f", ram_peak_bytes.value() / (1024.0 * 1024.0 * 1024.0))
				: ("-");
		const String vram_now_value_text = vram_now_bytes.has_value()
				? vformat("%.3f", vram_now_bytes.value() / (1024.0 * 1024.0))
				: ("-");
		return vformat("FPS: %s | RAM: %s/ %s GB (PEAK: %s GB) | VRAM: %s MB",
				fps_value_text,
				ram_now_value_text,
				ram_max_value_text,
				ram_peak_value_text,
				vram_now_value_text);
	}

	std::optional<double> fps{ std::nullopt };
	std::optional<uint64_t> ram_now_bytes{ std::nullopt };
	std::optional<uint64_t> ram_max_bytes{ std::nullopt };
	std::optional<uint64_t> ram_peak_bytes{ std::nullopt };
	std::optional<uint64_t> vram_now_bytes{ std::nullopt };
	std::optional<uint64_t> vram_max_bytes{ std::nullopt };
	std::optional<uint64_t> vram_peak_bytes{ std::nullopt };
};

PerfStats create_now_perf_stats() {
	std::optional<double> fps = std::nullopt;
	if (auto *engine = Engine::get_singleton()) {
		fps = std::optional<double>(engine->get_frames_per_second());
	}

	std::optional<uint64_t> ram_now_bytes = std::nullopt;
	std::optional<uint64_t> ram_max_bytes = std::nullopt;
	std::optional<uint64_t> ram_peak_bytes = std::nullopt;
	if (auto *os = OS::get_singleton()) {
		const Dictionary &ram_info = os->get_memory_info();
		if (ram_info.has("physical")) {
			if (const int64_t ram_max = ram_info["physical"];
					ram_max >= 0) {
				ram_max_bytes = std::optional<uint64_t>(ram_max);
			}
		}
		ram_now_bytes = std::optional<uint64_t>(os->get_static_memory_usage());
		ram_peak_bytes = std::optional<uint64_t>(os->get_static_memory_peak_usage());
	}

	std::optional<uint64_t> vram_now_bytes = std::nullopt;
	std::optional<uint64_t> vram_max_bytes = std::nullopt;
	std::optional<uint64_t> vram_peak_bytes = std::nullopt;
	if (auto *rs = RS::get_singleton()) {
		vram_now_bytes = std::optional<uint64_t>(rs->get_rendering_info(RSE::RENDERING_INFO_VIDEO_MEM_USED));
	}

	return PerfStats{
		fps,
		ram_now_bytes,
		ram_max_bytes,
		ram_peak_bytes,
		vram_now_bytes,
		vram_max_bytes,
		vram_peak_bytes,
	};
}

bool is_game_now() {
	if (auto *engine = Engine::get_singleton()) {
		return !engine->is_editor_hint();
	}
	return false;
}

// for HORIZONTAL_ALIGNMENT_LEFT
Vector2 calculate_final_print_screen_pos(
		const Vector2 &in_pos,
		const Ref<Font> &in_font,
		const int font_size) {
	Vector2 Ret = Vector2(in_pos.x, in_pos.y);
	if (in_font.is_valid()) {
		Ret += Vector2(0, in_font->get_ascent(font_size));
	}
	return Ret;
}

constexpr float convert_to_color_float(const int32_t color_value) {
	return CLAMP(color_value, 0, 255) / 255.f;
}

constexpr Color get_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	return Color(convert_to_color_float(r), convert_to_color_float(g), convert_to_color_float(b), convert_to_color_float(a));
}

} //namespace GDMYDebug_CPP

GDMYDebug::GDMYDebug() {
	singleton = this;
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (auto *rs = RenderingServer::get_singleton()) {
			canvas = rs->canvas_create();
			canvas_item = rs->canvas_item_create();
		}
	}
}

GDMYDebug::~GDMYDebug() {
	if (singleton == this) {
		if (GDMYDEBUG_IS_GAME_NOW()) {
			ERR_FAIL_NULL(RenderingServer::get_singleton());
			if (canvas_item.is_valid()) {
				RenderingServer::get_singleton()->free_rid(canvas_item);
			}
			if (canvas.is_valid()) {
				RenderingServer::get_singleton()->free_rid(canvas);
			}
		}
		singleton = nullptr;
	}
}

void GDMYDebug::flush() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (!GDMYDebug_CPP::is_game_now()) {
			return;
		}

		Vector<Cmd> copy_command_buffer;
		{
			MutexLock lock(command_buffer_mutex);
			copy_command_buffer.reserve(command_buffer.size());
			copy_command_buffer = command_buffer;
			command_buffer.clear();
		}

		try_attach_scene_tree();

		if (RenderingServer *rs = RenderingServer::get_singleton()) {
			rs->canvas_item_clear(canvas_item);
			if (auto *theme_db = ThemeDB::get_singleton()) {
				if (const Ref<Font> font = theme_db->get_fallback_font();
						font.is_valid()) {
					print_performance(font);
					process_print_commands(font, copy_command_buffer);
				}
			}
		}
	}
}

void GDMYDebug::set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		set_print_color(GDMYDebug_CPP::get_color(r, g, b, a));
	}
}

void GDMYDebug::set_print_color(const Color &color) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		add_print_commands(Cmd::SetColorCmd(color));
	}
}

void GDMYDebug::set_print_position(const int32_t pos_x, const int32_t pos_y) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		set_print_position(Vector2(pos_x, pos_y));
	}
}

void GDMYDebug::set_print_position(const Vector2 &pos) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		add_print_commands(Cmd::SetPositionCmd(pos));
	}
}

void GDMYDebug::print(const String &p_text) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		add_print_commands(Cmd::PrintCmd(p_text));
	}
}

void GDMYDebug::set_perf_stats_enabled(const bool is_enable_now) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		perf_stats_config_override.is_enabled = std::optional<bool>(is_enable_now);
	}
}

void GDMYDebug::set_perf_stats_font_size(const int32_t font_size) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		perf_stats_config_override.font_size = std::optional<int32_t>(font_size);
	}
}

void GDMYDebug::set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		set_perf_stats_font_color(GDMYDebug_CPP::get_color(r, g, b, a));
	}
}

void GDMYDebug::set_perf_stats_font_color(const Color &color) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		perf_stats_config_override.font_color = std::optional<Color>(color);
	}
}

void GDMYDebug::set_perf_stats_print_pos(const int32_t pos_x, const int32_t pos_y) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		set_perf_stats_print_pos(Vector2(pos_x, pos_y));
	}
}

void GDMYDebug::set_perf_stats_print_pos(const Vector2 &pos) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		perf_stats_config_override.print_pos = std::optional<Vector2>(Vector2(pos));
	}
}

void GDMYDebug::reset_perf_stats_config(const bool is_reset_enable_flag) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (is_reset_enable_flag) {
			perf_stats_config_override = PerfStatsConfigOverride{};
		} else {
			const bool keep_enable_flag = get_current_perf_stats_config().is_enabled;
			perf_stats_config_override = PerfStatsConfigOverride{};
			perf_stats_config_override.is_enabled = keep_enable_flag;
		}
		
	}
}

const int32_t GDMYDebug::print_font_size() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PRINT_FONT_SIZE;
	}
	return 0;
}

const Color GDMYDebug::white() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::WHITE;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::black() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::BLACK;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::red() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::RED;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::green() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::GREEN;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::blue() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::BLUE;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::cyan() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::CYAN;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::yellow() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::YELLOW;
	}
	return PredefinedColor::TRANSPARENT;
}

const Color GDMYDebug::magenta() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		return PredefinedColor::MAGENTA;
	}
	return PredefinedColor::TRANSPARENT;
}

void GDMYDebug::try_attach_scene_tree() {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (!is_attached_scene_tree) {
			// MEMO: main_window seems also fine (Window *main_window = Window::get_from_id(DisplayServerEnums::MAIN_WINDOW_ID))
			if (SceneTree *scene_tree = SceneTree::get_singleton()) {
				if (Viewport *viewport = scene_tree->get_root()) {
					if (RenderingServer *rs = RenderingServer::get_singleton()) {
						rs->viewport_attach_canvas(viewport->get_viewport_rid(), canvas);
						rs->viewport_set_canvas_stacking(viewport->get_viewport_rid(), canvas, RenderingServerEnums::CANVAS_LAYER_MAX, 10);
						rs->canvas_item_set_parent(canvas_item, canvas);

						is_attached_scene_tree = true;
					}
				}
			}
		}
	}
}

void GDMYDebug::print_performance(const Ref<Font> &p_font) const {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (canvas_item.is_valid()) {
			if (p_font.is_valid()) {
				const PerfStatsConfig &current_perf_stats_config = get_current_perf_stats_config();
				if (current_perf_stats_config.is_enabled) {
					const GDMYDebug_CPP::PerfStats &perf_stats = GDMYDebug_CPP::create_now_perf_stats();
					// expecting outline can make the font always readable in all types of background,
					// but it seems that outline makes the font looks blurry ...
#if 0
			p_font->draw_string(canvas_item,
					GDMYDebug_CPP::calculate_final_print_screen_pos(Vector2(0, 0), p_font, PERF_STATS_FONT_SIZE),
					perf_stats.get_one_line_text(),
					HORIZONTAL_ALIGNMENT_LEFT,
					-1,
					PERF_STATS_FONT_SIZE,
					PERF_STATS_FONT_COLOR);
			p_font->draw_string_outline(canvas_item,
					GDMYDebug_CPP::calculate_final_print_screen_pos(Vector2(0, 0), p_font, PERF_STATS_FONT_SIZE),
					perf_stats.get_one_line_text(),
					HORIZONTAL_ALIGNMENT_LEFT,
					-1,
					PERF_STATS_FONT_SIZE,
					PERF_STATS_OUTLINE_SIZE,
					PERF_STATS_OUTLINE_COLOR);
#endif
					p_font->draw_string(canvas_item,
							GDMYDebug_CPP::calculate_final_print_screen_pos(current_perf_stats_config.print_pos, p_font, current_perf_stats_config.font_size),
							perf_stats.get_one_line_text(),
							HORIZONTAL_ALIGNMENT_LEFT,
							-1,
							current_perf_stats_config.font_size,
							current_perf_stats_config.font_color);
				}
			}
		}
	}
}

void GDMYDebug::print_string(const Ref<Font> &p_font, const String &p_text, const Vector2 &p_position, const Color &text_color, const float font_size) const {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (canvas_item.is_valid()) {
			if (p_font.is_valid()) {
				p_font->draw_string(canvas_item,
						GDMYDebug_CPP::calculate_final_print_screen_pos(p_position, p_font, font_size),
						p_text,
						HORIZONTAL_ALIGNMENT_LEFT,
						-1,
						font_size,
						text_color);
			}
		}
	}
}

void GDMYDebug::add_print_commands(const Cmd &new_cmd) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		MutexLock lock(command_buffer_mutex);
		command_buffer.push_back(new_cmd);
	}
}

GDMYDebug::PerfStatsConfig GDMYDebug::get_current_perf_stats_config() const {
	PerfStatsConfig result = PERF_STATS_CONFIG_DEFAULT;
	if (GDMYDEBUG_IS_GAME_NOW()) {
#define X(type, name, value) \
	result.name = perf_stats_config_override.name.value_or(result.name);
		PERF_STATS_OPTION_FIELDS
#undef X
	}
	return result;
}

void GDMYDebug::process_print_commands(const Ref<Font> &p_font, const Vector<Cmd> &new_commands) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		if (p_font.is_valid()) {
			Vector2 current_position = Vector2(0, 0);
			Color current_color = PRINT_FONT_COLOR;
			for (const Cmd &command : new_commands) {
				switch (command.type) {
					case Cmd::Type::SET_POSITION : {
						current_position = command.position;
					} break;
					case Cmd::Type::SET_COLOR: {
						current_color = command.color;
					} break;
					case Cmd::Type::PRINT: {
						constexpr float font_size = PRINT_FONT_SIZE;
						print_string(p_font, command.text, current_position, current_color, font_size);
						current_position.y += p_font->get_height(font_size);
					} break;
				}
			}
		}
	}
}

void GDMYDebug::_bind_methods() {
	// common
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("white"), &GDMYDebug::white);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("black"), &GDMYDebug::black);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("red"), &GDMYDebug::red);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("green"), &GDMYDebug::green);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("blue"), &GDMYDebug::blue);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("cyan"), &GDMYDebug::cyan);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("yellow"), &GDMYDebug::yellow);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("magenta"), &GDMYDebug::magenta);
	// print related
	ClassDB::bind_method(D_METHOD("print", "message"), &GDMYDebug::print);
	ClassDB::bind_method(D_METHOD("set_print_pos_xy", "pos_x", "pos_y"), (void(GDMYDebug::*)(const int32_t, const int32_t)) & GDMYDebug::set_print_position);
	ClassDB::bind_method(D_METHOD("set_print_pos", "pos"), (void(GDMYDebug::*)(const Vector2 &)) & GDMYDebug::set_print_position);
	ClassDB::bind_method(D_METHOD("set_print_color_rgba", "r", "g", "b", "a"), (void(GDMYDebug::*)(const int32_t, const int32_t, const int32_t, const int32_t)) & GDMYDebug::set_print_color, DEFVAL(255));
	ClassDB::bind_method(D_METHOD("set_print_color", "color"), (void(GDMYDebug::*)(const Color &)) & GDMYDebug::set_print_color);
	ClassDB::bind_static_method("GDMYDebug", D_METHOD("print_font_size"), &GDMYDebug::print_font_size);
	// perf stats related
	ClassDB::bind_method(D_METHOD("set_perf_stats_enabled", "is_enable_now"), &GDMYDebug::set_perf_stats_enabled);
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_size", "font_size"), &GDMYDebug::set_perf_stats_font_size);
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_color_rgba", "r", "g", "b", "a"), (void(GDMYDebug::*)(const int32_t, const int32_t, const int32_t, const int32_t)) & GDMYDebug::set_perf_stats_font_color, DEFVAL(255));
	ClassDB::bind_method(D_METHOD("set_perf_stats_font_color", "color"), (void(GDMYDebug::*)(const Color &)) & GDMYDebug::set_perf_stats_font_color);
	ClassDB::bind_method(D_METHOD("set_perf_stats_print_pos_xy", "pos_x", "pos_y"), (void(GDMYDebug::*)(const int32_t, const int32_t)) & GDMYDebug::set_perf_stats_print_pos);
	ClassDB::bind_method(D_METHOD("set_perf_stats_print_pos", "pos"), (void(GDMYDebug::*)(const Vector2 &)) & GDMYDebug::set_perf_stats_print_pos);
	ClassDB::bind_method(D_METHOD("reset_perf_stats_config", "is_reset_enable_flag"), &GDMYDebug::reset_perf_stats_config, DEFVAL(false));
}

void GDMYDebug::_notification(int p_what) {
	if (GDMYDEBUG_IS_GAME_NOW()) {
		switch (p_what) {
			case NOTIFICATION_POSTINITIALIZE: {
				// MEMO: the most propor palce to attach, but scene tree is still not created ...
				// so we also try_attach_scene_tree in flush (process_frame)
				try_attach_scene_tree();

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
}
