#pragma once
#include <stdbool.h>
#include <stddef.h>

#include <windows.h>
#include <winnt.h>


// ---------------------
// Basic
bool is_zeroed(void* buffer, size_t size);
inline bool load_file(char* path, void** o_context);

// ---------------------
// Windows NT
PIMAGE_SECTION_HEADER find_nearest_section(PIMAGE_SECTION_HEADER first, int count, ULONG_PTR va);

