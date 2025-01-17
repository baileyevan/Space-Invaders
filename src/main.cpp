/*
Made by: Bailey Reese
For: Computer Org Programming Final Project
Date: 11/25/2024

Description: Own version of the popular game Space Invaders
*/

//core libraries
#include "bn_core.h"
#include <iostream>
#include <cstdlib>
#include <optional> 

//libs for interactions
#include "bn_keypad.h"

//libs for math
#include "bn_vector.h"
#include <ctime>

//background
#include "bn_regular_bg_items_background.h"
#include "bn_regular_bg_ptr.h"

//interactables
#include "bn_sprite_ptr.h" 
#include "bn_sprite_items_player_ship.h"
#include "bn_sprite_items_player_missile.h"
#include "bn_sprite_items_enemy_missile.h"
#include "bn_sprite_items_alien.h"

//string 
#include <string>
#include "bn_string.h"

//text
#include "bn_sprite_text_generator.h"
#include "common_variable_8x8_sprite_font.h"


constexpr int ROWS = 2; // Number of rows
constexpr int COLUMNS = 7; // Number of columns
constexpr int START_X = -65; // Starting X coordinate
constexpr int START_Y = -50; // Starting Y coordinate
constexpr int X_SPACING = 25; // Horizontal spacing between aliens
constexpr int Y_SPACING = 20; // Vertical spacing between aliens

int collision_range = 12;

// amount of aliens alive initially
int aliveAliens = 0;

// wave of aliens player is on
int wave = 0;

//tells if the game has started
bool isStarted = false;
bool isAlive = false;

//initialize scores
int score = 0, highScoreInt = 0;

// Declare 2D array for aliens
std::optional<bn::sprite_ptr> aliens[ROWS][COLUMNS];

// aliens remaining text
bn::string<32> aliensRemainingText = "Aliens: " + bn::to_string<32>(aliveAliens);

bn::string<32> currentWave;

void initialize_aliens() {
    for(int row = 0; row < ROWS; row++)
    {
        for(int col = 0; col < COLUMNS; col++)
        {
            int x = START_X + col * X_SPACING; // Calculate the x-coordinate
            int y = START_Y + row * Y_SPACING; // Calculate the y-coordinate

            // Initialize the alien sprite at the current position
            bn::sprite_ptr alien = bn::sprite_items::alien.create_sprite(x, y);
            alien.set_z_order(2);
            aliens[row][col] = alien;
        }
    }
    aliveAliens = ROWS * COLUMNS;
}

// pick random aliens to shoot
void alienShoot(bn::sprite_ptr& player_ship, std::optional<bn::sprite_ptr> aliens[ROWS][COLUMNS], bn::sprite_ptr& enemy_missile) {
    // Select a random alien
    int randomRow = rand() % ROWS;
    int randomCol = rand() % COLUMNS;

    // Ensure the selected alien is valid
    if (aliens[randomRow][randomCol]) {
        bn::sprite_ptr& alien = *aliens[randomRow][randomCol];

        // Position the existing enemy missile on the alien
        enemy_missile.set_x(alien.x());
        enemy_missile.set_y(alien.y());
    }
}


