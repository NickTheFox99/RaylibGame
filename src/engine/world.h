#include "entity.h"
#include <vector>
class World {
public:
  void Update(float dt);
  void Draw();
  void AddEntity(Entity entity) {}

private:
  std::vector<Entity> entities;
};
