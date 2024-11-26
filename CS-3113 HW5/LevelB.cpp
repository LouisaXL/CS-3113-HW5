#include "LevelB.h"
#include "Utility.h"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

constexpr char FISH_FILEPATH[] = "assets/fish.png",
MAP_TILESET_FILEPATH[] = "assets/tileset.png",
BGM_FILEPATH[] = "assets/bgm.mp3",
JUMP_SFX_FILEPATH[] = "assets/bounce.wav",
CRAB_FILEPATH[] = "assets/crab.png",
BOAT_FILEPATH[] = "assets/boat.png",
PLATFORM_FILEPATH[] = "assets/platformPack_tile027.png",
WORD_FILEPATH[] = "assets/words.png";

unsigned int LEVELB_DATA[] =
{
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2,
    3, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2
};

LevelB::~LevelB()
{
    delete[] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}



void LevelB::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 4, 1);


    // ————— PLAYER ————— //

    GLuint player_texture_id = Utility::load_texture(FISH_FILEPATH);

    int player_walking_animation[4][4] =
    {
        { 1, 5, 9, 13 },  // for George to move to the left,
        { 3, 7, 11, 15 }, // for George to move to the right,
        { 2, 6, 10, 14 }, // for George to move upwards,
        { 0, 4, 8, 12 }   // for George to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -10.0f, 0.0f);

    m_game_state.player = new Entity(
        player_texture_id,         // texture id
        5.0f,                      // speed
        acceleration,              // acceleration
        5.0f,                      // jumping power
        player_walking_animation,  // animation index sets
        0.0f,                      // animation time
        4,                         // animation frame amount
        0,                         // current animation index
        4,                         // animation column amount
        4,                         // animation row amount
        0.6f,                      // width
        0.7f,                       // height
        PLAYER
    );

    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(5.0f);


    // ————— NPC ————— //

    GLuint enemy_texture_id = Utility::load_texture(CRAB_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i] = Entity(enemy_texture_id, 0.7f, 0.8f, 0.8f, ENEMY, GUARD, IDLE);
    }


    m_game_state.enemies[0].set_position(glm::vec3(6.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    //m_game_state.enemies[0].set_scale(glm::vec3(2.0f, 2.0f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_game_state.bgm = Mix_LoadMUS("assets/dooblydoo.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);

    m_game_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");

}


void LevelB::update(float delta_time)
{


    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_game_state.enemies[i].update(delta_time, m_game_state.player, NULL, NULL, m_game_state.map);
    }

    m_game_state.player->check_player_hit(m_game_state.enemies, ENEMY_COUNT);
    m_game_state.player->check_enemy_hit(m_game_state.enemies, ENEMY_COUNT);

    // TODO: BELOW WITH ENEMY HIT NOT WORKING
    if (m_game_state.player->get_is_hit() == true) {
        if (m_game_state.player->get_lives() == 0) {
            // no lives, game over
            m_game_state.player->deactivate();
        }
        else if (m_game_state.player->get_lives() != 0) {
            m_game_state.player->decrease_lives();                          // decrease one life
            m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f)); // reset position
        }
        m_game_state.player->set_is_hit_false();
    }

    // IF PLAYER FALL, LOSE LIVE
    if (m_game_state.player->get_position().y < -5.0) {

        if (m_game_state.player->get_lives() == 0) {
            // no lives, game over
            m_game_state.player->deactivate();
        }
        else if (m_game_state.player->get_lives() != 0) {
            m_game_state.player->decrease_lives();                          // decrease one life
            m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f)); // reset position
        }
    }
}

void LevelB::render(ShaderProgram* g_shader_program)
{
    GLuint g_font_texture_id = Utility::load_texture(WORD_FILEPATH);

    m_game_state.map->render(g_shader_program);
    m_game_state.player->render(g_shader_program);
    for (int i = 0; i < m_number_of_enemies; i++)
        m_game_state.enemies[i].render(g_shader_program);

    if (m_game_state.player->get_is_active() == false) {
        Utility::draw_text(g_shader_program, g_font_texture_id, "WASTED", 0.5f, 0.05f,
            glm::vec3(9.0f, -1.0f, 0.0f));
    }
    if (m_game_state.enemies->get_is_active() == false && ENEMY_COUNT != 0) {
        Utility::draw_text(g_shader_program, g_font_texture_id, "WIN", 0.5f, 0.05f,
            glm::vec3(5.0f, 1.0f, 0.0f));
    }
}