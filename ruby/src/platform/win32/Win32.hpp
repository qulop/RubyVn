#pragma once

#include <platform/api/PlatformAPI.hpp>

#undef interface    // Warning C4005 in MSVC: "interface" macro redefinition in "combaseapi.h"
#include <Windows.h>
#undef interface
#define interface struct

#include "audio/WaveOutAudioOutputStream.hpp"

#ifdef _DEBUG
#define HEAP_ALLOC_FLAGS HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY
#else
#define HEAP_ALLOC_FLAGS HEAP_ZERO_MEMORY
#endif