#include <C:\Users\KKKer\SFML-2.5.1\include\SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <sstream>

using namespace sf;
using namespace std;

////////// setup ////////////
const int width = 30;
const int height = 30;
const int cell_size = 20;
int w = (cell_size + 1) * width;
int h = (cell_size + 1) * height;
RenderWindow window(VideoMode(w, h), "Snake");

bool Alive = 1;
int s_size = 3;
int dir = 1;
float speed;
Vector2i snake[400];

int score = 0;
string myScore = "Score: ";
string gameOverString = "GAME\nOVER";

bool isFast;
int fruitType[2];
Vector2i fruit[2];
int eatenFruit;
////////////////////////////

void snakeSetup()
{
    for (int i = 0; i < s_size; i++)
    {
        snake[i].x = width / 2;
        snake[i].y = height / 2 - (1 - i);
    }
}

bool checkFruitInSnake()
{
    for (int i = 0; i < s_size; i++)
    {
        if (snake[i].x == fruit[0].x && snake[i].y == fruit[0].y)
        {
            eatenFruit = 1;
            switch (fruitType[0])
            {
                case 0: isFast = 0; score++; break;
                case 1: isFast = 1; break;
                case 2: isFast = 0; score+= ((rand() % 2) * 3) - 1; break;
            }
            return 1;
        }
        else if (snake[i].x == fruit[1].x && snake[i].y == fruit[1].y)
        {
            eatenFruit = 2;
            switch (fruitType[1])
            {
                case 0: isFast = 0; score++; break;
                case 1: isFast = 1; break;
                case 2: isFast = 0; score+= ((rand() % 2) * 3) - 1; break;
            }
            return 1;
        }
    }
    return 0;
}

bool checkSnakeInSnake()
{
    for (int i = 1; i < s_size; i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return 1;
        }
    }
    return 0;
}

void isAlive()
{
    if (snake[0].x < 0 || snake[0].x > width - 1 || snake[0].y < 0 || snake[0].y > height - 1)
    {
        Alive = 0;
    }
    else if (checkSnakeInSnake())
    {
        Alive = 0;
    }
}

void snakeUpdate()
{
    for (int i = s_size - 1; i > 0; i--)
    {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    switch (dir)
    {
        case 1: snake[0].y--; break;
        case 2: snake[0].x++; break;
        case 3: snake[0].y++; break;
        case 4: snake[0].x--; break;
        default: break;
    }
}

void mapDraw()
{
    for (int i = 0; i <= height; i++)
    {
        int len = (height + 1) + (height * cell_size);
        RectangleShape line(Vector2f(1, len));
        line.setPosition(i * (cell_size + 1), 0);
        window.draw(line);
    }
    for (int j = 0; j <= width; j++)
    {
        int len = (width + 1) + (width * cell_size);
        RectangleShape line(Vector2f(1, len));
        line.setPosition(0, j * (cell_size + 1) + 1);
        line.rotate(-90);
        window.draw(line);
    }
    //// ���������� ��� ������ ����������
}

void snakeDraw()
{
    for (int i = 0; i < s_size; i++)
    {
        int x = snake[i].x * (cell_size + 1) + 1;
        int y = snake[i].y * (cell_size + 1) + 1;
        RectangleShape seg(Vector2f(cell_size, cell_size));
        seg.setFillColor(Color(0, 250, 0));
        seg.setPosition(Vector2f(x, y));
        window.draw(seg);
    }

}

void fruitGenerate()
{
    bool n = 1;
    while (n)
    {
        fruitType[eatenFruit - 1] = rand() % 3;
        fruit[eatenFruit - 1].x = rand() % width;
        fruit[eatenFruit - 1].y = rand() % height;

        for (int i = 0; i < s_size; i++)
        {
            if (snake[i].x == fruit[eatenFruit - 1].x && snake[i].y == fruit[eatenFruit - 1].y)
            {
                n += 1;
            }
        }
        n -= 1;
    }
}

void fruitDraw()
{
    for (int i = 0; i < 2; i++)
    {
        int x = fruit[i].x * (cell_size + 1) + 1;
        int y = fruit[i].y * (cell_size + 1) + 1;
        RectangleShape fruit(Vector2f(cell_size, cell_size));

        switch (fruitType[i])
        {
            case 0: fruit.setFillColor(Color::Red); break;
            case 1: fruit.setFillColor(Color::Yellow); break;
            case 2: fruit.setFillColor(Color::Magenta); break;
        }
        fruit.setPosition(Vector2f(x, y));
        window.draw(fruit);
    }
}

void gameOver(Text gameOverText)
{
    window.draw(gameOverText);
}

int main()
{
    snakeSetup();

    fruit[0].x = rand() % 15; fruit[0].y = rand() % height;
    fruit[1].x = rand() % 15 + 15, fruit[1].y = rand() % height;
    fruitType[0] = rand() % 3; fruitType[1] = rand() % 3;

    setlocale (LC_ALL, "Russian");
    Clock clock;

    Font font;
    font.loadFromFile("font4game.otf");

    Text scoreText ("", font, 30);
    scoreText.setColor(Color::Magenta);

    Text gameOverText ("", font, 100);
    gameOverText.setColor(Color::Red);
    gameOverText.setString(gameOverString);
    gameOverText.setPosition(w / 2 - 120, h / 2 - 130);

    while (window.isOpen())
    {

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time;
        speed += (time * (0.000006 * (isFast + 1)));

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && dir != 3) dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Down) && dir != 1) dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Right) && dir != 4) dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Left) && dir != 2) dir = 4;


        ostringstream scoreString;
        scoreString << score;
        scoreText.setString(myScore + scoreString.str());
        scoreText.setPosition(w / 2 - 55, 20);

        isAlive();

        window.clear();

        if (Alive)
        {
            if (speed > 1)
            {
                if (checkFruitInSnake())
                {
                    fruitGenerate();
                    s_size++;
                }
                snakeUpdate();
                speed = 0;
            }
            //mapDraw();
            fruitDraw();
            snakeDraw();
            window.draw(scoreText);
        }
        else
        {
            gameOver(gameOverText);
            if (speed > 5)
            {
                cout << "Your score is: " << score << endl;
                window.close();
            }
        }

        window.display();
    }
    return 0;
}
