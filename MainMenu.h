#include "Scene.h"

class MainMenu : public Scene {
public:
    int ENEMY_COUNT = 1;
    int DEAD_ENEMIES = 0;
    glm::mat4 m_model_matrix;
    GLuint font_texture_id;
    GLuint m_texture_id;

    ~MainMenu();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
};