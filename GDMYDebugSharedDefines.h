#pragma once

#include "core/math/color.h"

namespace GDMYDebugUtils {
namespace PredefinedColor {
constexpr Color TRANSPARENT = Color(
		0 / 255.f,
		0 / 255.f,
		0 / 255.f,
		0 / 255.f);
constexpr Color WHITE = Color(
		255 / 255.f,
		255 / 255.f,
		255 / 255.f,
		255 / 255.f);
constexpr Color BLACK = Color(
		0 / 255.f,
		0 / 255.f,
		0 / 255.f,
		255 / 255.f);
constexpr Color RED = Color(
		255 / 255.f,
		0 / 255.f,
		0 / 255.f,
		255 / 255.f);
constexpr Color GREEN = Color(
		0 / 255.f,
		255 / 255.f,
		0 / 255.f,
		255 / 255.f);
constexpr Color BLUE = Color(
		0 / 255.f,
		0 / 255.f,
		255 / 255.f,
		255 / 255.f);
constexpr Color CYAN = Color(
		0 / 255.f,
		255 / 255.f,
		255 / 255.f,
		255 / 255.f);
constexpr Color MAGENTA = Color(
		255 / 255.f,
		0 / 255.f,
		255 / 255.f,
		255 / 255.f);
constexpr Color YELLOW = Color(
		255 / 255.f,
		255 / 255.f,
		0 / 255.f,
		255 / 255.f);
} //namespace Color

} //namespace GDMYDebugUtils
