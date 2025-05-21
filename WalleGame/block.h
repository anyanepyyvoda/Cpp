#pragma once

#include <sgg/graphics.h>
#include "gameobject.h"
#include "box.h"

class Block : public Box, public GameObject
{
	graphics::Brush m_brush_block;
	graphics::Brush m_brush_block_debug;
	const float m_block_size = 1.0f;

public:
	virtual void update(float dt) override;
	virtual void draw() override;
	virtual void init() override;
	
	Block(std::string name, float x, float y) : GameObject(name) { m_pos_x = x, m_pos_y = y; }
	
	virtual std::string getName() const;

protected:
	virtual void debugDraw();

};

class MovingBlock : public Block {

public:
	float max_x; // megisto x pou ftanei to block
	float min_x; // elaxisto x pou ftanei to block
	float velocityX = 10.0;

	MovingBlock(std::string name, float x, float y, float mi_x, float ma_x) : Block(name, x, y) { min_x = mi_x, max_x = ma_x; }

	void update(float dt);
	void draw();
	void init();
};


class Lever: public Block {

public:
	
	Lever(std::string name, float x, float y) : Block(name, x, y) { ; }

	void update(float dt);
	void draw();
	void init();

};