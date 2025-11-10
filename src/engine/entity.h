#pragma once
#define ENTITY_H_INCLUDED
#include "component.h"
#include <memory>
#include <vector>

class Entity {
public:
  template <typename T, typename... Args> T *AddComponent(Args &&...args);
  template <typename T> T *GetComponent();
  template <typename T> bool HasComponent();
  template <typename T> void RemoveComponent();

  void Update(float dt);
  void Draw();

private:
  std::vector<std::unique_ptr<Component>> components;
};

#include "entity.ipp"
