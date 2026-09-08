#pragma once

#if defined(DEBUG_ENABLED) || defined(DEV_ENABLED)
#define GDMYDEBUG_ENABLE_IMPL 1
#else
#define GDMYDEBUG_ENABLE_IMPL 0
#endif

#include "scene/main/node.h"
#include "core/object/object.h"
#include "core/os/mutex.h"
#include "core/templates/vector.h"

// forward declaration
class Control;
class GDMYDebugPrintScreenOverlay;
class Font;


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
			SET_POSITION,
			SET_COLOR,
		};

		Cmd() = default;

		static Cmd PrintCmd(const String &in_text) {
			Cmd ret;
			ret.type = Type::PRINT;
			ret.text = in_text;
			return ret;
		}

		static Cmd SetPositionCmd(const Vector2 &in_pos) {
			Cmd ret;
			ret.type = Type::SET_POSITION;
			ret.position = in_pos;
			return ret;
		}

		static Cmd SetColorCmd(const Color &in_color) {
			Cmd ret;
			ret.type = Type::SET_COLOR;
			ret.color = in_color;
			return ret;
		}

		Type type{ Type::NONE };
		Vector2 position{ Vector2() };
		Color color{};
		String text{};
	};

	GDMYDebug();
	~GDMYDebug();

	void flush();
	void set_color(const int32_t r, const int32_t g, const int32_t b, const int32_t a = 255);
	void set_position(const int32_t pos_x, const int32_t pos_y);
	void print(const String &p_text);
	int32_t get_font_size() const;
	void try_attach_scene_tree();

private:
	inline static GDMYDebug *singleton = nullptr;
	inline static constexpr int DEBUG_FONT_SIZE = 12;
	inline static constexpr Color DEBUG_FONT_COLOR = Color(255 / 255.f, 255 / 255.f, 255 / 255.f);
	inline static constexpr int PERF_STATS_FONT_SIZE = 12;
	inline static constexpr Color PERF_STATS_FONT_COLOR = Color(221 / 255.f, 34 / 255.f, 136 / 255.f);

	void print_performance(const Ref<Font> &p_font) const;
	void process_print_commands(const Ref<Font> &p_font, const Vector<Cmd> &new_commands);
	void print_string(const Ref<Font> &p_font, const String &p_text, const Vector2 &p_position, const Color &text_color, const float font_size) const;

	void add_print_commands(const Cmd &new_cmd);

	RID canvas;
	RID canvas_item;
	bool is_attached_scene_tree{ false };
	Mutex command_buffer_mutex{};
	Vector<Cmd> command_buffer{};
};