int main()
{
    bn::core::init();

    //make the background
    bn::regular_bg_ptr background = bn::regular_bg_items::background.create_bg(0, 20);

    //initialize player starting point
    int playerX = 0;
    int playerMissileY = 0;

    //make the players ship
    bn::sprite_ptr player_ship = bn::sprite_items::player_ship.create_sprite(playerX, 65);;

    //initialize player missile
    bn::sprite_ptr player_missile = bn::sprite_items::player_missile.create_sprite(player_ship.x(), -1000);;

    //initialize player missile
    bn::sprite_ptr enemy_missile = bn::sprite_items::enemy_missile.create_sprite(0, 1000);

    player_ship.set_z_order(0);
    player_missile.set_z_order(1);
    enemy_missile.set_z_order(1);

    srand(time(0)); 

    //generate text sprite
    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 16> text_sprites;
    text_generator.generate(-55, -20, "(Press X to start)", text_sprites);
    text_generator.generate(-55, 0, "(Press S to shoot)", text_sprites);
    text_generator.generate(-55, 20, "(Use Arrows to Move)", text_sprites);

    

    

    while(true)
    {

        // start game get rid of text sprite
        if(bn::keypad::a_pressed() && !isStarted && !isAlive) {
            isStarted = true;
            isAlive = true;
            
            wave = 1;
            for(bn::sprite_ptr& text_sprite : text_sprites) {
                text_sprite.set_visible(false); // Hide old text sprites
            }

            text_sprites.clear();

            initialize_aliens();

            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

            aliensRemainingText = "Aliens: " + bn::to_string<32>(aliveAliens);
            text_generator.generate(50, -68, aliensRemainingText, text_sprites);

            currentWave = "Wave: " + bn::to_string<32>(wave);
            text_generator.generate(0, -68, currentWave, text_sprites);


        }

        if(isStarted && isAlive && aliveAliens == 0) {
            initialize_aliens();
            wave += 1;

            for(bn::sprite_ptr& text_sprite : text_sprites) {
                text_sprite.set_visible(false); // Hide old text sprites
            }

            text_sprites.clear();

            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

            aliensRemainingText = "Aliens: " + bn::to_string<32>(aliveAliens);
            text_generator.generate(50, -68, aliensRemainingText, text_sprites);

            currentWave = "Wave: " + bn::to_string<32>(wave);
            text_generator.generate(0, -68, currentWave, text_sprites);

        }

        if(bn::keypad::left_held() && playerX > -105) {
            player_ship.set_x(player_ship.x() - 2);
            playerX -= 2;
        }

        if(bn::keypad::right_held() && playerX < 105) {
            player_ship.set_x(player_ship.x() + 2);
            playerX += 2;
        }

        // R is S on keyboard
        if(bn::keypad::r_pressed() && player_missile.y() <= -90 && isStarted) {
            player_missile.set_x(player_ship.x());
            player_missile.set_y(player_ship.y());
        }

        // Move the missile upward if it has been launched
        if(player_missile.y() > -90) {
            player_missile.set_y(player_missile.y() - 3);
        }

        if (aliveAliens > 0 && player_missile.y() > -90) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLUMNS; j++) {
                    // Check if the alien exists (it might have been reset already)
                    if (aliens[i][j]) {

                        bn::sprite_ptr& alien = *aliens[i][j];

                        // Check if the player missile collides with the alien
                        if (player_missile.x() < alien.x() + collision_range &&
                            player_missile.x() > alien.x() - collision_range &&
                            player_missile.y() < alien.y() + collision_range &&
                            player_missile.y() > alien.y() - collision_range) {


                            alien.set_visible(false);

                            // Reset the alien sprite and reduce the count of alive aliens
                            aliens[i][j].reset();
                            aliveAliens -= 1;

                            score += 1;

                            for(bn::sprite_ptr& text_sprite : text_sprites) {
                                text_sprite.set_visible(false); // Hide old text sprites
                            }

                            text_sprites.clear();

                            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
                            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

                            aliensRemainingText = "Aliens: " + bn::to_string<32>(aliveAliens);
                            text_generator.generate(50, -68, aliensRemainingText, text_sprites);

                            currentWave = "Wave: " + bn::to_string<32>(wave);
                            text_generator.generate(0, -68, currentWave, text_sprites);


                            // Optionally, reset the player missile position
                            player_missile.set_y(-100); // Move it off-screen or handle appropriately
                        }
                    }
                }
            }
        }

        // trigger random alien to shoot
        if(isAlive && isStarted && aliveAliens > 0 && enemy_missile.y() > (player_ship.y() + 20)) {
            alienShoot(player_ship, aliens, enemy_missile);
        }

        // move shot enemy missile to bottom of screen
        if (enemy_missile.y() < 90) {
            enemy_missile.set_y(enemy_missile.y() + 3);
        }

        if (enemy_missile.x() < player_ship.x() + collision_range &&
            enemy_missile.x() > player_ship.x() - collision_range &&
            enemy_missile.y() < player_ship.y() + collision_range &&
            enemy_missile.y() > player_ship.y() - collision_range) {
                isAlive = false;
            }
        

        if(!isAlive && isStarted && wave > 0) {

            for (int i = 0; i < ROWS; ++i) {
                for (int j = 0; j < COLUMNS; ++j) {
                    aliens[i][j] = std::nullopt; 
                }
            }

            aliveAliens = 0;
            isStarted = false;
            wave = 0;
            player_ship.set_x(0);
            playerX = 0;

            for(bn::sprite_ptr& text_sprite : text_sprites) {
                text_sprite.set_visible(false); // Hide old text sprites
            }

            text_sprites.clear();

            if(score > highScoreInt) {
                highScoreInt = score;
            }

            score = 0;

            for(bn::sprite_ptr& text_sprite : text_sprites) {
                text_sprite.set_visible(false); // Hide old text sprites
            }

            text_sprites.clear();

            bn::string<32> high_score = "High Score: " + bn::to_string<32>(highScoreInt);
            text_generator.generate(-66, -68, high_score, text_sprites);

            text_generator.generate(-55, -20, "(Press X to start)", text_sprites);
            text_generator.generate(-55, 0, "(Press S to shoot)", text_sprites);
            text_generator.generate(-55, 20, "(Use Arrows to Move)", text_sprites);

            } 
        
        bn::core::update();
    }
}


