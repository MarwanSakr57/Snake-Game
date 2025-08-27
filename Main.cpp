#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

void snake_move(std::vector<sf::RectangleShape> &snake, std::vector<std::string> &segmentDirections, const std::string &direction, float cellSize) {
    float moveAmount = cellSize;
    sf::Vector2f pos = snake[0].getPosition();

    for (int i = segmentDirections.size() - 1; i > 0; --i) {
        segmentDirections[i] = segmentDirections[i - 1];
    }
    segmentDirections[0] = direction;

    if (direction == "right") pos.x += moveAmount;
    else if (direction == "left") pos.x -= moveAmount;
    else if (direction == "up") pos.y -= moveAmount;
    else if (direction == "down") pos.y += moveAmount;

    for (int i = snake.size() - 1; i > 0; i--) {
        snake[i].setPosition(snake[i - 1].getPosition());
    }
    snake[0].setPosition(pos);
}

bool checkFruitCollision(std::vector<sf::RectangleShape>& snake, sf::CircleShape& fruit){
    return snake[0].getGlobalBounds().findIntersection(fruit.getGlobalBounds()).has_value();
}

void growSnake(std::vector<sf::RectangleShape>& snake, std::vector<std::string>& segmentDirections, float cellSize) {
    sf::RectangleShape part({cellSize, cellSize});
    part.setOrigin(sf::Vector2f(part.getSize().x / 2, part.getSize().y / 2));
    part.setPosition(snake.back().getPosition());
    snake.push_back(part);
    segmentDirections.push_back(segmentDirections.back());
}

