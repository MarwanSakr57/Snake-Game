#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

// Move snake head depending on direction
void snake_move(std::vector<sf::RectangleShape> &snake, const std::string &direction,float cellSize) {
    float moveAmount = cellSize;
    sf::Vector2f pos = snake[0].getPosition();

    if (direction == "right") {
        snake[0].setRotation(sf::degrees(0));
        pos.x += moveAmount;
    } else if (direction == "left") {
        snake[0].setRotation(sf::degrees(180));
        pos.x -= moveAmount;
    } else if (direction == "up") {
        snake[0].setRotation(sf::degrees(-90));
        pos.y -= moveAmount;
    } else if (direction == "down") {
        snake[0].setRotation(sf::degrees(90));
        pos.y += moveAmount;
    }
    for(int i=snake.size()-1;i>0;i--){
    snake[i].setPosition(snake[i-1].getPosition());
    //snake[i].setRotation(snake[i-1].getRotation());
    }
    snake[0].setPosition(pos);
}
bool checkFruitCollision(std::vector<sf::RectangleShape>& snake, sf::CircleShape& fruit){
    return snake[0].getGlobalBounds().findIntersection(fruit.getGlobalBounds()).has_value();
}

void growSnake(std::vector<sf::RectangleShape>& snake,sf::Texture& Texture,float cellSize){
    sf::RectangleShape part({cellSize,cellSize});
    part.setTexture(&Texture);
    part.setOrigin(sf::Vector2f(part.getSize().x / 2, part.getSize().y / 2));
    part.setPosition(snake[snake.size()-1].getPosition());
    
    snake.push_back(part);
}
void spawnFruit(sf::CircleShape& fruit, const std::vector<sf::RectangleShape>& snake, float cellSize){
    int gridWidth  = 800 / cellSize;
    int gridHeight = 600 / cellSize;

    bool valid =false;
    sf::Vector2f fruitPos;
    while(!valid){
    int x = rand() % gridWidth;
    int y = rand() % gridHeight;
    valid =true;
    fruitPos=sf::Vector2f(x * cellSize, y * cellSize);

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

    sf::Texture bodyTexture;
    if (!bodyTexture.loadFromFile("I:/Game dev/C++ Snake/snake_body.png")) {
        return -1; 
    }
    sf::Texture snakeTexture;
    if (!snakeTexture.loadFromFile("I:/Game dev/C++ Snake/head_right.png")) {
        return -1; 
    }
    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("I:/Game dev/C++ Snake/apple.png")) {
        return -1; 
    }


    float cellSize=40;
    snakeTexture.setSmooth(true);

    std::vector<sf::RectangleShape> snake;
    sf::RectangleShape head({cellSize,cellSize});
    head.setTexture(&snakeTexture);
    head.setPosition({cellSize/2, cellSize/2});
    head.setOrigin(sf::Vector2f(head.getSize().x / 2, head.getSize().y / 2));
    snake.push_back(head);

    appleTexture.setSmooth(true);

    sf::CircleShape fruit;
    fruit.setRadius(cellSize / 2);
    fruit.setTexture(&appleTexture);
    fruit.setPosition({5*cellSize, 5*cellSize});

    std::string direction = "right"; 

    sf::Clock clock;
    sf::Time moveTick=sf::seconds(1);
    sf::Time elapsed;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    direction = "left";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    direction = "right";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    direction = "up";
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    direction = "down";
            }
        }
        elapsed+=clock.restart();
        if(elapsed>=moveTick){
        snake_move(snake, direction,cellSize);
        if(checkFruitCollision(snake,fruit)){
            growSnake(snake,bodyTexture,cellSize);
            spawnFruit(fruit,snake,cellSize);
           
        }
        std::cout << "Fruit: (" << fruit.getPosition().x << ", " << fruit.getPosition().y << ") , "
          << "Head: (" << snake[0].getPosition().x << ", " << snake[0].getPosition().y << ")" << std::endl;
        elapsed-=moveTick;
        }
        window.clear(sf::Color::White);
        window.draw(fruit);
        for (auto& part : snake) {
        window.draw(part);
        }
        window.display();
    }
    return 0;
}
