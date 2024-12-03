#define MODULE_NAME test
#include "module.h"
#include "stdio.h"

MODULE_DECL_VAR(const, int, version, 333)

MODULE_DECL_FN(,void, func, (MODULE_FN_ARG(int, a), MODULE_FN_ARG(int, b)), {
    printf("Hello there from module version %d, with args a %d and b %d\n", version, a, b);
})
