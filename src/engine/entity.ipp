#pragma once

#include <algorithm>
#ifndef ENTITY_H_INCLUDED
#include "entity.h"
#endif

template <typename T, typename... Args>
T *Entity::AddComponent(Args &&...args) {
  T *component = new T(std::forward<Args>(args)...);
  component->owner = this;
  components.emplace_back(component);
  return component;
}

template <typename T> T *Entity::GetComponent() {
  for (auto &comp : components) {
    if (T *casted = dynamic_cast<T *>(comp.get())) {
      return casted;
    }
  }
  return nullptr;
}

template <typename T> bool Entity::HasComponent() {
  return GetComponent<T>() != nullptr;
}

template <typename T> void Entity::RemoveComponent() {
  components.erase(std::remove_if(components.begin(), components.end(),
                                  [](const std::unique_ptr<Component> &comp) {
                                    return dynamic_cast<T *>(comp.get()) !=
                                           nullptr;
                                  }),
                   components.end());
}
