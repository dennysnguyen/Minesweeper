#pragma once
#include <string>

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
