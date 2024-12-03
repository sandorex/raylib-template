// #include <raylib.h>
// #include <hot_reload.hh>

#include <iostream>
const int screenWidth = 800;
const int screenHeight = 450;

// HR_EXPORT void loop(bool* should_close) HR_FN_BODY({
//     *should_close = WindowShouldClose();
//
//     BeginDrawing();
//
//     ClearBackground(RAYWHITE);
//
//     DrawRectangle(screenWidth/2 - 128, screenHeight/2 - 128, 256, 256, BLACK);
//     DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, RAYWHITE);
//     DrawText("Welp", screenWidth/2 - 44, screenHeight/2 + 48, 50, BLACK);
//
//     EndDrawing();
// })

#include <module_loader.h>
#include <test_module.h>

#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    module_t* module = module_load((char*)"./libmodule_test.so", 4);
    if (module == NULL) {
        std::cout << "error: " << module_error() << '\n';
        return 1;
    }

    // TODO should use a const pointer const char*
    module_symbol_t* fn = module_find_symbol(module, (char*)module_test_fn_func_name);
    if (fn == NULL) {
        std::cout << "error: " << module_error() << '\n';
        return 1;
    }

    while (true) {
        std::cout << "REloading\n";
        int success = module_reload(module);
        if (success != 0) {
            std::cout << "error reloading module: " << module_error() << '\n';
            return 1;
        }

        ((module_test_fn_func)fn->symbol)(5, 3);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    module_free(module);
    if (module == NULL) {
        std::cout << "error freeing module " << '\n';
        return 1;
    }

//     InitWindow(screenWidth, screenHeight, "hot-reload-raylib template demo");
//     SetTargetFPS(60);
//
//     bool should_close = false;
//
//     HR_LOAD_LIB()
//
//     while (!should_close) {
// #if HR_EXE
//         // reload key
//         if (IsKeyDown(KEY_F1)) {
//             HR_LOAD_LIB()
//         }
// #endif
//
//         HR_FN(loop)(&should_close);
//     }
//
//     CloseWindow();

    return 0;
}
