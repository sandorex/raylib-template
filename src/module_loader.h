#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    char* name;
    void* symbol;
} module_symbol_t;

// holds the raw library pointer and path to it so it can be reloaded
typedef struct {
    char* path;
    void* handle;
    module_symbol_t* symbols;
    size_t symbol_index;
    size_t symbols_length;
} module_t;

#ifdef __cplusplus
extern "C" {
#endif

// the symbol name is not freed automatically
module_symbol_t* module_find_symbol(module_t* module, char* symbol_name);
module_t* module_load(char* path, size_t symbols_max);
int module_reload(module_t* module);
int module_free(module_t* module);

char* module_error();

#ifdef __cplusplus
}
#endif
