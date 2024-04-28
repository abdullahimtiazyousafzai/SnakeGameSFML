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
    sf::Font font;
    sf::Text scoreText;
    sf::SoundBuffer buffer;
    sf::Sound fruitSound;
    std::vector<sf::Sprite> blocks;
    sf::SoundBuffer deathsound;
    sf::Sound deathSound;
    sf::Texture head;
    sf::Texture body;
    sf::Texture fruitTexture;
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Sprite headSprite;
    sf::Sprite fruit;

    int score;
    char direction = 'n';

    sf::Clock clock;
    const float moveInterval = 0.1f;
    float timeSinceLastMove = 0.f;

    void setup() {
        if(!backgroundTexture.loadFromFile("./assets/images/kita.png")){
            std::cout << "Error loading background texture" << std::endl;
        }
        background.setTexture(backgroundTexture);
        //load the font
        if (!font.loadFromFile("./assets/fonts/Lindelof-Font.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }

        //load the textures
        if (!head.loadFromFile("./assets/images/snakehead2.png")) {
            std::cout << "Error loading head texture" << std::endl;
        }
        if (!body.loadFromFile("./assets/images/snakebody2.png")) {
            std::cout << "Error loading body texture" << std::endl;
        }
        if (!fruitTexture.loadFromFile("./assets/images/apple.png")) {
            std::cout << "Error loading fruit texture" << std::endl;
        }



        //set up the score text
        scoreText.setFont(font);
        scoreText.setString("Score: 0");
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(350, 10);

        if (!buffer.loadFromFile("./assets/sounds/eating.wav")) {
            std::cerr << "Error loading voice!" << std::endl;
        }

        fruitSound.setBuffer(buffer);

        headSprite.setTexture(head);
        headSprite.setScale(0.1f, 0.1f);
        headSprite.setPosition(windowWidth / 2, windowHeight / 2);
        blocks.push_back(headSprite);

        fruit.setTexture(fruitTexture);
        sf::Vector2f fruitPosition(rand() % (windowWidth - 20), rand() % (windowHeight - 20));
        fruit.setPosition(fruitPosition);
        fruit.setScale(0.8f, 0.8f);

        score = 0;
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }
    }
    void addBodySegment() {
        sf::Sprite bodySprite;
        bodySprite.setTexture(body);
        bodySprite.setScale(0.1f, 0.1f);

        // Position the new body segment based on the position and rotation of the last body segment
        sf::Sprite& lastSegment = blocks.back();
        bodySprite.setPosition(lastSegment.getPosition());

        blocks.push_back(bodySprite);
    }


    void update() {
        if (fruit.getGlobalBounds().intersects(blocks[0].getGlobalBounds())) {
            fruitSound.play();

            addBodySegment();

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != 'r') {
            direction = 'l';
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != 'l') {
            direction = 'r';
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != 'd') {
            direction = 'u';
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != 'u') {
            direction = 'd';
        }
    }

    void moveSnake() {
        sf::Vector2f movement;

        switch (direction) {
            case 'l':
                movement.x -= 20;
                break;
            case 'r':
                movement.x += 20;
                break;
            case 'u':
                movement.y -= 20;
                break;
            case 'd':
                movement.y += 20;
                break;
        }

        for (int i = blocks.size() - 1; i > 0; i--) {
            blocks[i].setPosition(blocks[i - 1].getPosition());
        }

        blocks[0].move(movement);
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
        scoreText.setFillColor(sf::Color::Cyan);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.close();
                }
            }

            sf::Texture backgroundTexture;
            if (!backgroundTexture.loadFromFile("assets/images/snakedeath.jpeg")) {
                std::cout << "Error loading background" << std::endl;
            }

            window.clear();
            sf::Sprite background(backgroundTexture);
            background.setScale(0.8, 0.7);
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
