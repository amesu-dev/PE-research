 #include "utils.h"

 #include <malloc.h>
 #include <stdio.h>



// ---------------------
// Basic
bool is_zeroed(void* buffer, size_t size) {
    char* ptr = buffer;
    while(size) {
        char step_size = 0;
        switch(size) {
            case sizeof(char):
                if (*(char*)ptr != 0) return 0;
                step_size = sizeof(char);
                break;
            case sizeof(short):
                if (*(short*)ptr != 0) return 0;
                step_size = sizeof(short);
                break;
            case sizeof(int):
                if (*(int*)ptr != 0) return 0;
                step_size = sizeof(int);
                break;
            // case sizeof(long):
                // if (*(char*)ptr != 0) return 0;
                // step_size = sizeof(long);
                // break;
            case sizeof(long long):
            default:
                if (*(long long*)ptr != 0) return 0;
                step_size = sizeof(long long);
        }

        ptr += step_size;
        size -= step_size;
    }

    return 1;
}
inline bool load_file(char* path, void** o_context) { // Load file to memory
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    long long size = ftell(file);

    if (size > 32 * 1024 * 1024) {
        printf("File too big\n");
        return 0;
    }

    *o_context = malloc(size * sizeof(char));

    rewind(file);
    fread(*o_context, 1, size, file); 
    fclose(file);

    return 1;
}

// ---------------------
// Windows NT

PIMAGE_SECTION_HEADER find_nearest_section(PIMAGE_SECTION_HEADER first, int count, ULONG_PTR va) {
    PIMAGE_SECTION_HEADER nearest = first;
    for (PIMAGE_SECTION_HEADER section = first; section < first + count; section += 1) {
        if (va - nearest->VirtualAddress < va - section->VirtualAddress) continue;
        nearest = section;
    }
    return nearest;
}

