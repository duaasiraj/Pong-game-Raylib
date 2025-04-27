#include <raylib.h>

int playerScore = 0;
int cpuScore = 0;

class Ball {
public:
    float x, y;
    int speedx, speedy;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speedx;
        y += speedy;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speedy *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            cpuScore++;
            ResetBall();
        }

        if (x - radius <= 0) {
            playerScore++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        int speedchoice[2] = {1, -1};
        speedx = 7 * speedchoice[GetRandomValue(0, 1)];
        speedy = 7 * speedchoice[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void Limit() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }
        Limit();
    }
};

class CPUPaddle : public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y -= speed;
        }
        if (y + height / 2 < ball_y) {
            y += speed;
        }
        Limit();
    }
};

CPUPaddle cpu;
Ball ball;
Paddle player;

int main() {
    const int screenWidth = 1280;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Pong game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screenWidth / 2;
    ball.y = screenHeight / 2;
    ball.speedx = 7;
    ball.speedy = 7;

    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight / 2 - player.height / 2;
    player.speed = 10;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screenHeight / 2 - cpu.height / 2;
    cpu.speed = 10;

    bool gameOver = false;
    const int maxScore = 5;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (playerScore >= maxScore || cpuScore >= maxScore) {
            gameOver = true;
        }

        if (gameOver) {
            const char* winner = playerScore >= maxScore ? "Player Wins!" : "CPU Wins!";
            DrawText(winner, screenWidth / 2 - MeasureText(winner, 60) / 2, screenHeight / 2 - 30, 60, GREEN);
            DrawText("Press ENTER to Restart", screenWidth / 2 - MeasureText("Press ENTER to Restart", 30) / 2, screenHeight / 2 + 50, 30, WHITE);

            if (IsKeyPressed(KEY_ENTER)) {
                playerScore = 0;
                cpuScore = 0;
                gameOver = false;
                ball.ResetBall();
            }

            EndDrawing();
            continue;
        }

        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speedx *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speedx *= -1;
        }

        ball.Draw();
        player.Draw();
        cpu.Draw();

        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
