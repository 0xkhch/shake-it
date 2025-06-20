#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 800

#define MONITOR_WIDTH GetMonitorWidth(GetCurrentMonitor())
#define MONITOR_HEIGHT GetMonitorHeight(GetCurrentMonitor())

typedef struct {
    Vector2 pos;
    Vector2 size;
    Vector2 speed;
    Color color;
} Square_t;

Color color_array[6] = {ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED};
int color_index = 0;

void update_square(Square_t *square, Rectangle *boundry, Vector2 prev_boundry, Vector2 current_boundry, float dt) {
    // Calculate boundary velocity
    Vector2 boundry_velocity = {
        (current_boundry.x - prev_boundry.x) / dt,
        (current_boundry.y - prev_boundry.y) / dt
    };

    square->speed.y += 983 * dt;
    
    square->pos.y += square->speed.y * dt;
    square->pos.x += square->speed.x * dt;

    float min_bounce_speed = 50.0f;
    float bounce_factor = 0.75f;
    float floor_friction = 0.3f;
    float wall_friction = 0.2f;

    // Right wall collision
    if (square->pos.x >= (boundry->x + boundry->width) - square->size.x) {
        color_index = color_index > 5 ? 0 : color_index;
        square->color = color_array[color_index++];

        square->speed.x = -fabs(square->speed.x) + boundry_velocity.x * wall_friction;
        square->pos.x = (boundry->x + boundry->width) - square->size.x;
    }
    // Left wall collision
    else if (square->pos.x < boundry->x) {
        color_index = color_index > 5 ? 0 : color_index;
        square->color = color_array[color_index++];

        square->speed.x = fabs(square->speed.x) + boundry_velocity.x * wall_friction;
        square->pos.x = boundry->x;
    }
    // Floor collision
    else if (square->pos.y > (boundry->y + boundry->height) - square->size.y) {
        if (fabs(square->speed.y) > min_bounce_speed) {
            color_index = color_index > 5 ? 0 : color_index;
            square->color = color_array[color_index++];
        }

        square->speed.y = -fabs(square->speed.y) * bounce_factor + boundry_velocity.y;
        square->speed.x = square->speed.x * floor_friction + boundry_velocity.x;

        if (fabs(square->speed.y) < min_bounce_speed) {
            square->speed.y = 0;
        }
        square->pos.y = (boundry->y + boundry->height) - square->size.y;
    }
    // Ceiling collision
    else if (square->pos.y < boundry->y) {
        color_index = color_index > 5 ? 0 : color_index;
        square->color = color_array[color_index++];

        square->speed.y = fabs(square->speed.y) * bounce_factor + boundry_velocity.y * wall_friction;
        square->pos.y = boundry->y;
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Shake!");

    Square_t square = {
        .size = {.x = 100, .y = 100},
        .speed = {.x = 0, .y = 10},
        .color = RED,
    };

    // boundry size is the same as screen
    // so x and y are half that
    Rectangle boundry = {.x = MONITOR_WIDTH,
        .y = MONITOR_HEIGHT,
        .width = 800,
        .height = 800};

    printf("Width: %d Height: %d \n", MONITOR_WIDTH, MONITOR_HEIGHT);

    square.pos = (Vector2){.x = MONITOR_WIDTH / 2 - square.size.x / 2,
        .y = MONITOR_HEIGHT / 2 - square.size.x / 2};

    Camera2D camera = {0};
    camera.zoom = 1.0f;

    Vector2 prev_window = GetWindowPosition();

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        Vector2 current_window = GetWindowPosition();
        camera.target = current_window;

        boundry.x = current_window.x;
        boundry.y = current_window.y;

        update_square(&square, &boundry, prev_window, current_window, dt);

        prev_window = current_window;


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
