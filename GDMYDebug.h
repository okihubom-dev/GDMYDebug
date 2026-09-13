#pragma once

#include "GDMYDebugMacro.h"
#include "core/object/object.h"
#include "core/os/mutex.h"
#include "core/templates/vector.h"
#include "GDMYDebugSharedDefines.h"
#include <optional>

// forward declaration
class Viewport;
class Font;

#define PERF_STATS_OPTION_FIELDS                                    \
  X(bool, is_enabled, false)                                        \
  X(int32_t, font_size, 12)                                         \
  X(Color, font_color, Color(221 / 255.f, 34 / 255.f, 136 / 255.f)) \
  X(Vector2, print_pos, Vector2(0, 0))


// this is more like a custom server?
class GDMYDebug : public Object {
	GDCLASS(GDMYDebug, Object);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	struct Cmd {
		enum class Type : int32_t {
			NONE = 0,
			PRINT,
			SET_POS,
			SET_COLOR,
			SET_ABS_POS,
		};

		Cmd() = default;

		static Cmd PrintCmd(const String &in_text) {
			Cmd ret;
			ret.type = Type::PRINT;
			ret.text = in_text;
			return ret;
		}

		static Cmd SetPosCmd(const Vector2 &in_pos) {
			Cmd ret;
			ret.type = Type::SET_POS;
			ret.position = in_pos;
			return ret;
		}

		static Cmd SetColorCmd(const Color &in_color) {
			Cmd ret;
			ret.type = Type::SET_COLOR;
			ret.color = in_color;
			return ret;
		}

		static Cmd SetAbsPosCmd(const Vector2 &in_pos) {
			Cmd ret;
			ret.type = Type::SET_ABS_POS;
			ret.position = in_pos;
			return ret;
		}

		Type type{ Type::NONE };
		Vector2 position{ Vector2() };
		Color color{};
		String text{};
	};

	GDMYDebug();
	~GDMYDebug();

	static const Color white();
	static const Color black();
	static const Color red();
	static const Color green();
	static const Color blue();
	static const Color cyan();
	static const Color yellow();
	static const Color magenta();
	void set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_print_color(const Color& color);
	// pos is relative pos to BASE_RESOLUTION to keep printed text are visually in the same position
	// abs_pos is absolute pos on the screen
	void set_print_pos(const int32_t pos_x, const int32_t pos_y);
	void set_print_pos(const Vector2 &pos);
	void set_print_abs_pos(const int32_t abs_pos_x, const int32_t abs_pos_y);
	void set_print_abs_pos(const Vector2 &abs_pos);
	void print(const String &p_text);
	void set_perf_stats_enabled(const bool enable_flag);
	void set_perf_stats_font_size(const int32_t font_size);
	void set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_perf_stats_font_color(const Color &color);
	void set_perf_stats_print_abs_pos(const int32_t pos_x, const int32_t pos_y);
	void set_perf_stats_print_abs_pos(const Vector2 &pos);
	void reset_perf_stats_config(const bool is_reset_enable_flag = false);

private:
	struct PerfStatsConfig {
#define X(type, name, default_value) type name { default_value };
		PERF_STATS_OPTION_FIELDS
#undef X
	};

	struct PerfStatsConfigOverride {
#define X(type, name, default_value) std::optional<type> name{ std::nullopt };
		PERF_STATS_OPTION_FIELDS
#undef X
	};

	inline static GDMYDebug *singleton = nullptr;
	inline static constexpr Vector2 BASE_RESOLUTION = Vector2(1920, 1080);
	inline static constexpr int32_t PRINT_FONT_SIZE = 16;
	inline static constexpr Color PRINT_FONT_COLOR = GDMYDebugUtils::PredefinedColor::WHITE;
	inline static constexpr PerfStatsConfig PERF_STATS_CONFIG_DEFAULT = PerfStatsConfig{};

	void flush();
	void try_attach_scene_tree();
	Viewport *get_root_viewport() const;

	void print_performance(const Ref<Font> &p_font) const;
	void process_print_commands(const Ref<Font> &p_font, const Vector<Cmd> &new_commands, const std::optional<Vector2> &root_viewport_size);
	Size2 print_string(const Ref<Font> &p_font, const String &p_text, const Vector2 &p_position, const Color &text_color, const float font_size) const;
	void add_print_commands(const Cmd &new_cmd);

	PerfStatsConfig get_current_perf_stats_config() const;

	RID canvas;
	RID canvas_item;
	bool is_attached_scene_tree{ false };
	Mutex command_buffer_mutex{};
	Vector<Cmd> command_buffer{};
	
	PerfStatsConfigOverride perf_stats_config_override{};
};