void spawnFruit(sf::CircleShape& fruit, const std::vector<sf::RectangleShape>& snake, float cellSize){
    int gridWidth  = 800 / cellSize;
    int gridHeight = 600 / cellSize;

    bool valid = false;
    sf::Vector2f fruitPos;
    while(!valid){
        int x = rand() % gridWidth;
        int y = rand() % gridHeight;
        valid = true;
        fruitPos = sf::Vector2f(x * cellSize + cellSize/2, y * cellSize + cellSize/2);

        for(int i=0;i<snake.size();i++){
            if(snake[i].getPosition()==fruitPos){
                valid=false; 
                break;
            }
        }
    }
    fruit.setPosition(fruitPos);
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "snake game", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/apple.png")) {
        std::cerr << "Failed to load apple.png\n"; return -1; 
    }
    sf::Texture head_up, head_down, head_left, head_right;
    sf::Texture tail_up, tail_down, tail_left, tail_right;
    sf::Texture body_horizontal, body_vertical;
    sf::Texture body_topleft, body_topright, body_bottomleft, body_bottomright;

    if (!head_up.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/head_up.png")) {
        std::cerr << "Failed to load head_up.png\n"; return -1;
    }
    if (!head_down.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/head_down.png")) {
        std::cerr << "Failed to load head_down.png\n"; return -1;
    }
    if (!head_left.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/head_left.png")) {
        std::cerr << "Failed to load head_left.png\n"; return -1;
    }
    if (!head_right.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/head_right.png")) {
        std::cerr << "Failed to load head_right.png\n"; return -1;
    }
    if (!tail_up.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/tail_up.png")) {
        std::cerr << "Failed to load tail_up.png\n"; return -1;
    }
    if (!tail_down.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/tail_down.png")) {
        std::cerr << "Failed to load tail_down.png\n"; return -1;
    }
    if (!tail_left.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/tail_left.png")) {
        std::cerr << "Failed to load tail_left.png\n"; return -1;
    }
    if (!tail_right.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/tail_right.png")) {
        std::cerr << "Failed to load tail_right.png\n"; return -1;
    }
    if (!body_horizontal.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_horizontal.png")) {
        std::cerr << "Failed to load body_horizontal.png\n"; return -1;
    }
    if (!body_vertical.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_vertical.png")) {
        std::cerr << "Failed to load body_vertical.png\n"; return -1;
    }
    if (!body_topleft.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_topleft.png")) {
        std::cerr << "Failed to load body_topleft.png\n"; return -1;
    }
    if (!body_topright.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_topright.png")) {
        std::cerr << "Failed to load body_topright.png\n"; return -1;
    }
    if (!body_bottomleft.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_bottomleft.png")) {
        std::cerr << "Failed to load body_bottomleft.png\n"; return -1;
    }
    if (!body_bottomright.loadFromFile("I:/Game dev/C++ Snake/Snake-Game/Assets/body_bottomright.png")) {
        std::cerr << "Failed to load body_bottomright.png\n"; return -1;
    }

    float cellSize = 40;

    std::vector<sf::RectangleShape> snake;
    sf::RectangleShape head({cellSize, cellSize});
    head.setTexture(&head_right);
    head.setPosition({cellSize/2, cellSize/2});
    head.setOrigin(sf::Vector2f(head.getSize().x / 2, head.getSize().y / 2));
    snake.push_back(head);

    std::vector<std::string> segmentDirections;
    segmentDirections.push_back("right");

    appleTexture.setSmooth(true);

    sf::CircleShape fruit;
    fruit.setRadius(cellSize / 2);
    fruit.setOrigin(sf::Vector2f(cellSize / 2, cellSize / 2));
    fruit.setTexture(&appleTexture);
    fruit.setPosition({5*cellSize + cellSize/2, 5*cellSize + cellSize/2});

    std::string direction = "right";

    sf::Clock clock;
    sf::Time moveTick = sf::seconds(0.5f);
    sf::Time elapsed;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && direction != "right")
                    direction = "left";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && direction != "left")
                    direction = "right";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && direction != "down")
                    direction = "up";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && direction != "up")
                    direction = "down";
            }
        }
        elapsed += clock.restart();
        if (elapsed >= moveTick) {
            snake_move(snake, segmentDirections, direction, cellSize);
            if (checkFruitCollision(snake, fruit)) {
                growSnake(snake, segmentDirections, cellSize);
                spawnFruit(fruit, snake, cellSize);
            }
            elapsed -= moveTick;
        }
        for (size_t i = 0; i < snake.size(); ++i) {
            if (i == 0) {
                if (segmentDirections[0] == "up") snake[0].setTexture(&head_up);
                else if (segmentDirections[0] == "down") snake[0].setTexture(&head_down);
                else if (segmentDirections[0] == "left") snake[0].setTexture(&head_left);
                else if (segmentDirections[0] == "right") snake[0].setTexture(&head_right);
            }else if (i == snake.size() - 1) {
                std::string prev = segmentDirections[i - 1];
                std::string tailDir = segmentDirections[i];

                if ((prev == "up" && tailDir == "left") || (prev == "right" && tailDir == "down"))
                    snake[i].setTexture(&body_topright);
                else if ((prev == "up" && tailDir == "right") || (prev == "left" && tailDir == "down"))
                    snake[i].setTexture(&body_topleft);
                else if ((prev == "down" && tailDir == "left") || (prev == "right" && tailDir == "up"))
                    snake[i].setTexture(&body_bottomright);
                else if ((prev == "down" && tailDir == "right") || (prev == "left" && tailDir == "up"))
                    snake[i].setTexture(&body_bottomleft);
                else if (tailDir == "left")
                    snake[i].setTexture(&tail_left);
                else if (tailDir == "right")
                    snake[i].setTexture(&tail_right);
                else if (tailDir == "up")
                    snake[i].setTexture(&tail_up);
                else if (tailDir == "down")
                    snake[i].setTexture(&tail_down);
                else
                    snake[i].setTexture(&tail_right);
            } else {
                std::string prev = segmentDirections[i - 1];
                std::string next = segmentDirections[i];

                if ((prev == "left" && next == "left") || (prev == "right" && next == "right"))
                    snake[i].setTexture(&body_horizontal);
                else if ((prev == "up" && next == "up") || (prev == "down" && next == "down"))
                    snake[i].setTexture(&body_vertical);
                else if ((prev == "up" && next == "left") || (prev == "right" && next == "down"))
                    snake[i].setTexture(&body_topright);
                else if ((prev == "up" && next == "right") || (prev == "left" && next == "down"))
                    snake[i].setTexture(&body_topleft);
                else if ((prev == "down" && next == "left") || (prev == "right" && next == "up"))
                    snake[i].setTexture(&body_bottomright);
                else if ((prev == "down" && next == "right") || (prev == "left" && next == "up"))
                    snake[i].setTexture(&body_bottomleft);
                else
                    snake[i].setTexture(&body_horizontal);
            }
        }

        window.clear(sf::Color::Green);
        window.draw(fruit);
        for (auto& part : snake) {
            window.draw(part);
        }
        window.display();
    }
    return 0;
}