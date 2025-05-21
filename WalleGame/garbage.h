#pragma once

#include <sgg/graphics.h>
#include "gameobject.h"
#include "box.h"

class Garbage : public Box, public GameObject
{

public:
    graphics::Brush m_garbage_brush;
    graphics::Brush m_garbage_brush_debug;
    const float m_garbage_size = 1.0f;

    virtual void update(float dt) override;
    virtual void draw() override;
    virtual void init() override;

    Garbage(std::string name, float x, float y) : GameObject(name) { m_pos_x = x, m_pos_y = y; }

    virtual std::string getName() const;

protected:
    virtual void debugDraw();

};


class ScoreGarbage : public Garbage 
{

public:
    void update(float dt) override;
    void draw() override;
    void init() override;
    
    ScoreGarbage(std::string name, float x, float y) : Garbage(name, x, y) {}

};


class EnergyGarbage : public Garbage
{

public:
    void update(float dt) override;
    void draw() override;
    void init() override;
    
    EnergyGarbage(std::string name, float x, float y) : Garbage(name, x, y) {}
    
};
