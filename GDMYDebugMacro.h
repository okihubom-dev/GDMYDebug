#pragma once

#if defined(DEBUG_ENABLED) || defined(DEV_ENABLED)
#define GDMYDEBUG_ENABLE_IMPL 1
#else
#define GDMYDEBUG_ENABLE_IMPL 0
#endif
