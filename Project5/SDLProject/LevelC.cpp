#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 11
#define LEVEL_HEIGHT 15

constexpr char SPRITESHEET_FILEPATH[] = "assets/death_knight.png",
           ENEMY_FILEPATH[]       = "assets/balmer-andromalius-57x88-alpha.png";

unsigned int LEVELC_DATA[] =
{
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 14, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 11, 11, 11, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 0, 0, 0, 0, 0, 11, 11, 11, 16,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 11, 11, 11, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 0, 0, 0, 0, 0, 11, 11, 11, 16,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 11, 11, 11, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 0, 0, 0, 0, 0, 11, 11, 11, 16,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16,
    16, 11, 11, 11, 0, 0, 0, 0, 0, 0, 16,
    16, 0, 0, 0, 0, 0, 0, 11, 11, 11, 16,
    16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16,
};

LevelC::LevelC() {}

LevelC::LevelC(int currentPlayerLives) : Scene(currentPlayerLives) {}

LevelC::~LevelC()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelC::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/main_tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 9, 2);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    int player_walking_animation[4][3] =
    {
        { 0, 1, 2 },  // for Knight to move to the upwards,
        { 3, 4, 5 }, // for Knight to move to the right,
        { 6, 7, 8 },  // for Knight to move downwards,
        { 9, 10, 11 },   // for Knight to move left
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -9.80f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        3,                         // animation frame amount
        0,                         // current animation index
        3,                         // animation column amount
        4,                         // animation row amount
        0.40f,                      // width
        1.0f,                       // height
        PLAYER
    );
        
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(6.0f);
    
    /**
    Enemies' stuff */
    int ai_walking_animation[3][8] =
    {
        { 0, 1, 2, 3, 4, 5, 6, 7},
        { 8, 9, 10, 11, 12, 13, 14, 15},
        { 16, 17, 18, 19, 20, 21, 22, 23 },
    };
    
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, BALMER, IDLE, ai_walking_animation);
    }


    m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -1.8f, 0.0f));
    m_game_state.enemies[0].set_width(1.5f);
    m_game_state.enemies[0].set_height(1.5f);
    
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/Jump Sound Effect (High Quality).wav");
}

void LevelC::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    if(m_game_state.player->get_collided_enemy()){
        m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
        m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
        set_current_lives(get_current_lives() - 1);
        m_game_state.player->set_enemy_collide_status(false);
    }
    
}

void LevelC::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_number_of_enemies; i++)
            m_game_state.enemies[i].render(program);
}
