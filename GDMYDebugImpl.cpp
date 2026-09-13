// pair header
#include "GDMYDebugImpl.h"
// others
#include "GDMYDebugSharedUtility.h"
#include "core/config/engine.h"
#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "scene/main/window.h"
#include "scene/theme/theme_db.h"
#include "servers/rendering/rendering_server.h"

#if (GDMYDEBUG_ENABLE_IMPL)
namespace GDMYDebugImpl_CPP {
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
	// 2026-09-13: godot does not have such api now
#if 0
	std::optional<uint64_t> vram_max_bytes{ std::nullopt };
	std::optional<uint64_t> vram_peak_bytes{ std::nullopt };
#endif
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
		// "physical" is from OS::get_memory_info()
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
#if 0
		vram_max_bytes,
		vram_peak_bytes,
#endif
	};
}

// for HORIZONTAL_ALIGNMENT_LEFT
Vector2 calculate_final_print_abs_pos(
		const Vector2 &abs_pos,
		const Ref<Font> &in_font,
		const int font_size) {
	Vector2 Ret = Vector2(abs_pos.x, abs_pos.y);
	if (in_font.is_valid()) {
		Ret += Vector2(0, in_font->get_ascent(font_size));
	}
	return Ret;
}

float convert_to_color_float(const int32_t color_value) {
	return CLAMP(color_value, 0, 255) / 255.f;
}

Color get_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	return Color(convert_to_color_float(r), convert_to_color_float(g), convert_to_color_float(b), convert_to_color_float(a));
}

float calculate_adjusted_scale(const Vector2 &base_size, const Vector2 &current_viewport_size) {
	const float scale_x = current_viewport_size.x / base_size.x;
	const float scale_y = current_viewport_size.y / base_size.y;
	// to fit different aspect ratio
	return MIN(scale_x, scale_y);
}

Vector2 convert_to_abs_pos(const Vector2 &pos, const Vector2 &base_size, const Vector2 &current_viewport_size) {
	const float final_scale = calculate_adjusted_scale(base_size, current_viewport_size);
	return Vector2(
			pos.x * final_scale,
			pos.y * final_scale);
}

} //namespace GDMYDebugImpl_CPP

GDMYDebugImpl::GDMYDebugImpl() {
	if (GDMYDebugUtils::is_game_now()) {
		if (auto *rs = RenderingServer::get_singleton()) {
			canvas = rs->canvas_create();
			canvas_item = rs->canvas_item_create();
		}
	}
}

GDMYDebugImpl::~GDMYDebugImpl() {
	if (auto *rs = RenderingServer::get_singleton()) {
		if (canvas_item.is_valid()) {
			rs->free_rid(canvas_item);
		}
		if (canvas.is_valid()) {
			rs->free_rid(canvas);
		}
	}
}

void GDMYDebugImpl::flush() {
	if (GDMYDebugUtils::is_game_now()) {
		Vector<Cmd> copy_command_buffer;
		{
			MutexLock lock(command_buffer_mutex);
			SWAP(copy_command_buffer, command_buffer);
		}

		try_attach_scene_tree();

		if (RenderingServer *rs = RenderingServer::get_singleton()) {
			rs->canvas_item_clear(canvas_item);
			if (auto *theme_db = ThemeDB::get_singleton()) {
				if (const Ref<Font> font = theme_db->get_fallback_font();
						font.is_valid()) {
					const Viewport *root_viewport = get_root_viewport();
					const std::optional<Vector2> root_viewport_size = (root_viewport)
							? std::optional<Vector2>(root_viewport->get_visible_rect().size)
							: std::nullopt;

					print_performance(font);
					process_print_commands(font, copy_command_buffer, root_viewport_size);
				}
			}
		}
	}
}

void GDMYDebugImpl::set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	if (GDMYDebugUtils::is_game_now()) {
		set_print_color(GDMYDebugImpl_CPP::get_color(r, g, b, a));
	}
}

void GDMYDebugImpl::set_print_color(const Color &color) {
	if (GDMYDebugUtils::is_game_now()) {
		add_print_commands(Cmd::SetColorCmd(color));
	}
}

void GDMYDebugImpl::set_print_pos(const int32_t pos_x, const int32_t pos_y) {
	if (GDMYDebugUtils::is_game_now()) {
		set_print_pos(Vector2(pos_x, pos_y));
	}
}

