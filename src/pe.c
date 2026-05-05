#include "pe.h"
#include "utils.h"
#include <stdbool.h>

// ---------------------
// Base
int parse_file(char* path, struct FILE_INFO* o_info) {
    bool ok = load_file(path, &o_info->dos_header);
    if(!ok) return MEMORY_NOT_ENOUGH;

    if(o_info->dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
        return WRONG_FILE_FORMAT;
    }

    o_info->coff_header = (ULONG_PTR)o_info->dos_header + o_info->dos_header->e_lfanew;

    if (o_info->coff_header->Signature != IMAGE_NT_SIGNATURE) {
        return WRONG_FILE_FORMAT;
    }

    return OK;
}

// ---------------------
// Import table
int get_import_table(const struct FILE_INFO* file, struct IMPORT_TABLE_INFO* o_info) {
    struct IMPORT_TABLE_INFO info = {};

    o_info->rva = file->coff_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    PIMAGE_SECTION_HEADER section = find_nearest_section(IMAGE_FIRST_SECTION(file->coff_header), sizeof(IMAGE_SECTION_HEADER), o_info->rva);

    o_info->ra = o_info->rva - section->VirtualAddress + section->PointerToRawData;
    o_info->ptr = (long long)file->dos_header + o_info->ra;

    return OK;
}
