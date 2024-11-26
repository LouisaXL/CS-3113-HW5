#include "LevelStart.h"
#include "Utility.h"


constexpr char FISH_FILEPATH[] = "assets/fish.png",
MAP_TILESET_FILEPATH[] = "assets/tileset.png",
BGM_FILEPATH[] = "assets/bgm.mp3",
JUMP_SFX_FILEPATH[] = "assets/bounce.wav",
CRAB_FILEPATH[] = "assets/crab.png",
BOAT_FILEPATH[] = "assets/boat.png",
PLATFORM_FILEPATH[] = "assets/platformPack_tile027.png",
ENEMY_FILEPATH[] = "assets/soph.png",
WORD_FILEPATH[] = "assets/words.png";

void LevelStart::initialise() {

}

void LevelStart::update(float delta_time)
{
}

void LevelStart::render(ShaderProgram* g_shader_program)
{
    GLuint g_font_texture_id = Utility::load_texture(WORD_FILEPATH);

    Utility::draw_text(g_shader_program, g_font_texture_id, "Arrow to walk, Space to jump; ", 0.5f, 0.05f,
        glm::vec3(1.0f, 2.0f, 0.0f));

    Utility::draw_text(g_shader_program, g_font_texture_id, "Kill the enemies to move on;", 0.5f, 0.05f,
        glm::vec3(1.0f, 0.0f, 0.0f));

    Utility::draw_text(g_shader_program, g_font_texture_id, "ENTER TO BEGIN", 0.5f, 0.05f,
        glm::vec3(0.0f, -2.0f, 0.0f));


}