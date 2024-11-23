#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"

/**
    Notice that the game's state is now part of the Scene class, not the main file.
*/
struct GameState
{
    // ————— GAME OBJECTS ————— //
    Map *map;
    Entity *player;
    Entity *enemies;
    // ————— AUDIO ————— //
    Mix_Music *bgm;
    Mix_Chunk *jump_sfx;
    
    // ————— POINTERS TO OTHER SCENES ————— //
    int next_scene_id;
};


class Scene {
protected:
    GameState m_game_state;
    int playerLives = 3;
public:
    // ————— ATTRIBUTES ————— //
    int m_number_of_enemies = 1;
    
    Scene() : playerLives(3) {}

    Scene(int current_lives) : playerLives(current_lives) {}
    
    // ————— METHODS ————— //
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram *program) = 0;
    // Setter to start the game //
    virtual void launch(){}
    
    // ————— GETTERS ————— //
    GameState const get_state() const { return m_game_state;             }
    int const get_number_of_enemies() const { return m_number_of_enemies; }
    int const get_scene_id() const {return m_game_state.next_scene_id; }
    int const get_current_lives() const {return playerLives; }
    
    void set_current_lives(int new_lives) {playerLives = new_lives; }
    
};
