#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include <iostream>
#include <vector>



int main()
{
    int pixelwidth = 800;
    int pixelheight = 600;
    sf::RenderWindow window(sf::VideoMode(pixelwidth, pixelheight), "SFML Snake Game");
    window.setFramerateLimit(20);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    std::vector<sf::RectangleShape> blocks;
    sf::RectangleShape block;
    block.setSize(sf::Vector2f(20.f, 20.f));
    block.setFillColor(sf::Color(	75,0,130));
    block.setPosition(sf::Vector2f(200.f, 200.f));
    blocks.push_back(block);

    sf::CircleShape fruit;
    srand(time(NULL));
    sf::Vector2f fruitPosition(100, 100);
    fruit.setRadius(10.f);
    fruit.setFillColor(sf::Color(	255, 255, 0));
    fruit.setPosition(fruitPosition);

    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("F:/CLionProjects/SnakeGameSFML/eating.wav"))
        return -1;



    sf::Sound fruitsound;
    fruitsound.setBuffer(buffer);

    sf::RectangleShape background;
    background.setSize(sf::Vector2f(800,600));
    background.setFillColor(sf::Color(136,139,141));

    // Load the font
    sf::Font font;
    if (!font.loadFromFile("F:/CLionProjects/SnakeGameSFML/Lindelof-Font.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }

    // Create the text object for the score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: 0");
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(350, 10);

    int score = 0;

    bool isleft = false;
    bool isright = false;
    bool isup = false;
    bool isdown = false;



    sf::Clock clock;
    const float moveInterval = 0.1f; // The interval in seconds between moves
    float timeSinceLastMove = 0.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();


        if (fruit.getGlobalBounds().intersects(blocks[0].getGlobalBounds()))
        {
            fruitsound.play();
            // if snake collides with fruit, increase size of snake
            sf::RectangleShape newBlock(sf::Vector2f(20.f, 20.f));
            newBlock.setFillColor(sf::Color(255,203,164));
            newBlock.setPosition(blocks.back().getPosition().x , blocks.back().getPosition().y);
            blocks.push_back(newBlock);

            score++;
            scoreText.setString("Score: " + std::to_string(score));
            // generate new random position for the fruit
            fruitPosition.x = rand() % (800 - 20) + 1;
            fruitPosition.y = rand() % (600 - 20) + 1;
            fruit.setPosition(fruitPosition);
        }

        if (timeSinceLastMove >= moveInterval)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !isright)
            {
                isleft = true;
                isright = false;
                isup = false;
                isdown = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                    !isleft)
            {
                isleft = false;
                isright = true;
                isup = false;
                isdown = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isdown)
            {
                isleft = false;
                isright = false;
                isup = true;
                isdown = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !isup)
            {
                isleft = false;
                isright = false;
                isup = false;
                isdown = true;
            }

            if (isleft)
            {
                for (int i = blocks.size() - 1; i > 0; i--)
                {
                    blocks[i].setPosition(blocks[i - 1].getPosition());
                }
                blocks[0].move(-20.f, 0.f);
            }
            else if (isright)
            {
                for (int i = blocks.size() - 1; i > 0; i--)
                {
                    blocks[i].setPosition(blocks[i - 1].getPosition());
                }
                blocks[0].move(20.f, 0.f);
            }
            else if (isup)
            {
                for (int i = blocks.size() - 1; i > 0; i--)
                {
                    blocks[i].setPosition(blocks[i - 1].getPosition());
                }
                blocks[0].move(0.f, -20.f);
            }
            else if (isdown)
            {
                for (int i = blocks.size() - 1; i > 0; i--)
                {
                    blocks[i].setPosition(blocks[i - 1].getPosition());
                }
                blocks[0].move(0.f, 20.f);
            }

            timeSinceLastMove = 0.f;
        }



         //loop for checking collision of snake with itself
        for (int i = 2; i < blocks.size(); i++) {
            if (blocks[0].getGlobalBounds().intersects(fruit.getGlobalBounds())) {
                continue; // skip collision check for fruit
            }
            if (blocks[0].getGlobalBounds().intersects(blocks[i].getGlobalBounds())) {
                if (i != 1) { // snake head collided with body segment
                   while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !event.type == sf::Event::Closed)
                   {
                            sf::Text gameOverText("Game Over!!",font,90);
                            gameOverText.setFillColor(sf::Color::Red);
                            gameOverText.setPosition(140,200);
                            scoreText.setPosition(200,360);
                            scoreText.setStyle(sf::Text::Bold);
                            scoreText.setString(" Final Score: " + std::to_string(score));
                            scoreText.setCharacterSize(50);
                            scoreText.setFillColor(sf::Color::Yellow);
                       while (window.isOpen()) {
                           while (window.pollEvent(event)) {
                               if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                                   window.close();
                           }
                           if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                               window.close();
                           sf::Texture backgroundTexture;
                           if (!backgroundTexture.loadFromFile("F:/CLionProjects/SnakeGameSFML/snakedead.png"))
                           {
                               std::cout << "Error loading background" << std::endl;
                           }
                           window.clear();
                           sf::Sprite background(backgroundTexture);
                           background.setScale(1.2,1.0);
                           window.draw(background);
                           window.draw(scoreText);
                           window.draw(gameOverText);
                           window.display();
                       }
                   }
                }
            }
        }

        if(blocks[0].getPosition().x <0 || blocks[0].getPosition().x > pixelwidth - 20 || blocks[0].getPosition().y < 0 ||
                blocks[0].getPosition().y > pixelheight - 20)
        {

            while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !event.type == sf::Event::Closed)
            {
                sf::Text gameOverText("Game Over!!",font,90);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setPosition(140,200);
                scoreText.setPosition(200,360);
                scoreText.setString(" Final Score: " + std::to_string(score));
                scoreText.setStyle(sf::Text::Bold);
                scoreText.setCharacterSize(50);
                scoreText.setFillColor(sf::Color::Yellow);
                while (window.isOpen()) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                            window.close();
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                        window.close();
                    sf::Texture backgroundTexture;
                    if (!backgroundTexture.loadFromFile("F:/CLionProjects/SnakeGameSFML/snakedead.png"))
                    {
                        std::cout << "Error loading background" << std::endl;
                    }
                    window.clear(sf::Color(230, 230, 230));
                    sf::Sprite background(backgroundTexture);
                   background.setScale(1.2,1.0);
                    window.draw(background);
                    window.draw(scoreText);
                    window.draw(gameOverText);
                    window.display();
                }
            }
        }


        window.clear();
        window.draw(background);
        window.draw(fruit);

        for (int i = 0; i < blocks.size(); i++) {
            sf::RectangleShape copyblock = blocks[i];
            window.draw(copyblock);
        }

        window.draw(scoreText);
        window.display();

        float deltaTime = clock.restart().asSeconds();
        timeSinceLastMove += deltaTime;
    }

    return 0;
}