void GDMYDebugImpl::set_print_pos(const Vector2 &pos) {
	if (GDMYDebugUtils::is_game_now()) {
		add_print_commands(Cmd::SetPosCmd(pos));
	}
}

void GDMYDebugImpl::set_print_abs_pos(const int32_t abs_pos_x, const int32_t abs_pos_y) {
	if (GDMYDebugUtils::is_game_now()) {
		set_print_abs_pos(Vector2(abs_pos_x, abs_pos_y));
	}
}

void GDMYDebugImpl::set_print_abs_pos(const Vector2 &abs_pos) {
	if (GDMYDebugUtils::is_game_now()) {
		add_print_commands(Cmd::SetAbsPosCmd(abs_pos));
	}
}

void GDMYDebugImpl::print(const String &p_text) {
	if (GDMYDebugUtils::is_game_now()) {
		add_print_commands(Cmd::PrintCmd(p_text));
	}
}

void GDMYDebugImpl::set_perf_stats_enabled(const bool enable_flag) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(perf_stats_config_override_mutex);
		perf_stats_config_override.is_enabled = std::optional<bool>(enable_flag);
	}
}

void GDMYDebugImpl::set_perf_stats_font_size(const int32_t font_size) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(perf_stats_config_override_mutex);
		perf_stats_config_override.font_size = std::optional<int32_t>(font_size);
	}
}

void GDMYDebugImpl::set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a) {
	if (GDMYDebugUtils::is_game_now()) {
		set_perf_stats_font_color(GDMYDebugImpl_CPP::get_color(r, g, b, a));
	}
}

void GDMYDebugImpl::set_perf_stats_font_color(const Color &color) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(perf_stats_config_override_mutex);
		perf_stats_config_override.font_color = std::optional<Color>(color);
	}
}

void GDMYDebugImpl::set_perf_stats_print_abs_pos(const int32_t pos_x, const int32_t pos_y) {
	if (GDMYDebugUtils::is_game_now()) {
		set_perf_stats_print_abs_pos(Vector2(pos_x, pos_y));
	}
}

void GDMYDebugImpl::set_perf_stats_print_abs_pos(const Vector2 &pos) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(perf_stats_config_override_mutex);
		perf_stats_config_override.print_pos = std::optional<Vector2>(Vector2(pos));
	}
}

void GDMYDebugImpl::reset_perf_stats_config(const bool is_reset_enable_flag) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(perf_stats_config_override_mutex);
		if (is_reset_enable_flag) {
			perf_stats_config_override = PerfStatsConfigOverride{};
		} else {
			// do NOT use get_current_perf_stats_config() here since already locked before if statement,
			// use get_current_perf_stats_config_unlocked()
			const bool keep_enable_flag = get_current_perf_stats_config_unlocked().is_enabled;
			perf_stats_config_override = PerfStatsConfigOverride{};
			perf_stats_config_override.is_enabled = keep_enable_flag;
		}
	}
}

Color GDMYDebugImpl::white() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::WHITE;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::black() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::BLACK;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::red() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::RED;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::green() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::GREEN;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::blue() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::BLUE;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::cyan() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::CYAN;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::yellow() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::YELLOW;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

Color GDMYDebugImpl::magenta() {
	if (GDMYDebugUtils::is_game_now()) {
		return GDMYDebugUtils::PredefinedColor::MAGENTA;
	}
	return GDMYDebugUtils::PredefinedColor::TRANSPARENT;
}

void GDMYDebugImpl::try_attach_scene_tree() {
	if (GDMYDebugUtils::is_game_now()) {
		if (!is_attached_scene_tree) {
			// MEMO: main_window seems also fine (Window *main_window = Window::get_from_id(DisplayServerEnums::MAIN_WINDOW_ID))
			if (Viewport *root_viewport = get_root_viewport()) {
				if (RenderingServer *rs = RenderingServer::get_singleton()) {
					rs->viewport_attach_canvas(root_viewport->get_viewport_rid(), canvas);
					rs->viewport_set_canvas_stacking(root_viewport->get_viewport_rid(), canvas, RenderingServerEnums::CANVAS_LAYER_MAX, 10);
					rs->canvas_item_set_parent(canvas_item, canvas);
					is_attached_scene_tree = true;
				}
			}
		}
	}
}

Viewport *GDMYDebugImpl::get_root_viewport() const {
	if (GDMYDebugUtils::is_game_now()) {
		if (SceneTree *scene_tree = SceneTree::get_singleton()) {
			return scene_tree->get_root();
		}
	}
	return nullptr;
}

