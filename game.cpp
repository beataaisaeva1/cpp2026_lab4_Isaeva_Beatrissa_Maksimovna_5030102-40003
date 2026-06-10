#include "Game.h"
#include "Constants.h"
#include "BigCarriageBonus.h"
#include "SlowBallBonus.h"
#include "StickyBallBonus.h"
#include "BottomShieldBonus.h"
#include "ExtraBallBonus.h"
#include <cmath>
#include <iostream> 
#include <algorithm>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid"), rng(std::random_device{}()) {
    window.setFramerateLimit(60);

    score = 0;
    defeats = 0;
    shieldActive = false;
    stickyActive = false;
    gameStarted = false;

    balls.push_back(Ball(400.f, 400.f));

    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 10; x++) {
            std::uniform_int_distribution<int> distType(0, 9);
            int randomType = distType(rng);

            BlockType type;
            int hp;

            if (randomType == 0) {
                type = UNBREAKABLE;
                hp = UNBREAKABLE_HP;
            }
            else if (randomType <= 4) {
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
            if (ball.stuck) {
                ball.shape.setPosition(carriage.shape.getPosition().x + carriage.shape.getSize().x / 2.f - ball.shape.getRadius(), carriage.shape.getPosition().y - ball.shape.getRadius() * 2.f);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    ball.stuck = false;
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
                    ball.stuck = true;
                    ball.velocity = { 0.f, 0.f };
                    ball.shape.setPosition(carriage.shape.getPosition().x + carriage.shape.getSize().x / 2.f - ball.shape.getRadius(), carriage.shape.getPosition().y - ball.shape.getRadius() * 2.f);                
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
                    float ballCenterX = ball.shape.getPosition().x + ball.shape.getRadius();
                    float ballCenterY = ball.shape.getPosition().y + ball.shape.getRadius();

                    float blockCenterX = block.shape.getPosition().x + block.shape.getSize().x / 2.f;
                    float blockCenterY = block.shape.getPosition().y + block.shape.getSize().y / 2.f;

                    float dx = std::abs(ballCenterX - blockCenterX);
                    float dy = std::abs(ballCenterY - blockCenterY);

                    if (dx > dy) {
                        if (ballCenterX < blockCenterX) {
                            ball.shape.setPosition(block.shape.getPosition().x - ball.shape.getRadius() * 2.f, ball.shape.getPosition().y);
                        }
                        else {
                            ball.shape.setPosition(block.shape.getPosition().x + block.shape.getSize().x, ball.shape.getPosition().y);
                        }
                        ball.velocity.x *= -1.f;
                    }
                    else {
                        if (ballCenterY < blockCenterY) {
                            ball.shape.setPosition(ball.shape.getPosition().x, block.shape.getPosition().y - ball.shape.getRadius() * 2.f);
                        }
                        else {
                            ball.shape.setPosition(ball.shape.getPosition().x, block.shape.getPosition().y + block.shape.getSize().y);
                        }
                        ball.velocity.y *= -1.f;
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
                            if (randomBonus == 0) {
                                bonuses.push_back(std::make_unique<BigCarriageBonus>(block.shape.getPosition().x + 20.f, block.shape.getPosition().y));
                            }
                            else if (randomBonus == 1) {
                                bonuses.push_back(std::make_unique<SlowBallBonus>(block.shape.getPosition().x + 20.f, block.shape.getPosition().y));
                            }
                            else if (randomBonus == 2) {
                                bonuses.push_back(std::make_unique<StickyBallBonus>(block.shape.getPosition().x + 20.f, block.shape.getPosition().y));
                            }
                            else if (randomBonus == 3) {
                                bonuses.push_back(std::make_unique<BottomShieldBonus>(block.shape.getPosition().x + 20.f, block.shape.getPosition().y));
                            }
                            else {
                                bonuses.push_back(std::make_unique<ExtraBallBonus>(block.shape.getPosition().x + 20.f, block.shape.getPosition().y));
                            }
                        }
                    }
                }
            }
            if (ball.shape.getPosition().y > WINDOW_HEIGHT) {
                if (shieldActive) {
                    shieldActive = false;
                    ball.shape.setPosition(ball.shape.getPosition().x, SHIELD_BOUNCE_Y);
                    ball.velocity.y = -BALL_SPEED;
                }
                else {
                    if (balls.size() > 1) {
                        ball.alive = false;
                        continue;
                    }
                    else {
                        defeats++;
                        score -= DEFEAT;
                        std::cout << "Score: " << score << "(-5)" << std::endl;

                        stickyActive = false;
                        shieldActive = false;

                        carriage.shape.setSize({CARRIAGE_WIDTH, CARRIAGE_HEIGHT});

                        bonuses.clear();

                        ball.shape.setPosition(BALL_START_X, BALL_START_Y);

                        ball.velocity = {BALL_SPEED, -BALL_SPEED};

                        for (auto& ball : balls) {
                            ball.stuck = false;
                            ball.alive = true;
                        }                    
                        gameStarted = false;
                    }
                }
            }
        }
        balls.erase(std::remove_if(balls.begin(), balls.end(), [](const Ball& ball) { return !ball.alive; }), balls.end());

        for (auto& bonus : bonuses) {
            if (!bonus->active) {
                continue;
            }

            bonus->update();

            if (bonus->shape.getPosition().y > WINDOW_HEIGHT) {
                bonus->active = false;
            }

            if (bonus->shape.getGlobalBounds().intersects(carriage.shape.getGlobalBounds())) {
                bonus->active = false;
                bonus->activate(*this);                
                
            }
        }
        window.clear();

        window.draw(carriage.shape);

        for (auto& block : blocks) {
            if (block.alive) {
                window.draw(block.shape);
            }
        }

        for (auto& ball : balls) {
            window.draw(ball.shape);
        }

        for (auto& bonus : bonuses) {
            if (bonus->active) {
                window.draw(bonus->shape);
            }
        }
        window.display();
    }
}

void Game::makeCarriageBig() {
    carriage.shape.setSize({BIG_CARRIAGE_WIDTH, CARRIAGE_HEIGHT});
}

void Game::slowBalls() {
    for (auto& ball : balls) {
        if (ball.velocity.x > 0) {
            ball.velocity.x = SLOW_BALL_SPEED;
        }
        else {
            ball.velocity.x = -SLOW_BALL_SPEED;
        }
        if (ball.velocity.y > 0) {
            ball.velocity.y = SLOW_BALL_SPEED;
        }
        else {
            ball.velocity.y = -SLOW_BALL_SPEED;
        }
    }
}

void Game::activateStickyBall() {
    stickyActive = true;
}


void Game::activateShield() {
    shieldActive = true;
}

void Game::addExtraBall() {
    if (!balls.empty() && balls.size() < 2) {
        Ball newBall(balls[0].shape.getPosition().x, balls[0].shape.getPosition().y);

        newBall.velocity = {-BALL_SPEED, -BALL_SPEED};
        balls.push_back(newBall);
    }
}


