#pragma once
#include <windows.h>
#include <winnt.h>

typedef enum {
    UNKOWN = -1,
    OK = 0,

    WRONG_FILE_FORMAT,
    MEMORY_NOT_ENOUGH
} STATUS;

// ---------------------
// Base
struct FILE_INFO {
    long long size;
    PIMAGE_DOS_HEADER dos_header;
    PIMAGE_NT_HEADERS64 coff_header;
    
    // WINNT PROCESSOR_<arch>
    short arch;

    char is_pe : 1;
    char is_64 : 1;
};

/**
* @return {STATUS} status code
*/
int parse_file(char* path, struct FILE_INFO* o_info);

// ---------------------
// Import table
struct IMPORT_TABLE_INFO {
    // Raw relative address (in file address space)
    long ra;
    // Virtual address (in exe memory space)
    long rva;
    // Pointer to import table loaded in CURRENT memory space
    PIMAGE_IMPORT_DESCRIPTOR ptr;
};

/**
* @return {STATUS} status code
*/
int get_import_table(const struct FILE_INFO* const file, struct IMPORT_TABLE_INFO* o_info);
