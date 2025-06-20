#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 800

#define MONITOR_WIDTH GetMonitorWidth(GetCurrentMonitor())
#define MONITOR_HEIGHT GetMonitorHeight(GetCurrentMonitor())

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
int color_index = 0;

void update_square(Square_t* square, Rectangle* boundry, float dt) {
    square->speed.y += 983 * dt;
    square->pos.y += square->speed.y * dt;

    if (square->pos.x >= (boundry->x + boundry->width) - square->size.x) {
        square->pos.x = (boundry->x + boundry->width) - square->size.x;
    }
    if (square->pos.x < boundry->x) square->pos.x = boundry->x;
    if (square->pos.y > (boundry->y + boundry->height) - square->size.y) {
        if (!square->on_ground) {
            square->speed.y *= -0.8f;
            color_index = color_index > 5 ? 0 : color_index;
            square->color = color_array[color_index];
            square->on_ground = true;
            color_index++;
        }
        square->pos.y = (boundry->y + boundry->height) - square->size.y;
    } else {
        square->on_ground = false;
    }
    if (square->pos.y < boundry->y) square->pos.y = boundry->y;

}

int main() {
	InitWindow(WIDTH, HEIGHT, "Shake!");

    Square_t square = {
        .size = {.x = 100, .y = 100},
        .speed = {.x = 0, .y = 10},
        .color = RED,
        .on_ground = false
    };

    // boundry size is the same as screen
    // so x and y are half that
    Rectangle boundry = {
        .x = MONITOR_WIDTH/2 - 200,
        .y = MONITOR_HEIGHT/2 - 200,
        .width = 400, .height = 400
    };

    printf("Width: %d Height: %d \n", MONITOR_WIDTH, MONITOR_HEIGHT);

    square.pos = (Vector2){
        .x = MONITOR_WIDTH/2 - square.size.x/2, 
        .y = MONITOR_HEIGHT/2 - square.size.x/2
    };

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    SetTargetFPS(60);
	while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        Vector2 window_pos = GetWindowPosition();
        camera.target = window_pos;

        boundry.x = window_pos.x  + boundry.width/2;
        boundry.y = window_pos.y + boundry.height/2;

        update_square(&square, &boundry, dt);

		BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(BLACK);
                DrawRectangleV(square.pos, square.size, square.color);
                DrawRectangleLines(boundry.x, boundry.y, boundry.width, boundry.height, RED);
            EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
