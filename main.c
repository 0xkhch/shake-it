#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    Color color;
    bool on_ground;
} Square_t;
Color color_array[6] = {
    ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED
};

int main() {
	InitWindow(800, 600, "Shake!");
    Square_t square = {
        .size = {.x = 100, .y = 100},
        .speed = {.x = 0, .y = 10},
        .color = RED,
        .on_ground = false
    };
    square.pos = (Vector2){
        .x = (GetScreenWidth()/2 - square.size.x/2), 
        .y = GetScreenHeight()/2 - square.size.x/2
    };
    SetTargetFPS(60);
    int i = 0;
	while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 window_pos = GetWindowPosition();
        printf("Pos: %f %f\n", window_pos.x, window_pos.y);
        square.speed.y += 983 * dt;
        square.pos.y += square.speed.y * dt;
        if (square.pos.x >= GetScreenWidth() - square.size.x) {
            square.pos.x = GetScreenWidth() - square.size.x;
        }
        if (square.pos.x < square.size.x) square.pos.x = square.size.x;
        if (square.pos.y > GetScreenHeight() - square.size.y) {
            if (!square.on_ground) {
                square.speed.y *= -0.8f;
                square.color = color_array[i % 6];
                i++;
                square.on_ground = true;
            }
            square.pos.y = GetScreenHeight() - square.size.y;
        } else {
            square.on_ground = false;
        }
        if (square.pos.y < square.size.y) square.pos.y = square.size.y;
		BeginDrawing();
			ClearBackground(BLACK);
            DrawRectangleV(square.pos, square.size, square.color);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
