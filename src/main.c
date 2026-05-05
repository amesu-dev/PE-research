#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

#include <windows.h>
#include <winnt.h>

#include "utils.h"
#include "pe.h"


int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: main <path to file>\n");
        return 1;
    }

    struct FILE_INFO file_info;
    int code = parse_file(argv[1], &file_info);
    if (code == MEMORY_NOT_ENOUGH) {
        printf("ERR: File too big");
        return 1;
    } else if (code == WRONG_FILE_FORMAT) {
        printf("ERR: File wrong format");
        return 1;
    } 
    printf("Entry: %2s\n", &file_info.dos_header->e_magic);
    printf("Entry: %2s\n", file_info.coff_header);

    //// --------------------------
    //// Import table

    struct IMPORT_TABLE_INFO import_info;
    get_import_table(&file_info, &import_info);
    
    printf("Import table address: %p\n", import_info.ptr);

    for (
        PIMAGE_IMPORT_DESCRIPTOR p_entry = import_info.ptr;
        !is_zeroed(p_entry, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        p_entry += 1
    ) {
        char* name_ptr = (char*)p_entry->Name - import_info.rva + (long)import_info.ptr;
        printf("Found import at %p(0x%-6X): %s\n", p_entry, p_entry->OriginalFirstThunk, name_ptr);
        // p_entry->OriginalFirstThunk -> IMAGE_THUNK_DATA64 -> IMAGE_IMPORT_BY_NAME
        
        for (
            PIMAGE_THUNK_DATA64 p_thunk = p_entry->OriginalFirstThunk - import_info.rva + (long)import_info.ptr;
            !is_zeroed(p_thunk, sizeof(IMAGE_THUNK_DATA64));
            p_thunk += 1
        ) {
            PIMAGE_IMPORT_BY_NAME p_import_by_name = (PIMAGE_IMPORT_BY_NAME)(p_thunk->u1.AddressOfData - import_info.rva + (long)import_info.ptr);
            printf("\t (%4X): %s\n", p_thunk->u1.AddressOfData, &p_import_by_name->Name);
        }
    }

    //// --------------------------
    // 


    printf("Process successed!\n");
    return 0;
}