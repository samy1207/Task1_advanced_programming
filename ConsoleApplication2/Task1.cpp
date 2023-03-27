#include <iostream>
#include <ctime>
#include <string>
#include <stdio.h>
#include <fstream>
#include <chrono>

#define _CRT_SECURE_NO_WARNINGS

class GameComponent {
public:
    GameComponent() : id(++instances) {}

    void Update() {
        std::time_t t = std::time(nullptr);
        char timeStr[26];

        std::asctime_s(timeStr, sizeof timeStr, std::localtime(&t));
        std::cout << "GameComponent " << id << " updated at " << timeStr;
    }

protected:
    int id;
    static int instances;
};

int GameComponent::instances = 0;

enum Direction {
    Up, Down, Left, Right
};

class DrawableGameComponent : public GameComponent {
public:
    DrawableGameComponent() {
        x = y = 0;
        direction = Right;
    }

    void Update() {
        GameComponent::Update();

        // Update position
        switch (direction) {
        case Up:
            y = std::max(0, y - 1);
            break;
        case Down:
            y = std::min(20, y + 1);
            break;
        case Left:
            x = std::max(0, x - 1);
            break;
        case Right:
            x = std::min(80, x + 1);
            break;
        }

        // Change direction if necessary
        Direction newDirection;
        do {
            newDirection = static_cast<Direction>(std::rand() % 4);
        } while (newDirection == direction);
        direction = newDirection;

        Draw();
    }

    void Draw() {
        std::cout << "DrawableGameComponent " << id << " position (" << x << ", " << y << ") direction " << direction << std::endl;
    }

    void ChangeDirection() {
        Direction newDirection;
        do {
            newDirection = static_cast<Direction>(std::rand() % 4);
        } while (newDirection == direction);
        direction = newDirection;
    }

private:
    int x, y;
    Direction direction;
};

typedef void(*FP)();

class Game {
public:
    void initialiseGame() {
        componentCount = 0;
        initialise = nullptr;
        terminate = nullptr;
    }

    void Add(GameComponent* component) {
        components[componentCount++] = component;
    }

    void Run() {
        if (initialise != nullptr) {
            initialise();
        }

        while (true) {
            for (int i = 0; i < componentCount; i++) {
                components[i]->Update();
            }
        }

        if (terminate != nullptr) {
            terminate();
        }
    }

    void SetInitialise(FP function) {
        initialise = function;
    }

    void SetTerminate(FP function) {
        terminate = function;
    }

private:
    GameComponent* components[100];
    int componentCount;
    FP initialise;
    FP terminate;
};

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game game;
    game.initialiseGame();

    DrawableGameComponent drawableGameComponent;
    game.Add(&drawableGameComponent);

    game.Run();

    return 0;
}
