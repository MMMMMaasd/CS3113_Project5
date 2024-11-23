#include "Menu.h"
#include "Utility.h"

#define LEVEL_WIDTH 11
#define LEVEL_HEIGHT 15

constexpr char SPRITESHEET_FILEPATH[] = "assets/death_knight.png",
           ENEMY_FILEPATH[]       = "assets/main_tiles.png",
           FONT_FILEPATH[] = "assets/font1.png";

unsigned int MENU_DATA[] = {};


Menu::Menu() {}

Menu::~Menu()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}


void Menu::initialise()
{
    m_game_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/main_tiles.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 9, 2);
    m_game_state.player = new Entity();
    m_game_state.enemies = new Entity[ENEMY_COUNT];
    

    
    /**
     BGM and SFX
     */
    
}

void Menu::update(float delta_time){
    
}

void Menu::render(ShaderProgram *program)
{
    GLuint g_font_texture_id = Utility::load_texture(FONT_FILEPATH);
    Utility::draw_text(program, g_font_texture_id, "Death Knight", 0.7f, 0.01f, glm::vec3(1.0f, -1.7f, 0.0f));
    Utility::draw_text(program, g_font_texture_id,  "of", 1.0f, 0.05f, glm::vec3(4.3f, -2.6f, 0.0f));
    Utility::draw_text(program, g_font_texture_id, "The Dark Area", 0.7f, 0.01f, glm::vec3(0.5f, -3.5f, 0.0f));
    Utility::draw_text(program, g_font_texture_id, "Press enter to start", 0.3f, 0.01f, glm::vec3(2.0f, -6.0f, 0.0f));
}

void Menu::launch(){
    m_game_state.next_scene_id = 0;
}

