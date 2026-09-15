#pragma once

#include "GDMYDebugMacro.h"
#include "GDMYDebugSharedDefines.h"

#include "core/object/object.h"
#include "core/os/mutex.h"
#include "core/templates/vector.h"

#include <optional>
#include <variant>

#if GDMYDEBUG_ENABLE_IMPL
// forward declaration
class Viewport;
class Font;
class RenderingServer;

#define PERF_STATS_OPTION_FIELDS                                              \
	X(bool, is_enabled, false)                                                \
	X(int32_t, font_size, 12)                                                 \
	X(Color, font_color, Color(1.f, 1.f, 1.f, 1.f))                           \
	X(Color, background_color, GDMYDebugUtils::PredefinedColor::DEFAULT_BG)   \
	X(Vector2, print_pos, Vector2(0, 0))                                      \
    X(bool, is_no_background, false)


class GDMYDebugImpl {
public:
	GDMYDebugImpl();
	~GDMYDebugImpl();

	// functions need to be called by outer module
	void flush();
	void try_attach_scene_tree();

	// methods to be exposed to GDScript
	static Color white();
	static Color black();
	static Color red();
	static Color green();
	static Color blue();
	static Color cyan();
	static Color yellow();
	static Color magenta();
	// print related
	void set_print_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_print_color(const Color &color);
	// pos is relative pos to BASE_RESOLUTION to keep printed text are visually in the same position
	// abs_pos is absolute pos on the screen
	void set_print_pos(const int32_t pos_x, const int32_t pos_y, const bool is_abs);
	void set_print_pos(const Vector2 &pos, const bool is_abs);
	void print(const String &text, const bool is_with_background);
	// perf stats related
	void set_perf_stats_enabled(const bool enable_flag);
	void set_perf_stats_font_size(const int32_t font_size);
	void set_perf_stats_font_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_perf_stats_font_color(const Color &color);
	void set_perf_stats_print_abs_pos(const int32_t pos_x, const int32_t pos_y);
	void set_perf_stats_print_abs_pos(const Vector2 &pos);
	void set_perf_stats_background_alpha(const float alpha);
	void set_perf_stats_background_enabled(const bool enable_flag);
	void reset_perf_stats_config(const bool is_reset_enable_flag = false);
	// shape related
	void draw_rect(const Rect2 &p_rect, const Color &p_color, bool p_filled, real_t p_width, const bool is_abs);
	// TODO
	void draw_dashed_line(const Point2 &p_from, const Point2 &p_to, const Color &p_color, real_t p_width = -1.0, real_t p_dash = 2.0, bool p_aligned = true);
	void draw_line(const Point2 &p_from, const Point2 &p_to, const Color &p_color, real_t p_width = -1.0);
	void draw_polyline(const Vector<Point2> &p_points, const Color &p_color, real_t p_width = -1.0);
	void draw_polyline_colors(const Vector<Point2> &p_points, const Vector<Color> &p_colors, real_t p_width = -1.0);
	void draw_ellipse_arc(const Vector2 &p_center, real_t p_major, real_t p_minor, real_t p_start_angle, real_t p_end_angle, int p_point_count, const Color &p_color, real_t p_width = -1.0);
	void draw_arc(const Vector2 &p_center, real_t p_radius, real_t p_start_angle, real_t p_end_angle, int p_point_count, const Color &p_color, real_t p_width = -1.0);
	void draw_multiline(const Vector<Point2> &p_points, const Color &p_color, real_t p_width = -1.0);
	void draw_multiline_colors(const Vector<Point2> &p_points, const Vector<Color> &p_colors, real_t p_width = -1.0);
	void draw_ellipse(const Point2 &p_pos, real_t p_major, real_t p_minor, const Color &p_color, bool p_filled = true, real_t p_width = -1.0);
	void draw_circle(const Point2 &p_pos, real_t p_radius, const Color &p_color, bool p_filled = true, real_t p_width = -1.0);
	void draw_primitive(const Vector<Point2> &p_points, const Vector<Color> &p_colors, const Vector<Point2> &p_uvs);
	void draw_polygon(const Vector<Point2> &p_points, const Vector<Color> &p_colors, const Vector<Point2> &p_uvs = Vector<Point2>());
	void draw_colored_polygon(const Vector<Point2> &p_points, const Color &p_color, const Vector<Point2> &p_uvs = Vector<Point2>());

private:
	// let text always be the topmost
	enum ZOrderOfCanvasItem : int32_t {
		Shape = 0,
		Text,
	};

	struct CmdPrint {
		String text{};
		bool is_with_background{false};
	};

	struct CmdSetPrintPos {
		Vector2 pos{ Vector2() };
		bool is_abs{ false };
	};

	struct CmdSetPrintColor {
		Color text_color{};
	};

	struct CmdDrawRect {
		Rect2 rect{ Rect2() };
		Color rect_color{};
		real_t width{ -1.0 };
		bool is_filled{ true };
		bool is_abs{ false };
	};

	using VarCmd = std::variant<
			CmdPrint,
			CmdSetPrintPos,
			CmdSetPrintColor,
			CmdDrawRect>;

	struct PerfStatsConfig {
#define X(type, name, default_value) type name{ default_value };
		PERF_STATS_OPTION_FIELDS
#undef X
	};

	struct PerfStatsConfigOverride {
#define X(type, name, default_value) std::optional<type> name{ std::nullopt };
		PERF_STATS_OPTION_FIELDS
#undef X
	};

	inline static constexpr Vector2 BASE_RESOLUTION = Vector2(1920, 1080);
	inline static constexpr int32_t PRINT_FONT_SIZE = 16;
	inline static constexpr Color PRINT_FONT_COLOR = GDMYDebugUtils::PredefinedColor::WHITE;
	inline static constexpr Color PRINT_BG_COLOR = GDMYDebugUtils::PredefinedColor::DEFAULT_BG;
	inline static constexpr PerfStatsConfig PERF_STATS_CONFIG_DEFAULT = PerfStatsConfig{};

	static VarCmd VarCmdPrint(const String &in_text, const bool in_is_with_background);
	static VarCmd VarCmdSetPrintPos(const Vector2 &in_pos, const bool in_is_abs);
	static VarCmd VarCmdSetPrintColor(const Color &in_text_color);
	static VarCmd VarCmdDrawRect(
			const Rect2 &in_rect,
			const Color &in_rect_color,
			const bool in_is_filled,
			const real_t in_width,
			const bool in_is_abs);

	Viewport *get_root_viewport() const;
	void add_command(const VarCmd &new_cmd);
	void process_commands(RenderingServer *rs, const Ref<Font> &p_font, const Vector<VarCmd> &new_commands, const std::optional<Vector2> &root_viewport_size);

	// all refer CanvasItem::xxx
	void print_performance(RenderingServer *rs, const Ref<Font> &p_font);
	Size2 print_string(const Ref<Font> &p_font, const String &p_text, const Vector2 &p_position, const Color &text_color, const float font_size) const;
	void draw_rect(RenderingServer *rs, const Rect2 &p_rect, const Color &p_color, bool p_filled, real_t p_width);

	PerfStatsConfig get_current_perf_stats_config() const;
	PerfStatsConfig get_current_perf_stats_config_unlocked() const;

	RID canvas;
	RID canvas_item_text;
	RID canvas_item_shape;
	bool is_attached_scene_tree{ false };
	Mutex command_buffer_mutex{};
	Vector<VarCmd> command_buffer{};
	Mutex perf_stats_config_override_mutex{};
	PerfStatsConfigOverride perf_stats_config_override{};
};
#endif
