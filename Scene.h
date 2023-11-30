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
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

struct GameState
{
    Map* map;
    Entity* player;
    Entity* enemies;
    Entity* menu;

    Mix_Music* bgm;
    Mix_Chunk* jump_sfx;

    int next_scene_id;
};

class Scene {
public:
    int m_number_of_enemies = 1;
    bool win = false;
    bool gameover = false;
    bool start = false;
    int level_width = 14;
    int level_height = 8;

    GameState m_state;

    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram* program) = 0;

    GameState const get_state() const { return m_state; }
    void level_passed() { win = true; };
    bool const is_passed()      const { return win; };
    bool const game_over()      const { return gameover; };
    int const get_edge()        const { return level_width; };
    void start_game()                 { start = true; };
    bool const is_start()      const { return start; };
};