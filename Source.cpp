#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <ctime> 


enum class GameState {
    StartScreen,
    LevelSelect,
    Playing,
    GameOver
};

// Constants for map sizes and number of mines for each difficulty level
GameState gameState;
const int EASY_SIZE_X = 9;
const int EASY_SIZE_Y = 9;
const int EASY_MINES = 10;
const int MEDIUM_SIZE_X = 16;
const int MEDIUM_SIZE_Y = 16;
const int MEDIUM_MINES = 40;
const int HARD_SIZE_X = 30;
const int HARD_SIZE_Y = 16;
const int HARD_MINES = 99;
int CUSTOM_SIZE_X;
int CUSTOM_SIZE_Y;
int CUSTOM_MINE;

int window_size_x = 800;
int window_size_y = 600;

int square_size;
int map_size_x;
int map_size_y;
int mine_num;
int map[30][30];
int map_view[30][30];

sf::RenderWindow app;
sf::Texture mine;
sf::Texture flag;
sf::Font font;

// used so that when a non-mine tile is clicked it will check around to see if it is a number or empty tile
int surroundings[][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

int random(int min, int max) {
    static bool first = true;
    if (first) {
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

void generate_map(int size_x, int size_y, int num_mines) {
    map_size_x = size_x;
    map_size_y = size_y;
    mine_num = num_mines;
    square_size = 60; // Set a fixed square size for now

    // Calculate window size based on grid size and square size
    window_size_x = map_size_x * square_size;
    window_size_y = map_size_y * square_size;

    //map[][] = 0 is not a mine and -1 is a mine
    for (int y = 0; y < map_size_y; y++) {
        for (int x = 0; x < map_size_x; x++) {
            map[y][x] = 0; 
            map_view[y][x] = 10; // Initialize all cells as hidden
        }
    }

    //random mine placement
    int mines_placed = 0;
    while (mines_placed < mine_num) {
        int x = rand() % map_size_x;
        int y = rand() % map_size_y;
        if (map[y][x] != -1) {
            map[y][x] = -1; // Place mine
            mines_placed++;
        }
    }

    //number tile creation
    for (int y = 0; y < map_size_y; y++) {
        for (int x = 0; x < map_size_x; x++) {
            if (map[y][x] != -1) {
                int n = 0;
                for (auto i : surroundings) {
                    if (x - i[0] >= 0 && x - i[0] < map_size_x && y - i[1] >= 0 && y - i[1] < map_size_y && map[y - i[1]][x - i[0]] == -1) {
                        n++;
                    }
                }
                map[y][x] = n;
            }
        }
    }

    // Adjust window size to fit the grid exactly
    app.setSize(sf::Vector2u(window_size_x, window_size_y));
    app.setView(sf::View(sf::FloatRect(0, 0, window_size_x, window_size_y)));
}

void start_screen() {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text startText;
    startText.setFont(font);
    startText.setString("Click to Start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(200, 200);

    app.clear(sf::Color::Black);
    app.draw(startText);
    app.display();

    // Wait for user input to start the game
    sf::Event event;
    while (app.isOpen()) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    gameState = GameState::LevelSelect;
                    return;
                }
            }
        }
    }
}

