#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 8
#define LOG(argument) std::cout << argument << '\n'

unsigned int LEVELC_DATA[] =
{
    79, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 44,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 21, 21, 21, 21,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 20, 22, 00, 00, 00, 00, 00,
    75, 00, 00, 20, 21, 21, 21, 22, 00, 00, 00, 00, 00, 00, 00, 00,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
    75, 103, 106, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
    95, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};

/*
    79, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 52,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 52,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 21, 52,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 20, 22, 00, 00, 52,
    75, 00, 00, 00, 00, 20, 21, 22, 00, 00, 00, 00, 00, 52,
    75, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 52,
    75, 103, 106, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 52,
    95, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 52
*/

LevelC::~LevelC()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelC::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/tilemap.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 16, 7);

    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/drake.png");

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[8] { 43, 42, 41, 40, 39, 38, 37, 36 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[8] { 36, 37, 38, 39, 40, 41, 42, 43 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 9;
    m_state.player->m_animation_rows = 5;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    // Jumping
    m_state.player->m_jumping_power = 7.0f;

    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/images/monster.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(BRUTE);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(10.0f, 0.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));


    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/Gothamlicious.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(1.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump.wav");
}

void LevelC::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    //LOG(ENEMY_COUNT);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
        if (m_state.player->is_colliding_other_x()) {
            //LOG("reached");
            m_state.player->take_damage(&m_state.enemies[i]);
            //LOG(m_state.player->get_health());
        }
        if (m_state.player->is_colliding_other_y()) {
            m_state.enemies[i].take_damage(m_state.player);
        }

        if (m_state.enemies[i].get_health() <= 0.0f && m_state.enemies[i].is_active()) {
            m_state.enemies[i].deactivate();
            DEAD_ENEMIES += 1;
        }
    }
    if (m_state.player->get_health() <= 0.0f && m_state.player->is_active()) {
        m_state.player->deactivate();
        gameover = true;
    }
    if (DEAD_ENEMIES == ENEMY_COUNT) {
        // 13, 27, 41, 55, 69, 83, 97, 111 
        //LOG("reached");
        LEVELC_DATA[13] = 0;
        LEVELC_DATA[27] = 0;
        LEVELC_DATA[41] = 0;
        LEVELC_DATA[55] = 0;
        LEVELC_DATA[69] = 0;
        LEVELC_DATA[83] = 0;
        LEVELC_DATA[97] = 0;
        LEVELC_DATA[111] = 0;
    }
}

void LevelC::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}