#include "module_loader.h"

#include <stdlib.h>

#if (defined(_WIN32) || defined(_WIN64))
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#else
#include <dlfcn.h>
#endif

int _close_handle(void* handle) {
#ifdef _WIN32
    return FreeLibrary(handle);
#else
    return dlclose(handle);
#endif
}

module_symbol_t* module_find_symbol(module_t* module, char* symbol_name) {
    // TODO better error?
    if (module->symbol_index >= module->symbols_length) {
        return NULL;
    }

    void* symbol = dlsym(module->handle, symbol_name);
    if (symbol == NULL) {
        return NULL;
    }

    module_symbol_t* module_symbol = &module->symbols[module->symbol_index];
    module_symbol->symbol = symbol;
    module_symbol->name = symbol_name;

    module->symbol_index++;

    return module_symbol;
}

module_t* module_load(char* path, size_t symbols_max) {
    void* handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        return NULL;
    }

    // TODO test if malloc and calloc succeded
    module_t* module = malloc(sizeof(module_t));
    module->path = path;
    module->handle = handle;
    module->symbols = calloc(symbols_max, sizeof(module_symbol_t));
    if (module->symbols == NULL) {
        return NULL;
    }

    module->symbol_index = 0;
    module->symbols_length = symbols_max;

    return module;
}

int module_reload(module_t* module) {
    // have to close the old handle first to reload it
    int success = _close_handle(module->handle);
    if (success != 0) {
        return 1;
    }

    // open new module handle
    module->handle = dlopen(module->path, RTLD_NOW | RTLD_LOCAL);
    if (module->handle == NULL) {
        return 1;
    }

    // update all symbols
    for (size_t i = 0; i < module->symbol_index; ++i) {
        void* symbol = dlsym(module->handle, module->symbols[i].name);
        if (symbol == NULL) {
            return 1;
        }

        // update pointer
        module->symbols[i].symbol = symbol;
    }

    return 0;
}

int module_free(module_t* module) {
    int success = _close_handle(module->handle);
    if (success != 0) {
        return 1;
    }

    free(module->symbols);
    free(module);

    return 0;
}

char* module_error() {
    return dlerror();
}

