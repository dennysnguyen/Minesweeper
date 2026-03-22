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
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("MINESWEEPER");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);

    // Center Title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(window_size_x / 2.0f, window_size_y / 2.0f - 40);

    sf::Text startText;
    startText.setFont(font);
    startText.setString("Click anywhere to start");
    startText.setCharacterSize(24);
    startText.setFillColor(sf::Color(200, 200, 200));

    // Center Subtitle
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setOrigin(startBounds.left + startBounds.width / 2.0f, startBounds.top + startBounds.height / 2.0f);
    startText.setPosition(window_size_x / 2.0f, window_size_y / 2.0f + 40);

    app.clear(sf::Color(30, 30, 30)); // Dark Charcoal Background
    app.draw(titleText);
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
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Select Difficulty");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(window_size_x / 2.0f, 100);

    // Lambda helper to easily format and center our buttons
    auto setupButton = [&](sf::Text& t, const std::string& str, float yPos) {
        t.setFont(font);
        t.setString(str);
        t.setCharacterSize(30);
        t.setFillColor(sf::Color(100, 250, 100)); // Light Green
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        t.setPosition(window_size_x / 2.0f, yPos);
        };

    sf::Text easyText, mediumText, hardText;
    setupButton(easyText, "Easy (9x9)", 220);
    setupButton(mediumText, "Medium (16x16)", 300);
    setupButton(hardText, "Hard (30x16)", 380);

    while (app.isOpen()) {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                    // Click detection using bounding boxes
                    if (easyText.getGlobalBounds().contains(mousePos)) {
                        generate_map(EASY_SIZE_X, EASY_SIZE_Y, EASY_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (mediumText.getGlobalBounds().contains(mousePos)) {
                        generate_map(MEDIUM_SIZE_X, MEDIUM_SIZE_Y, MEDIUM_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                    else if (hardText.getGlobalBounds().contains(mousePos)) {
                        generate_map(HARD_SIZE_X, HARD_SIZE_Y, HARD_MINES);
                        gameState = GameState::Playing;
                        return;
                    }
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                // Hover Effects - Turn text white if the mouse is over it
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                easyText.setFillColor(easyText.getGlobalBounds().contains(mousePos) ? sf::Color::White : sf::Color(100, 250, 100));
                mediumText.setFillColor(mediumText.getGlobalBounds().contains(mousePos) ? sf::Color::White : sf::Color(100, 250, 100));
                hardText.setFillColor(hardText.getGlobalBounds().contains(mousePos) ? sf::Color::White : sf::Color(100, 250, 100));
            }
        }

        app.clear(sf::Color(30, 30, 30));
        app.draw(titleText);
        app.draw(easyText);
        app.draw(mediumText);
        app.draw(hardText);
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