#ifndef MODULE_NAME
#error MODULE_NAME is not defined!
#endif

#ifdef _WIN32
#define MODULE_FN_EXPORT __declspec(dllexport)
#else
#define MODULE_FN_EXPORT
#endif

#define MODULE_CONCAT_(x, y) x##y
#define MODULE_CONCAT(x, y) MODULE_CONCAT_(x, y)

#define MODULE_LIBRARY_VAR MODULE_CONCAT(module_, MODULE_NAME)

#define MODULE_GEN_FN_TYPEDEF_NAME(x) MODULE_CONCAT(MODULE_CONCAT(MODULE_LIBRARY_VAR, _fn_), x)
#define MODULE_GEN_FN_NAME_VAR_NAME(x) MODULE_CONCAT(MODULE_GEN_FN_TYPEDEF_NAME(x), _name)
#define MODULE_GEN_VAR_TYPEDEF_NAME(x) MODULE_CONCAT(MODULE_CONCAT(MODULE_LIBRARY_VAR, _var_), x)
#define MODULE_GEN_VAR_NAME(x) MODULE_CONCAT(MODULE_CONCAT(MODULE_CONCAT(MODULE_LIBRARY_VAR, _var_), x), _name)

// just removes name of arguments so typedef is valid
#ifdef MODULE_IMPL
#define MODULE_FN_ARG(type, name) type name
#else
#define MODULE_FN_ARG(type, name) type
#endif

#ifdef MODULE_IMPL
#define MODULE_DECL_FN(prefix, ret, name, args, impl) MODULE_FN_EXPORT prefix ret name args impl
#else
#define MODULE_DECL_FN(prefix, ret, name, args, x) typedef ret (*MODULE_GEN_FN_TYPEDEF_NAME(name)) args ; \
    const static char* MODULE_GEN_FN_NAME_VAR_NAME(name) = #name;
#endif

#ifdef MODULE_IMPL
#define MODULE_DECL_VAR(prefix, type, name, value) prefix static type name = value;
#else
#define MODULE_DECL_VAR(prefix, type, name, value) typedef type MODULE_GEN_VAR_TYPEDEF_NAME(name); \
    const static char* MODULE_GEN_VAR_NAME(name) = #name;
#endif

