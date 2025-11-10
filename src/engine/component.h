#pragma once

class Entity;

class Component {
public:
  Entity *owner = nullptr;

  virtual ~Component() = default;
  virtual void Update(float dt) {}
  virtual void Draw() {}
};
