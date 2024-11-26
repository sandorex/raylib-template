#include <raylib.h>
#include <hot_reload.hh>

const int screenWidth = 800;
const int screenHeight = 450;

HR_EXPORT void loop(bool* should_close) HR_FN_BODY({
    *should_close = WindowShouldClose();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangle(screenWidth/2 - 128, screenHeight/2 - 128, 256, 256, BLACK);
    DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, RAYWHITE);
    DrawText("Welp", screenWidth/2 - 44, screenHeight/2 + 48, 50, BLACK);

    EndDrawing();
})

int main(int argc, char* argv[]) {
    InitWindow(screenWidth, screenHeight, "hot-reload-raylib template demo");
    SetTargetFPS(60);

    bool should_close = false;

    HR_LOAD_LIB()

    while (!should_close) {
#if HR_EXE
        // reload key
        if (IsKeyDown(KEY_F1)) {
            HR_LOAD_LIB()
        }
#endif

        HR_FN(loop)(&should_close);
    }

    CloseWindow();

    return 0;
}