void level_select_screen() {
    sf::Text text;
    text.setFont(font);
    text.setString("Select Difficulty:");
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition(50, 50);

    // Text input for pre-set difficulties
    sf::Text easyText;
    easyText.setFont(font);
    easyText.setString("Easy");
    easyText.setCharacterSize(20);
    easyText.setFillColor(sf::Color::Red);
    easyText.setPosition(50, 100);

    sf::Text mediumText;
    mediumText.setFont(font);
    mediumText.setString("Medium");
    mediumText.setCharacterSize(20);
    mediumText.setFillColor(sf::Color::Red);
    mediumText.setPosition(50, 150);

    sf::Text hardText;
    hardText.setFont(font);
    hardText.setString("Hard");
    hardText.setCharacterSize(20);
    hardText.setFillColor(sf::Color::Red);
    hardText.setPosition(50, 200);

    sf::Text customText;
    customText.setFont(font);
    customText.setString("Custom:");
    customText.setCharacterSize(20);
    customText.setFillColor(sf::Color::Blue);
    customText.setPosition(50, 270);

    // Text input fields for custom parameters
    sf::Text customXSizeText;
    customXSizeText.setFont(font);
    customXSizeText.setString("Enter Custom Size X:");
    customXSizeText.setCharacterSize(17);
    customXSizeText.setFillColor(sf::Color::Blue);
    customXSizeText.setPosition(50, 300);

    sf::Text customYSizeText;
    customYSizeText.setFont(font);
    customYSizeText.setString("Enter Custom Size Y:");
    customYSizeText.setCharacterSize(17);
    customYSizeText.setFillColor(sf::Color::Blue);
    customYSizeText.setPosition(50, 320);

    sf::Text customMineText;
    customMineText.setFont(font);
    customMineText.setString("Enter Custom Mine:");
    customMineText.setCharacterSize(17);
    customMineText.setFillColor(sf::Color::Blue);
    customMineText.setPosition(50, 340);

    sf::Text customEnter;
    customEnter.setFont(font);
    customEnter.setString("Enter Custom Game");
    customEnter.setCharacterSize(19);
    customEnter.setFillColor(sf::Color::Blue);
    customEnter.setPosition(50, 365);

    std::string customXInput;
    std::string customYInput;
    std::string customMineInput;
    bool customMode = false;

    while (app.isOpen()) {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.x >= 50 && event.mouseButton.x < 150 && event.mouseButton.y >= 100 && event.mouseButton.y < 130) {
                        // Easy difficulty selected
                        generate_map(EASY_SIZE_X, EASY_SIZE_Y, EASY_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (event.mouseButton.x >= 50 && event.mouseButton.x < 150 && event.mouseButton.y >= 150 && event.mouseButton.y < 180) {
                        // Medium difficulty selected
                        generate_map(MEDIUM_SIZE_X, MEDIUM_SIZE_Y, MEDIUM_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (event.mouseButton.x >= 50 && event.mouseButton.x < 150 && event.mouseButton.y >= 200 && event.mouseButton.y < 230) {
                        // Hard difficulty selected
                        generate_map(HARD_SIZE_X, HARD_SIZE_Y, HARD_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (event.mouseButton.x >= 50 && event.mouseButton.x < 150 && event.mouseButton.y >= 250 && event.mouseButton.y < 280) {
                        // Custom difficulty selected
                        customMode = true;
                    }
                }
            }
            //custom diffculty picked
            else if (event.type == sf::Event::TextEntered && customMode) {
                // Handle text input for custom X size
                if (event.text.unicode >= 48 && event.text.unicode <= 57 && customXInput.size() < 3) {
                    customXInput += static_cast<char>(event.text.unicode);
                }
                // Handle text input for custom Y size
                else if (event.text.unicode >= 48 && event.text.unicode <= 57 && customYInput.size() < 3) {
                    customYInput += static_cast<char>(event.text.unicode);
                }
                // Handle text input for custom mine amount
                else if (event.text.unicode >= 48 && event.text.unicode <= 57 && customMineInput.size() < 3) {
                    customMineInput += static_cast<char>(event.text.unicode);
                }
                // Handle Enter key press
                else if (event.text.unicode == 13) {
                    int customX = std::stoi(customXInput);
                    int customY = std::stoi(customYInput);
                    int customMine = std::stoi(customMineInput);
                    if (customX > 3 && customY > 3 && customMine > 2) {
                        CUSTOM_SIZE_X = customX;
                        CUSTOM_SIZE_Y = customY;
                        CUSTOM_MINE = customMine;
                        generate_map(CUSTOM_SIZE_X, CUSTOM_SIZE_Y, CUSTOM_MINE);
                        gameState = GameState::Playing;
                        return;
                    }
                    else {
                        std::cout << "Invalid input. Size must be greater than 3 and mine count greater than 2." << std::endl;
                    }
                }
            }
        }

        app.clear(sf::Color::Black);
        app.draw(text);
        app.draw(easyText);
        app.draw(mediumText);
        app.draw(hardText);
        app.draw(customText);
        app.draw(customXSizeText);
        app.draw(customYSizeText);
        app.draw(customMineText);

        // Draw text input fields
        sf::Text customXInputTextObj;
        customXInputTextObj.setFont(font);
        customXInputTextObj.setString(customXInput);
        customXInputTextObj.setCharacterSize(20);
        customXInputTextObj.setFillColor(sf::Color::Black);
        customXInputTextObj.setPosition(250, 300);
        app.draw(customXInputTextObj);

        sf::Text customYInputTextObj;
        customYInputTextObj.setFont(font);
        customYInputTextObj.setString(customYInput);
        customYInputTextObj.setCharacterSize(20);
        customYInputTextObj.setFillColor(sf::Color::Black);
        customYInputTextObj.setPosition(250, 350);
        app.draw(customYInputTextObj);

        sf::Text customMineInputTextObj;
        customMineInputTextObj.setFont(font);
        customMineInputTextObj.setString(customMineInput);
        customMineInputTextObj.setCharacterSize(20);
        customMineInputTextObj.setFillColor(sf::Color::Black);
        customMineInputTextObj.setPosition(250, 400);
        app.draw(customMineInputTextObj);

        app.display();
    }
}


bool check_win_condition() {
    for (int y = 0; y < map_size_y; y++) {
        for (int x = 0; x < map_size_x; x++) {
            if (map[y][x] != -1 && map_view[y][x] == 10) {
                return false; // All non-mine cells are not revealed
            }
        }
    }
    return true;
}

void game_over_screen() {
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text gameOverText;
    gameOverText.setFont(font);
    if (check_win_condition()) {
        gameOverText.setString("You Win!");
    }
    else {
        gameOverText.setString("Game Over!");
    }
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);

    // Center the text horizontally
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    gameOverText.setPosition(window_size_x / 2.0f, window_size_y / 2.0f - 50); // Adjust vertically

    sf::Text retryText;
    retryText.setFont(font);
    retryText.setString("Retry");
    retryText.setCharacterSize(30);
    retryText.setFillColor(sf::Color::White);

    // Center the retry text horizontally
    sf::FloatRect retryBounds = retryText.getLocalBounds();
    retryText.setOrigin(retryBounds.left + retryBounds.width / 2.0f,
        retryBounds.top + retryBounds.height / 2.0f);
    retryText.setPosition(window_size_x / 2.0f, window_size_y / 2.0f + 50); // Adjust vertically

    sf::Text startText;
    startText.setFont(font);
    startText.setString("Return to Start Screen");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);

    // Center the start text horizontally
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(startBounds.left + startBounds.width / 2.0f,
        startBounds.top + startBounds.height / 2.0f);
    startText.setPosition(window_size_x / 2.0f, window_size_y / 2.0f + 100); // Adjust vertically

    app.clear(sf::Color::Black);
    app.draw(gameOverText);
    app.draw(retryText);
    app.draw(startText);
    app.display();

    // Wait for user input to retry or return to start screen
    sf::Event event;
    while (app.isOpen()) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (event.mouseButton.y >= window_size_y / 2.0f + 20 && event.mouseButton.y < window_size_y / 2.0f + 80) {
                        // Retry the same level
                        generate_map(map_size_x, map_size_y, mine_num);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (event.mouseButton.y >= window_size_y / 2.0f + 80 && event.mouseButton.y < window_size_y / 2.0f + 140) {
                        // Return to start screen
                        gameState = GameState::StartScreen;
                        return;
                    }
                }
            }
        }
    }
}

