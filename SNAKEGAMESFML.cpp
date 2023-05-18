/*
Project developed by M.Abdullah May 18, 2023, 12:30 PM
This game is my Computer Science project.
This is only for educational purpose.
If you want to use this code for commercial purpose then you have to take permission from the developer.
 */
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

class Game {
public:
    Game(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {
        window.create(sf::VideoMode(windowWidth, windowHeight), "SFML Snake Game");
        window.setFramerateLimit(20);
        window.setMouseCursorVisible(false);
        window.setMouseCursorGrabbed(true);
        srand(static_cast<unsigned int>(time(NULL)));
    }

    void run() {
        setup();
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    int windowWidth;
    int windowHeight;
    sf::RenderWindow window;
    sf::RectangleShape background;
    sf::Font font;
    sf::Text scoreText;
    sf::SoundBuffer buffer;
    sf::Sound fruitSound;
    std::vector<sf::RectangleShape> blocks;
    sf::CircleShape fruit;

    int score;
    bool isLeft;
    bool isRight;
    bool isUp;
    bool isDown;

    sf::Clock clock;
    const float moveInterval = 0.1f;
    float timeSinceLastMove = 0.f;

    void setup() {
        background.setSize(sf::Vector2f(windowWidth, windowHeight));
        background.setFillColor(sf::Color(136, 139, 141));

        if (!font.loadFromFile("F:/CLionProjects/SnakeGameSFML/Lindelof-Font.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }

        scoreText.setFont(font);
        scoreText.setString("Score: 0");
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(350, 10);

        if (!buffer.loadFromFile("F:/CLionProjects/SnakeGameSFML/eating.wav")) {
            std::cerr << "Error loading voice!" << std::endl;
        }

        fruitSound.setBuffer(buffer);

        sf::RectangleShape headBlock;
        headBlock.setSize(sf::Vector2f(20.f, 20.f));
        headBlock.setFillColor(sf::Color(75, 0, 130));
        headBlock.setPosition(sf::Vector2f(200.f, 200.f));
        blocks.push_back(headBlock);

        fruit.setRadius(10.f);
        sf::Vector2f fruitPosition(rand() % (windowWidth - 20), rand() % (windowHeight - 20));
        fruit.setFillColor(sf::Color(255, 255, 0));
        fruit.setPosition(fruitPosition);

        score = 0;
        isLeft = false;
        isRight = false;
        isUp = false;
        isDown = false;
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }

    void update() {
        if (fruit.getGlobalBounds().intersects(blocks[0].getGlobalBounds())) {
            fruitSound.play();

            sf::RectangleShape newBlock(sf::Vector2f(20.f, 20.f));
            newBlock.setFillColor(sf::Color(255, 203, 164));
            newBlock.setPosition(blocks.back().getPosition());
            blocks.push_back(newBlock);

            score++;
            scoreText.setString("Score: " + std::to_string(score));

            sf::Vector2f fruitPosition(rand() % (windowWidth - 20), rand() % (windowHeight - 20));
            fruit.setPosition(fruitPosition);
        }

        if (timeSinceLastMove >= moveInterval) {
            handleInput();
            moveSnake();
            timeSinceLastMove = 0.f;
        }

        checkCollision();

        float deltaTime = clock.restart().asSeconds();
        timeSinceLastMove += deltaTime;
    }

    void render() {
        window.clear();
        window.draw(background);
        window.draw(fruit);

        for (const auto& block : blocks) {
            window.draw(block);
        }

        window.draw(scoreText);
        window.display();
    }

    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !isRight) {
            isLeft = true;
            isRight = false;
            isUp = false;
            isDown = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !isLeft) {
            isLeft = false;
            isRight = true;
            isUp = false;
            isDown = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isDown) {
            isLeft = false;
            isRight = false;
            isUp = true;
            isDown = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isUp) {
            isLeft = false;
            isRight = false;
            isUp = false;
            isDown = true;
        }
    }

    void moveSnake() {
        if (isLeft) {
            for (int i = blocks.size() - 1; i > 0; i--) {
                blocks[i].setPosition(blocks[i - 1].getPosition());
            }
            blocks[0].move(-20.f, 0.f);
        }
        else if (isRight) {
            for (int i = blocks.size() - 1; i > 0; i--) {
                blocks[i].setPosition(blocks[i - 1].getPosition());
            }
            blocks[0].move(20.f, 0.f);
        }
        else if (isUp) {
            for (int i = blocks.size() - 1; i > 0; i--) {
                blocks[i].setPosition(blocks[i - 1].getPosition());
            }
            blocks[0].move(0.f, -20.f);
        }
        else if (isDown) {
            for (int i = blocks.size() - 1; i > 0; i--) {
                blocks[i].setPosition(blocks[i - 1].getPosition());
            }
            blocks[0].move(0.f, 20.f);
        }
    }

    void checkCollision() {
        for (int i = 2; i < blocks.size(); i++) {
            if (blocks[0].getGlobalBounds().intersects(fruit.getGlobalBounds())) {
                continue; // skip collision check for fruit
            }
            if (blocks[0].getGlobalBounds().intersects(blocks[i].getGlobalBounds())) {
                endGame();
            }
        }

        if (blocks[0].getPosition().x < 0 || blocks[0].getPosition().x > windowWidth - 20 ||
            blocks[0].getPosition().y < 0 || blocks[0].getPosition().y > windowHeight - 20) {
            endGame();
        }
    }

    void endGame() {
        sf::Text gameOverText("Game Over!!", font, 90);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(140, 200);

        scoreText.setPosition(200, 360);
        scoreText.setString(" Final Score: " + std::to_string(score));
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setCharacterSize(50);
        scoreText.setFillColor(sf::Color::Yellow);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.close();
                }
            }

            sf::Texture backgroundTexture;
            if (!backgroundTexture.loadFromFile("F:/CLionProjects/SnakeGameSFML/snakedead.png")) {
                std::cout << "Error loading background" << std::endl;
            }

            window.clear();
            sf::Sprite background(backgroundTexture);
            background.setScale(1.2, 1.0);
            window.draw(background);
            window.draw(scoreText);
            window.draw(gameOverText);
            window.display();
        }
    }
};

int main() {
    Game game(800, 600);
    game.run();

    return 0;
}
