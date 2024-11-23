#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 17
#define LEVEL_HEIGHT 8

constexpr char SPRITESHEET_FILEPATH[] = "assets/death_knight.png",
           ENEMY_FILEPATH[]       = "assets/redshrike-wartaur-alpha.png";

unsigned int LEVELB_DATA[] =
{
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    16, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    16, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    16, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
     
};

LevelB::LevelB() {}

LevelB::LevelB(int currentPlayerLives) : Scene(currentPlayerLives) {}

LevelB::~LevelB()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelB::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/main_tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 9, 2);
    
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
    m_game_state.player->set_jumping_power(5.0f);
    
    /**
    Enemies' stuff */
    
    int ai_walking_animation[3][4] =
    {
        { 0, 1, 2, 3},  //move left
        { 4, 5, 6, 7}, // attack
        { 8, 9, 10, 11 },
    };
    
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, WARTAUR, IDLE, ai_walking_animation);
    }


    m_game_state.enemies[0].set_position(glm::vec3(15.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_game_state.enemies[0].set_width(2.0f);
    m_game_state.enemies[0].set_height(3.5f);
    
    
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/Jump Sound Effect (High Quality).wav");
}

void LevelB::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }
    
    
    if(m_game_state.player->get_collided_enemy()){
        m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
        m_game_state.enemies[0].set_position(glm::vec3(15.0f, 0.0f, 0.0f));
        set_current_lives(get_current_lives() - 1);
        m_game_state.player->set_enemy_collide_status(false);
    }
    
    if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 2;
}
 
void LevelB::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < m_number_of_enemies; i++)
            m_game_state.enemies[i].render(program);
}
  
