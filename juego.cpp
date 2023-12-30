#include "juego.h"

QSize Game::RESOLUTION;
float Game::DELAY;
const float Game::JUMP_FORCE = -10.0f;
const int Game::X_OFFSET = 20;
const int Game::Y_OFFSET = 70;
const float Game::JUMP_SPEED = 0.2f;
const int Game::DEAD_LEVEL = 500;;
const QSize Game::HERO_SIZE = QSize(34, 64);
const QSize Game::PLATFORM_SIZE = QSize(64, 16);
const QSize Game::NUMBER_SIZE = QSize(32, 32);

Game::Game()
{
    PATH_TO_BACKGROUND_PIXMAP = ":/fondoMP.png";
    PATH_TO_BACKGROUND2_PIXMAP = ":/fondoNoche.png";
    PATH_TO_HERO_PIXMAP = ":/personInca2.png";
    PATH_TO_PLATFORM_PIXMAP = ":/piso.png";
    PATH_TO_ALL_NUMBERS_PIXMAP = ":/all_numbers.png";
    PATH_TO_PAUSED_BG = ":/bg_pause.png";
    PATH_TO_GAME_OVER_BG = ":/bg_gameover.png";
    POINTS = 0;


}

void Game::init()
{
    RESOLUTION = QSize(400, 533);
    DELAY = 0.2f;
}
