#pragma once

#include "gameobject.h"
#include "player.h"
#include "block.h"
#include "garbage.h"
#include <sgg/graphics.h>
#include <vector>


class Level : public GameObject
{
	graphics::Brush m_brush_background;

	typedef enum { STATUS_PLAY, STATUS_FINISH, STATUS_GAMEOVER } status_level;
	status_level status = STATUS_PLAY;

	void updateGameoverScreen(float dt);
	void drawGameoverScreen();
	void updateFinishScreen(float dt);
	void drawFinishScreen();

	std::vector<Block*> m_blocks;				// blocks
	std::vector<Block*> m_gameover_blocks;		// diafanh blocks katw apo thn gefuga gia na kanoun collide me ton paixth kai na ginetai gameover
	std::vector<MovingBlock*> m_moving_blocks;	// moving blocks gia level 2
	std::vector<Garbage*> m_garbages;			// garbages me antikeimena tupou ScoreGarbage h EnergyGarbage
	std::vector<Lever*> m_levers;				// o moxlos

	void checkCollisions(); // detect collisions

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	Level(const std::string& name = "Level0") ;
	~Level();
};
