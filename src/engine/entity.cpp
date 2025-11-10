#include "entity.h"

void Entity::Update(float dt) {
  for (auto &comp : components) {
    comp->Update(dt);
  }
}

void Entity::Draw() {
  for (auto &comp : components) {
    comp->Draw();
  }
}
