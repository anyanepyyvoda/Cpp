#include "garbage.h"
#include "util.h"

void Garbage::update(float dt)
{
    GameObject::update(dt);
}


void Garbage::draw()
{
    float x = m_pos_x + m_state->m_global_offset_x; // h thesh x upologizontas thn thesh tou paixth 
    float y = m_pos_y + m_state->m_global_offset_y; // h thesh y upologizontas thn thesh tou paixth 

    graphics::drawRect(x, y, m_garbage_size, m_garbage_size, m_garbage_brush);

    // debug
    if (m_state->m_debugging)
        debugDraw();
}


void Garbage::init()
{
    m_garbage_brush.fill_opacity = 1.0f;
    m_garbage_brush.outline_opacity = 0.0f;

    // Adjust width for finer collision detection
    m_width = 0.5f;
}


std::string Garbage::getName() const
{
    return m_name;
}


void Garbage::debugDraw()
{
    m_garbage_brush_debug.fill_opacity = 0.1f;
    SETCOLOR(m_garbage_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
    SETCOLOR(m_garbage_brush_debug.outline_color, 0.3f, 1.0f, 0.2f); 

    float x = m_pos_x + m_state->m_global_offset_x;
    float y = m_pos_y + m_state->m_global_offset_y;

    graphics::drawRect(x, y, m_garbage_size*0.6f, m_garbage_size*0.8f, m_garbage_brush_debug);
}



void ScoreGarbage::draw()
{
    Garbage::draw();
}

void ScoreGarbage::update(float dt)
{
    Garbage::update(dt);
}

void ScoreGarbage::init()
{
    Garbage::init(); 
    m_garbage_brush.texture = m_state->getFullAssetPath(m_name + ".png");
  
}



void EnergyGarbage::draw()
{
    Garbage::draw();
}

void EnergyGarbage::update(float dt)
{
    Garbage::update(dt);
}

void EnergyGarbage::init()
{
    Garbage::init();
    m_garbage_brush.texture = m_state->getFullAssetPath(m_name + ".png");
}
