#include "Scene.h"

class LevelA : public Scene {
public:
    // ————— STATIC ATTRIBUTES ————— //
    int ENEMY_COUNT = 1;
    int DEAD_ENEMIES = 0;
    bool win = false;
    bool gameover = false;
    int level_width = 14;
    int level_height = 8;

    // ————— CONSTRUCTOR ————— //
    ~LevelA();

    // ————— METHODS ————— //
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;

    void level_passed() { win = true; };
    bool const is_passed()      const { return win; };
    bool const game_over()      const { return gameover; };
    int const get_edge()        const { return level_width; };
    void const end_game() { gameover = true; };
};