//functions to show cells when clicked
void open_all_zero_around(int x, int y) {
    for (auto i : surroundings) {
        if (x - i[0] >= 0 && x - i[0] < map_size_x && y - i[1] >= 0 && y - i[1] < map_size_y && map_view[y - i[1]][x - i[0]] == 10) {
            map_view[y - i[1]][x - i[0]] = map[y - i[1]][x - i[0]];
            if (map[y - i[1]][x - i[0]] == 0) {
                open_all_zero_around(x - i[0], y - i[1]);
            }
        }
    }
}
void reveal_cell(int x, int y) {
    map_view[y][x] = map[y][x];
    if (map[y][x] == 0) {
        open_all_zero_around(x, y);
    }
}


void loop() {

    sf::Event e;
    while (app.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            app.close();
        }

        if (e.type == sf::Event::MouseButtonPressed && gameState == GameState::Playing) {
            int x = (int)(e.mouseButton.x / square_size);
            int y = (int)(e.mouseButton.y / square_size);

            if (x < map_size_x && x >= 0 && y < map_size_y && y >= 0) {
                if (e.mouseButton.button == sf::Mouse::Right && map_view[y][x] == 10) {
                    map_view[y][x] = 9; // Place flag
                }
                else if (e.mouseButton.button == sf::Mouse::Left) {
                    if (map[y][x] == -1) {
                        // Game over
                        gameState = GameState::GameOver;
                    }
                    else {
                        // Reveal cell
                        reveal_cell(x, y);
                        if (check_win_condition()) {
                            gameState = GameState::GameOver;
                        }
                    }
                }
            }
        }
    }

        // Draw the grid and game elements
        app.clear(sf::Color::Black);
        for (int y = 0; y < map_size_y; y++) {
            for (int x = 0; x < map_size_x; x++) {
                sf::RectangleShape rectangle(sf::Vector2f(square_size, square_size));
                rectangle.setPosition(sf::Vector2f(x * square_size, y * square_size));
                rectangle.setFillColor(sf::Color(192, 192, 192)); // Set default color to grey for unrevealed tiles
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setOutlineThickness(1);

                if (map_view[y][x] != 10) { // If tile is revealed
                    rectangle.setFillColor(sf::Color::White); // Change color to white
                }

                app.draw(rectangle);

                if (map_view[y][x] == -1) { // Mine
                    sf::Sprite sprite;
                    sprite.setTexture(mine);
                    sprite.scale(sf::Vector2f((float)square_size / (float)mine.getSize().x, (float)square_size / (float)mine.getSize().y));
                    sprite.setPosition(sf::Vector2f(x * square_size, y * square_size));
                    app.draw(sprite);
                }
                else if (map_view[y][x] == 9) { // Flag
                    sf::Sprite sprite;
                    sprite.setTexture(flag);
                    sprite.scale(sf::Vector2f((float)square_size / (float)flag.getSize().x, (float)square_size / (float)flag.getSize().y));
                    sprite.setPosition(sf::Vector2f(x * square_size, y * square_size));
                    app.draw(sprite);
                }
                else if (map_view[y][x] != 10 && map_view[y][x] != -1) { // Revealed numbered cell
                    sf::Color colors[] = { sf::Color::Blue, sf::Color::Green, sf::Color::Red, sf::Color::Magenta, sf::Color::Cyan, sf::Color::Blue, sf::Color::Green, sf::Color::Red };

                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(map[y][x]));
                    text.setCharacterSize(square_size);
                    text.setFillColor(colors[map[y][x] - 1]);
                    text.setPosition(sf::Vector2f(x * square_size + square_size / 4, y * square_size - square_size / 6));
                    app.draw(text);
                }
            }
        }
        app.display();

}



int main() {
    // Adjust the window size to fit the initial grid size
    generate_map(EASY_SIZE_X, EASY_SIZE_Y, EASY_MINES);
    app.create(sf::VideoMode(window_size_x, window_size_y), "Minesweeper!", sf::Style::Close);

    // Load textures and font
    if (!mine.loadFromFile("images/mine.png") || !flag.loadFromFile("images/flag.jpg") || !font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load resources." << std::endl;
        return 1;
    }

    gameState = GameState::StartScreen;

    while (app.isOpen()) {
        switch (gameState) {
        case GameState::StartScreen:
            start_screen();
            break;
        case GameState::LevelSelect:
            level_select_screen();
            break;
        case GameState::Playing:
            loop();
            break;
        case GameState::GameOver:
            game_over_screen();
            break;
        }
    }
    return 0;
}