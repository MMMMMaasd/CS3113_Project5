#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 1;
    
    LevelB();
    
    LevelB(int currentPlayerLives);
    
    ~LevelB();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
