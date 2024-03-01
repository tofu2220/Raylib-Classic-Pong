#include "raylib.h"

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        // Collision
        if (y + radius >= GetScreenHeight() || y - radius <= 0) speed_y *= -1;
        if (x + radius >= GetScreenWidth()) {
            cpu_score++;
            ResetBall();
        }

        if (x - radius <= 0) {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
protected:
    void LimitMovement() {
        if (y <= 0)                          y = 0;
        if (y + height >= GetScreenHeight()) y = GetScreenHeight() - height;
    }


public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() {
        // Control
        if (IsKeyDown(KEY_UP))      y -= speed;
        if (IsKeyDown(KEY_DOWN))    y += speed;

        LimitMovement();
    }

};

class CpuPaddle: public Paddle
{
public:
    void Update(int ball_y) {
        if (y + height/2 > ball_y)  y = y - speed;
        if (y + height/2 <= ball_y) y = y + speed;

        LimitMovement();
    }
    
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() 
{
    InitWindow(800, 600, "Raylib Classic Pong");
    SetTargetFPS(60);

    ball.radius = 10;
    ball.x = GetScreenWidth() / 2;
    ball.y = GetScreenHeight() / 2;
    ball.speed_x = 5;
    ball.speed_y = 5;

    player.width = 15;
    player.height = 100;
    player.x = GetScreenWidth() - player.width - 10;
    player.y = (GetScreenHeight() - player.height) / 2;
    player.speed = 10;

    cpu.width = 15;
    cpu.height = 100;
    cpu.x = 10;
    cpu.y = (GetScreenHeight() - cpu.height) / 2;
    cpu.speed = 10;


    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Checking for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) 
            ball.speed_x *= -1;
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) 
            ball.speed_x *= -1;
            
        // Drawing
        ClearBackground(BLACK);
        DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();

        DrawText(TextFormat("%i", cpu_score), (GetScreenWidth() - 80) / 4, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*(GetScreenWidth() - 80) / 4, 20, 80, WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}