#include "block.h"
#include "util.h"


void Block::update(float dt)
{
	GameObject::update(dt);
}


void Block::draw()
{

	float x = m_pos_x + m_state->m_global_offset_x; // h thesh x upologizontas thn thesh tou paixth 
	float y = m_pos_y + m_state->m_global_offset_y; // h thesh y upologizontas thn thesh tou paixth 

	if (m_name == "eve") {
		m_brush_block.texture = m_state->getFullAssetPath("eve.png"); 
	} else if (m_name == "bridge") {
		m_brush_block.texture = m_state->getFullAssetPath("bridge.png"); 
	} else if (m_name == "bridge2") {
		m_brush_block.texture = m_state->getFullAssetPath("bridge2.png"); 
	} else
		m_brush_block.texture = m_state->getFullAssetPath("block.png");

	graphics::drawRect(x, y, 1.0f * m_block_size, 1.0f * m_block_size, m_brush_block);

	// debug 
	if (m_state->m_debugging)
		debugDraw();

}


void Block::init()
{
	// block brush
	m_brush_block.outline_opacity = 0.0f;
	
	// block brush debug 
	m_brush_block_debug.fill_opacity = 0.1f;
	SETCOLOR(m_brush_block_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_brush_block_debug.outline_color, 0.3f, 1.0f, 0.2f);
}


std::string Block::getName() const
{
	return m_name;
}


void Block::debugDraw()
{
	float x = m_pos_x + m_state->m_global_offset_x; 
	float y = m_pos_y + m_state->m_global_offset_y;

	graphics::drawRect(x, y, m_block_size, m_block_size, m_brush_block_debug);

}



void MovingBlock::draw()
{
	float x = m_pos_x + m_state->m_global_offset_x;
	float y = m_pos_y + m_state->m_global_offset_y;
	
	graphics::Brush m_brush_mblock;
	m_brush_mblock.outline_opacity = 0.0f;
	m_brush_mblock.texture = m_state->getFullAssetPath("block.png");
	graphics::drawRect(x, y, 1.0f * 1, 1.0f * 1, m_brush_mblock);

}

void MovingBlock::init()
{
}

void MovingBlock::update(float dt)
{
	Block::update(dt);

	float delta_time = dt / 1000.0f; // sec

	// enhmerwsh theshs tou block
	m_pos_x += (delta_time * velocityX) / 10;

	if (m_pos_x > max_x) // an ftasei sto megisto x
	{
		m_pos_x = max_x;
		velocityX = -velocityX;
	}
	else if (m_pos_x < min_x) // an ftasei sto elaxisto x
	{
		m_pos_x = min_x;
		velocityX = -velocityX;
	}
	
}



void Lever::update(float dt)
{
	Block::update(dt);
}

void Lever::draw()
{

	float x = m_pos_x + m_state->m_global_offset_x; // h thesh x upologizontas thn thesh tou paixth 
	float y = m_pos_y + m_state->m_global_offset_y; // h thesh y upologizontas thn thesh tou paixth 
	
	graphics::Brush m_brush_lever;
	m_brush_lever.outline_opacity = 0.0f; 
	if (isActive()) {
		m_brush_lever.texture = m_state->getFullAssetPath("lever.png"); 
	}
	else 
		m_brush_lever.texture = m_state->getFullAssetPath("lever2.png");

	graphics::drawRect(x, y, 1.0f * 1, 1.0f * 1, m_brush_lever);

}

void Lever::init()
{

}
