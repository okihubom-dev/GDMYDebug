// pair header
#include "GDMYDebugSharedUtility.h"
// others
#include "core/config/engine.h"

namespace GDMYDebugUtils {
bool is_game_now() {
	if (auto *engine = Engine::get_singleton()) {
		return !engine->is_editor_hint();
	}
	return false;
}
}