void GDMYDebugImpl::print_performance(const Ref<Font> &p_font) const {
	if (GDMYDebugUtils::is_game_now()) {
		if (canvas_item.is_valid()) {
			if (p_font.is_valid()) {
				const PerfStatsConfig &current_perf_stats_config = get_current_perf_stats_config();
				if (current_perf_stats_config.is_enabled) {
					const GDMYDebugImpl_CPP::PerfStats perf_stats = GDMYDebugImpl_CPP::create_now_perf_stats();
					// expecting an outline can make the font always readable in all types of background (draw_string, draw_string_outline) ,
					// but it seems that outline makes the font looks blurry ...
					p_font->draw_string(canvas_item,
							GDMYDebugImpl_CPP::calculate_final_print_abs_pos(current_perf_stats_config.print_pos, p_font, current_perf_stats_config.font_size),
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

Size2 GDMYDebugImpl::print_string(const Ref<Font> &p_font, const String &p_text, const Vector2 &p_position, const Color &text_color, const float font_size) const {
	Size2 Ret;
	if (GDMYDebugUtils::is_game_now()) {
		if (canvas_item.is_valid()) {
			if (p_font.is_valid()) {
				Ret = p_font->get_multiline_string_size(p_text, HORIZONTAL_ALIGNMENT_LEFT, -1, font_size);
				p_font->draw_multiline_string(canvas_item,
						GDMYDebugImpl_CPP::calculate_final_print_abs_pos(p_position, p_font, font_size),
						p_text,
						HORIZONTAL_ALIGNMENT_LEFT,
						-1,
						font_size,
						-1,
						text_color);
			}
		}
	}
	return Ret;
}

void GDMYDebugImpl::add_print_commands(const Cmd &new_cmd) {
	if (GDMYDebugUtils::is_game_now()) {
		MutexLock lock(command_buffer_mutex);
		command_buffer.push_back(new_cmd);
	}
}

GDMYDebugImpl::PerfStatsConfig GDMYDebugImpl::get_current_perf_stats_config() const {
	MutexLock lock(perf_stats_config_override_mutex);
	return get_current_perf_stats_config_unlocked();
}

GDMYDebugImpl::PerfStatsConfig GDMYDebugImpl::get_current_perf_stats_config_unlocked() const {
	PerfStatsConfig result = PERF_STATS_CONFIG_DEFAULT;
	if (GDMYDebugUtils::is_game_now()) {
#define X(type, name, value) \
	result.name = perf_stats_config_override.name.value_or(result.name);
		PERF_STATS_OPTION_FIELDS
#undef X
	}
	return result;
}

void GDMYDebugImpl::process_print_commands(const Ref<Font> &p_font, const Vector<Cmd> &new_commands, const std::optional<Vector2> &root_viewport_size) {
	if (GDMYDebugUtils::is_game_now()) {
		if (p_font.is_valid()) {
			static const String linebreak_trick = String("\n");
			Vector2 current_abs_pos = Vector2(0, 0);
			Color current_color = PRINT_FONT_COLOR;
			for (const Cmd &command : new_commands) {
				switch (command.type) {
					case Cmd::Type::SET_ABS_POS: {
						current_abs_pos = command.position;
					} break;
					case Cmd::Type::SET_POS: {
						if (root_viewport_size.has_value()) {
							current_abs_pos = GDMYDebugImpl_CPP::convert_to_abs_pos(command.position, BASE_RESOLUTION, root_viewport_size.value());
						}
					} break;
					case Cmd::Type::SET_COLOR: {
						current_color = command.color;
					} break;
					case Cmd::Type::PRINT: {
						constexpr float font_size = PRINT_FONT_SIZE;
						const float adjusted_font_size = (root_viewport_size.has_value())
								? GDMYDebugImpl_CPP::calculate_adjusted_scale(BASE_RESOLUTION, root_viewport_size.value()) * font_size
								: font_size;
						// previously, use draw_string but this cannot handle in-text linebreak,
						// so moving to draw_multiline_string
						//
						// draw_multiline_string seems to add some line spacing.
						// Add a line break at the end of the passed text so that
						// current_abs_pos.y advances by the same amount for both single-line
						// and multi-line text.
						const Size2 draw_size = print_string(p_font, command.text + linebreak_trick, current_abs_pos, current_color, adjusted_font_size);
						current_abs_pos.y += draw_size.y;
					} break;
					default: {
					} break;
				}
			}
		}
	}
}

#endif
