#pragma once

#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>


class Player : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_sprites;
	std::vector<std::string> m_sprites_left;
	std::vector<std::string> m_sprites_jump;
	
	graphics::Brush m_brush_player;

	// dynamic motion control
	const float m_accel_horizontal = 35.0f; // epitaxunsh gia thn orizontia kateuthish
	const float m_accel_vertical = 300.1f;  // epitaxunsh gia thn katheth kateuthinsh
	const float m_max_velocity = 5.0f;      // megisth taxythta
	const float m_gravity = 10.0f;			// varythta - poso grhgora tha epanaferetai pros ta katw otan paei na phdhksei
	float life = 1.0f; 


public:
	float m_vx = 0.0f;  // taxythta tou paixth ston x
	float m_vy = 0.0f;	// taxuthta tou paixth ston y

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Player(std::string name) : GameObject(name) {}

protected:
	void debugDraw();
	
	// dynamic motion control
	void movePlayer(float dt);	// metakinhsh tou paixth

public:	
	void drainLife(float amount);
	void addLife(float amount);
	float getRemainingLife() const;
	void fillLife();
	~Player();
};
