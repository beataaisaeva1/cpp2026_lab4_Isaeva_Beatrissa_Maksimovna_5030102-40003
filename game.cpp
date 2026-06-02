#include "game.h"
#include <cmath>
#include <string>
#include <iostream> 

Game::Game() : window(sf::VideoMode(800, 600), "Arkanoid"), rng(std::random_device{}()) {
    window.setFramerateLimit(60);

    score = 0;
    defeats = 0;
    shieldActive = false;
    stickyActive = false;
    gameStarted = false;

    balls.push_back(Ball(400.f, 400.f));

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 10; x++) {
            std::uniform_int_distribution<int> distType(0, 2);
            int randomType = distType(rng);

            BlockType type;
            int hp;

            if (randomType == 0) {
                type = UNBREAKABLE;
                hp = 999;
            }
            else if (randomType == 1) {
                type = BONUS;
                hp = 1;
            }
            else {            
                type = HEALTH;
                hp = 2;
            }
            blocks.push_back(Block(60.f + x * 68.f, 50.f + y * 32.f, type, hp));
        }
    }
}

void Game::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {            
                window.close();
            }
        }

        carriage.update();

        if (carriage.shape.getPosition().x < 0) {        
            carriage.shape.setPosition(0.f, carriage.shape.getPosition().y);
        }
        if (carriage.shape.getPosition().x + carriage.shape.getSize().x > 800) { 
            carriage.shape.setPosition(800.f - carriage.shape.getSize().x, carriage.shape.getPosition().y);
        }
        for (auto& ball : balls) {        
            if (!gameStarted) {
                ball.velocity = { 0.f, 0.f };
                ball.shape.setPosition(carriage.shape.getPosition().x + carriage.shape.getSize().x / 2.f - ball.shape.getRadius(), carriage.shape.getPosition().y - ball.shape.getRadius() * 2.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {                
                    gameStarted = true;
                    ball.velocity = { 3.f, -3.f };
                }
                continue;
            }

            ball.update();

            if (ball.shape.getPosition().x <= 0) {            
                ball.shape.setPosition(0.f, ball.shape.getPosition().y);
                ball.velocity.x = std::abs(ball.velocity.x);
            }
            if (ball.shape.getPosition().x + ball.shape.getRadius() * 2 >= 800) {            
                ball.shape.setPosition(800.f - ball.shape.getRadius() * 2, ball.shape.getPosition().y);
                ball.velocity.x = -std::abs(ball.velocity.x);
            }
            if (ball.shape.getPosition().y <= 0) {         
                ball.shape.setPosition(ball.shape.getPosition().x, 0.f);
                ball.velocity.y = std::abs(ball.velocity.y);
            }

            if (ball.shape.getGlobalBounds().intersects(carriage.shape.getGlobalBounds())) {
                if (stickyActive) {
                    ball.velocity = { 0.f, 0.f };
                    ball.shape.setPosition(carriage.shape.getPosition().x + carriage.shape.getSize().x / 2.f - ball.shape.getRadius(), carriage.shape.getPosition().y - ball.shape.getRadius() * 2.f);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {                    
                        ball.velocity = { 3.f, -3.f };
                        stickyActive = false;
                    }
                }
                else {
                    ball.velocity.y = -std::abs(ball.velocity.y);
                }
            }

            for (auto& block : blocks) {
                if (!block.alive) {
                    continue;
                }

                if (ball.shape.getGlobalBounds().intersects(block.shape.getGlobalBounds())) {
                    ball.velocity.y *= -1.f;
                    if (ball.velocity.y > 0) {
                        ball.shape.move(0.f, 5.f);
                    }
                    else {
                        ball.shape.move(0.f, -5.f);
                    }

                    if (block.type == UNBREAKABLE) {
                        break;
                    }
                    block.hp--;
                    score++;
                    std::cout << "Score: " << score << "(+1)" << std::endl;

                    if (block.type == HEALTH) {
                        if (block.hp == 1) {
                            block.shape.setFillColor(sf::Color::Red);
                        }
                    }
                    if (block.hp <= 0) {
                        block.alive = false;

                        if (block.type == BONUS) {
                            std::uniform_int_distribution<int> distBonus(0, 4);
                            int randomBonus = distBonus(rng);
                            BonusType bonusType;

                            switch (randomBonus) {
                            case 0:
                                bonusType = BIG_CARRIAGE;
                                break;
                            case 1:
                                bonusType = SLOW_BALL;
                                break;
                            case 2:
                                bonusType = STICKY_BALL;
                                break;
                            case 3:
                                bonusType = BOTTOM_SHIELD;
                                break;
                            default:
                                bonusType = EXTRA_BALL;
                                break;
                            }

                            bonuses.push_back(Bonus(block.shape.getPosition().x + 20.f, block.shape.getPosition().y, bonusType));
                        }
                    }
                }
            }

            if (ball.shape.getPosition().y > 600) {
                if (shieldActive) {
                    shieldActive = false;
                    ball.shape.setPosition(ball.shape.getPosition().x, 560.f);
                    ball.velocity.y = -3.f;
                }
                else {
                    defeats++;
                    score -= 5;
                    std::cout << "Score: " << "(-5)" << std::endl;
                    ball.shape.setPosition(400.f, 400.f);
                    ball.velocity = { 3.f, -3.f };                     
                }
            }
        }

        for (auto& bonus : bonuses) {
            if (!bonus.active) {
                continue;
            }

            bonus.update();

            if (bonus.shape.getPosition().y > 600) {
                bonus.active = false;
            }

            if (bonus.shape.getGlobalBounds().intersects(carriage.shape.getGlobalBounds())) {
                bonus.active = false;
                switch (bonus.type) {
                case BIG_CARRIAGE:
                    carriage.shape.setSize({ 180.f, 20.f });
                    break;

                case SLOW_BALL:
                    for (auto& ball : balls) {
                        if (ball.velocity.x > 0) {
                            ball.velocity.x = 2.f;
                        }
                        else {
                            ball.velocity.x = -2.f;
                        }
                        if (ball.velocity.y > 0) {
                            ball.velocity.y = 2.f;
                        }
                        else {
                            ball.velocity.y = -2.f;
                        }
                    }
                    break;

                case STICKY_BALL:
                    for (auto& ball : balls) {
                        ball.velocity = { 3.f, -3.f };
                    }      
                    break;

                case BOTTOM_SHIELD:
                    shieldActive = true;
                    break;

                case EXTRA_BALL:
                    if (!balls.empty() && balls.size() < 2) {
                        Ball newBall(balls[0].shape.getPosition().x, balls[0].shape.getPosition().y);
                        newBall.velocity = { -3.f, -3.f };
                        balls.push_back(newBall);
                    }
                    break;
                }
            }
        }       
        window.clear();
        window.draw(carriage.shape);       

        for (auto& ball : balls) {
            window.draw(ball.shape);
        }

        for (auto& block : blocks) {
            if (block.alive) {
                window.draw(block.shape);
            }
        }

        for (auto& bonus : bonuses) {
            if (bonus.active) {
                window.draw(bonus.shape);
            }
        }
        window.display();
    }
}

