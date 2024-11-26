#pragma once

#if defined(HR_EXE) && !defined(HR_LIB_NAME)
#error HR_LIB_NAME not defined in executable!
#endif

// NOTE cannot read mangled names so each function needs to be extern C and exported (windows)
#ifdef HR_LIB
#ifdef _WIN32
#define HR_EXPORT extern "C" __declspec(dllexport)
#else
#define HR_EXPORT extern "C"
#endif
#else
#define HR_EXPORT
#endif

#ifdef HR_EXE
#include <dylib.hpp>
#include <optional>

static std::optional<dylib> HR_DYLIB = {};

// void hr_reload_lib() {
//     HR_DYLIB.reset();
//     HR_DYLIB = dylib("./", HR_LIB_NAME);
// }
//
// dylib& hr_get_lib() {
//     if (!HR_DYLIB.has_value()) {
//         hr_reload_lib();
//     }
//
//     return HR_DYLIB.value();
// }
#endif

#ifdef HR_EXE
#define HR_LOAD_LIB() HR_DYLIB.reset(); HR_DYLIB = dylib("./", HR_LIB_NAME);
#else
#define HR_LOAD_LIB()
#endif

// get the pre-existing function by name (gets the type from locally defined symbol)
#ifdef HR_EXE
#define HR_FN(NAME) HR_DYLIB.value().get_function<decltype(NAME)>(#NAME)
#else
#define HR_FN(NAME) NAME
#endif

#ifdef HR_EXE
#define HR_VAR(NAME) HR_DYLIB.value().get_variable<decltype(NAME)>(#NAME)
#else
#define HR_VAR(NAME) NAME
#endif

// macro to remove code in the executable as the code is gonna be executed from the library
#ifdef HR_EXE
#define HR_FN_BODY(X) ;
#else
#define HR_FN_BODY(X) X
#endif

