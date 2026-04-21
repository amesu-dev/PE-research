#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

#include <windows.h>
#include <winnt.h>

#include "utils.h"



int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: main <path to file>\n");
        return 1;
    }

    void* fcontext;
    bool ok = load_file(argv[1], &fcontext);
    if(!ok) return 1;

    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)fcontext;
    
    printf("Entry: %2s\n", &dos_header->e_magic);
    printf("%s\n", (char*)fcontext);

    if(dos_header->e_magic != IMAGE_DOS_SIGNATURE) {
        printf("That's not an executable file!\n");
        return 1;
    }

    PIMAGE_NT_HEADERS64 coff_header = (PIMAGE_NT_HEADERS64)((ULONG_PTR)dos_header + dos_header->e_lfanew);
    printf("Entry: %2s\n", coff_header);

    if (coff_header->Signature != IMAGE_NT_SIGNATURE) {
        printf("That's not a PE file!\n");
        return 1;
    }

    //// --------------------------
    //// Import table
    // Virtual address (in exe memory space)
    long rva_import_table = coff_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    PIMAGE_SECTION_HEADER section = find_nearest_section(IMAGE_FIRST_SECTION(coff_header), sizeof(IMAGE_SECTION_HEADER), rva_import_table);
    printf("Import section: %s\n", &section->Name);

    // Raw relative address (in file address space)
    long rra_import_table = rva_import_table - section->VirtualAddress + section->PointerToRawData;

    // Pointer to import table loaded in CURRENT memory space 
    PIMAGE_IMPORT_DESCRIPTOR p_import_table = (PIMAGE_IMPORT_DESCRIPTOR)((long)fcontext + rra_import_table);
    printf("Import table address: %p\n", p_import_table);

    for (
        PIMAGE_IMPORT_DESCRIPTOR p_target = p_import_table;
        !is_zeroed(p_target, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        p_target += 1
    ) {
        char* name_ptr = (char*)p_target->Name - rva_import_table + (long)p_import_table;
        // p_target->OriginalFirstThunk -> IMAGE_THUNK_DATA64 -> IMAGE_IMPORT_BY_NAME
        PIMAGE_THUNK_DATA64 p_thunk = (PIMAGE_THUNK_DATA64)(p_target->OriginalFirstThunk - rva_import_table + (long)p_import_table);
        PIMAGE_IMPORT_BY_NAME p_import_by_name = (PIMAGE_IMPORT_BY_NAME)(p_thunk->u1.AddressOfData - rva_import_table + (long)p_import_table);

        printf("Found import at %p: %s\n", p_target, name_ptr);
        printf("First import (%4x): %s\n", p_import_by_name->Hint, &p_import_by_name->Name);
    }

    //// --------------------------
    // 


    printf("Process successed!\n");
    return 0;
}