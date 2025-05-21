#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "garbage.h"
#include "block.h"
#include "util.h"
#include <sgg/graphics.h>
#include <thread>
#include <chrono>


void Level::checkCollisions()
{
	// COLLISION PLAYER ME BLOCKS DOWN
	for (auto & block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(*block)) 
		{
			m_state->getPlayer()->m_pos_y += offset; // phgaine ton paixth toso panw oso to offset
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("jump.wav"), 1.0f);
			m_state->getPlayer()->m_vy = 0.0f;
			break;
		}
	}

	// COLLISION PLAYER ME BLOCKS SIDEWAYS
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(*block))
		{
			if (block->getName() == "eve" && m_state->getScore() >=35 ) 
			{
				graphics::playSound(m_state->getFullAssetPath("collideWeve2.wav"), 0.3f);
				std::this_thread::sleep_for(std::chrono::seconds(4));
				status = STATUS_FINISH;
			}
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}

	}

	// COLLISION PLAYER ME BLOCKS UP
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = block->intersectDown(*m_state->getPlayer()))
		{
			m_state->getPlayer()->m_pos_y -= offset; // phgaine ton paixth toso katw oso to offset
			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}
	}
	

	// COLLISION PLAYER ME GARBAGES
	for (auto& p_garbage : m_garbages) 
	{
		Garbage& garbage = *p_garbage; // pointer gia na paroume to antikeimeno apo ton vector

		if (garbage.isActive()) 
		{
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectDown(garbage) || m_state->getPlayer()->intersectSideways(garbage))
			{
				garbage.setActive(false);
				if (typeid(garbage) == typeid(ScoreGarbage)) {
					if (garbage.getName() == "plant") {
						m_state->addScore(35);
						graphics::playSound(m_state->getFullAssetPath("collectedScorePlant.wav"), 0.2f);
					}
					else {
						m_state->addScore(5);
						graphics::playSound(m_state->getFullAssetPath("collectedScore.wav"), 0.5f);
					}
					
				}
				else if (typeid(garbage) == typeid(EnergyGarbage)) {
					if (garbage.getName() == "energy") {
						m_state->getPlayer()->addLife(0.3f);
						graphics::playSound(m_state->getFullAssetPath("collectedEnergy.wav"), 0.2f);
					}
					else {
						m_state->getPlayer()->drainLife(0.5f);
						graphics::playSound(m_state->getFullAssetPath("collectedToxicEnergy.wav"), 0.3f);
					}
				}
				break;
			}
		}
	}


	// COLLISION PLAYER ME GAMEOVER BLOCKS DOWN
	for (auto& block : m_gameover_blocks)
	{
		if (m_state->getPlayer()->intersectDown(*block))
		{
			status = STATUS_GAMEOVER;
			break;
		}
	}



	// COLLISIONS MONO GIA LEVEL 2
	if (m_name == "2.lvl") {

		// COLLISION PLAYER ME MOVING BLOCKS DOWN
		for (auto& block : m_moving_blocks)
		{
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectDown(*block))
			{
				m_state->getPlayer()->m_pos_y += offset;
				if (m_state->getPlayer()->m_vy > 1.0f)
					graphics::playSound(m_state->getFullAssetPath("jump.mp3"), 1.0f);
				m_state->getPlayer()->m_vy = 0.0f;
				break;
			}
		}

		// COLLISION PLAYER ME MOVING BLOCKS SIDEWAYS 
		for (auto& block : m_moving_blocks)
		{
			float offset = 0.0f;
			if (offset = m_state->getPlayer()->intersectSideways(*block))
			{
				m_state->getPlayer()->m_pos_x += offset;

				m_state->getPlayer()->m_vx = 0.0f;
				break;
			}

		}

		// COLLISION PLAYER ME MOVING BLOCKS UP
		for (auto& block : m_moving_blocks)
		{
			float offset = 0.0f;
			if (offset = block->intersectDown(*m_state->getPlayer()))
			{
				m_state->getPlayer()->m_pos_y -= offset; // phgaine ton paixth toso katw oso to offset
				m_state->getPlayer()->m_vx = 0.0f;
				break;
			}
		}


		// COLLISION PLAYER ME MOXLO
		for (auto& p_lever : m_levers)
		{
			Lever& lever = *p_lever; // pointer gia na paroume to antikeimeno apo ton vector

			if (m_state->getPlayer()->intersectDown(lever) || m_state->getPlayer()->intersectSideways(lever))
			{
				if (!lever.isActive()) 
				{
					// emfanish block1
					Block* block1 = new Block("block", 12.0, 6.0);
					block1->init();
					m_blocks.push_back(block1);

					// emfanish block2
					Block* block2 = new Block("block", 14.0, 4.0);
					block2->init();
					m_blocks.push_back(block2);

					// emfanish plant
					ScoreGarbage* plant = new ScoreGarbage("plant", 14.0, 3.0);
					plant->init();
					m_garbages.push_back(plant);

					lever.setActive(1); // to thetoume mh active gia na allaksei thn fwtogrfia
				}
				break;
			}
		}


	}
	
}


void Level::updateGameoverScreen(float dt)
{
	GameObject::update(dt);
	graphics::playSound(m_state->getFullAssetPath("musicFailedWalleSigh.wav"), 0.1f);
}


void Level::updateFinishScreen(float dt)
{
	GameObject::update(dt);
	m_state->switchToNextLevel();
	graphics::playSound(m_state->getFullAssetPath("musicWon.wav"), 0.1f);
}


void Level::update(float dt)
{
	if (status == STATUS_GAMEOVER)
	{
		updateGameoverScreen(dt);
	}
	else if (status == STATUS_FINISH)
	{
		updateFinishScreen(dt);
	}
	else {

		if (m_state->getPlayer()->isActive())
			m_state->getPlayer()->update(dt);
		
		if (m_state->getPlayer()->getRemainingLife() <= 0.0)
			status = STATUS_GAMEOVER;

		// enhmerwnetai prwta h thesh tou paixth kai meta to collision check
		checkCollisions();


		if (m_name == "2.lvl") 
		{
			for (auto& block : m_moving_blocks) // update moving blocks 
				block->update(dt);

			for (auto& lever : m_levers) // update lever
				lever->update(dt);

		}

		GameObject::update(dt);
	}
}


void Level::drawGameoverScreen()
{
	graphics::Brush m_brush_gameover;
	m_brush_gameover.outline_opacity = 0.0f;
	m_brush_gameover.texture = m_state->getFullAssetPath("gameover.png");

	graphics::drawRect(m_state->getCanvasWidth()/ 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), m_state->getCanvasHeight(), m_brush_gameover);

	graphics::Brush br;
	br.outline_opacity = 0.0f;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(m_state->getCanvasWidth() / 2 - 2.70, m_state->getCanvasHeight() / 2 + 2.3, 1.0, "Game Over", br);
}


void Level::drawFinishScreen()
{
	graphics::Brush m_brush_finish;
	m_brush_finish.outline_opacity = 0.0f;
	m_brush_finish.texture = m_state->getFullAssetPath("finish.png");

	graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2, m_state->getCanvasWidth(), m_state->getCanvasHeight(), m_brush_finish);

}


void Level::draw()
{

	if (status == STATUS_GAMEOVER)
	{
		drawGameoverScreen();
	}
	else if (status == STATUS_FINISH)
	{
		drawFinishScreen();
	}
	else 
	{
		float w = m_state->getCanvasWidth();
		float h = m_state->getCanvasHeight();

		float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;  // h thesh x tou background upologizontas thn thesh tou paixth 
		float offset_y = m_state->m_global_offset_y / 2.0f + h / 2.0f;  // h thesh y tou background upologizontas thn thesh tou paixth 

		// draw background
		graphics::drawRect(offset_x, offset_y, 3.0f * w, 1.5f*w, m_brush_background);

		// draw player
		if (m_state->getPlayer()->isActive())
			m_state->getPlayer()->draw();

		// draw garbages
		for (auto& garbage : m_garbages) 
		{
			if (garbage->isActive())
				garbage->draw();
		}

		// draw blocks
		for (auto& block : m_blocks) 
		{
			if (block->isActive())
				block->draw();
		}

		// draw gameover blocks
		for (auto& block : m_gameover_blocks) 
		{
			if (block->isActive())
				block->draw();
		}


		if (m_name == "2.lvl") {

			// draw moving Blocks
			for (auto& block : m_moving_blocks)
			{
				if (block->isActive())
					block->draw();
			}

			// draw lever
			for (auto& lever : m_levers)
				lever->draw();

		}

		// draw life
		float player_life = m_state->getPlayer() ? m_state->getPlayer()->getRemainingLife() : 0.0f;

		graphics::Brush br;
		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f; //red
		br.fill_color[1] = 0.0f; //green 
		br.fill_color[2] = 0.0f; //blue
		br.texture = "";
		br.fill_secondary_color[0] = 0.0f;  // red
		br.fill_secondary_color[1] = 1.0f * (1.0f - player_life) + player_life * 0.7f; // green
		br.fill_secondary_color[2] = 0.0f; // blue
		br.gradient = true;
		br.gradient_dir_u = 1.0f;
		br.gradient_dir_v = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth() / 2 + 4 - ((1.0f - player_life) * 1 / 2), m_state->getCanvasHeight() / 2 - 2.3, player_life * 1, 0.5, br);


		// draw perigramma life
		br.outline_opacity = 1.0f;
		br.gradient = false;
		br.fill_opacity = 0.0f;
		graphics::drawRect(m_state->getCanvasWidth() / 2 + 4, m_state->getCanvasHeight() / 2 - 2.3, 1, 0.5, br);


		// draw energy text
		graphics::Brush br1;
		br1.outline_opacity = 0.0f;
		SETCOLOR(br1.fill_color, 1.0f, 1.0f, 1.0f);
		graphics::drawText(m_state->getCanvasWidth() / 2 + 3.60, m_state->getCanvasHeight() / 2 - 2.25, 0.25, "Energy", br1);

		// draw score text
		const std::string strNumber = std::to_string(m_state->getScore());
		graphics::drawText(m_state->getCanvasWidth() / 2 + 3.53, m_state->getCanvasHeight() / 2 - 2.65, 0.25, "Score: " + strNumber, br1);

	}
}


void Level::init()
{

	struct GarbageSpec {
		std::string name;
		double posX;
		double posY;
	};

	struct BlockSpec {
		std::string name;
		double posX;
		double posY;
	};

	if (m_name == "1.lvl") {

		// SCORE GARBAGES
		std::vector<GarbageSpec> scoreGarbageSpecs = { {"plant", 5.0, 6.0}, {"good1", -4.0, 5.0}, {"good2", 1.0, 2.0}, {"good3", -5.5, -1.0}, {"good4", 11.0, 5.0}, {"good1", 17.5, 6.0}, {"good3", 24.0, 0.0} };

		for (const auto& spec : scoreGarbageSpecs) {
			ScoreGarbage* newBlock = new ScoreGarbage(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_garbages.push_back(newBlock);
		}


		// ENERGY GARBAGES
		std::vector<GarbageSpec> energyGarbageSpecs = { {"toxic1", 0.0, 7.0}, {"toxic2", -4.5, -1.0}, {"toxic3", 13.0, 4.0}, {"toxic2", 14.0, 4.0}, {"toxic1", 10.0, 7.0}, {"energy", 12.0, 4.0} };

		for (const auto& spec : energyGarbageSpecs) {
			EnergyGarbage* newBlock = new EnergyGarbage(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_garbages.push_back(newBlock);
		}


		// BLOCKS
		std::vector<BlockSpec> blockSpecs = {
		{"eve", 24.0, 7.0},
		{"bridge", 3.0, 8.0}, {"bridge2", 7.0, 8.0}, {"bridge", 16.0, 8.0}, {"bridge2", 19.0, 8.0},
		{"block", -9.0, 8.0}, {"block", -8.0, 8.0}, {"block", -7.0, 8.0}, {"block", -6.0, 8.0}, {"block", -5.0, 8.0}, {"block", -4.0, 8.0}, {"block", -3.0, 8.0},
		{"block", -2.0, 8.0}, {"block", -1.0, 8.0}, {"block", -0.0, 8.0}, {"block", 1.0, 8.0}, {"block", 2.0, 8.0}, {"block", 8.0, 8.0}, {"block", 9.0, 8.0},
		{"block", 10.0, 8.0}, {"block", 11.0, 8.0}, {"block", 12.0, 8.0}, {"block", 13.0, 8.0}, {"block", 14.0, 8.0}, {"block", 15.0, 8.0}, {"block", 20.0, 8.0},
		{"block", 21.0, 8.0}, {"block", 22.0, 8.0}, {"block", 23.0, 8.0}, {"block", 24.0, 8.0}, {"block", -4.0, 6.0}, {"block", -1.0, 6.0}, {"block", 1.0, 4.0},
		{"block", -2.0, 2.0}, {"block", 14.0, 5.0}, {"block", 12.0, 5.0}, {"block", 13.0, 5.0}, {"block", 21.0, 6.0}, {"block", 24.0, 5.0}, {"block", 21.0, 3.0},
		{"block", 24.0, 1.0}, {"block", -5.5, 0.0}, {"block", -5.0, 0.0}, {"block", -4.5, 0.0}, {"block", -4.0, 0.0}, {"block", -3.5, 0.0}, {"block", 11.0, 6.0}
		};

		for (const auto& spec : blockSpecs) {
			Block* newBlock = new Block(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_blocks.push_back(newBlock);
		}


		// GAMEOVER BLOCKS 
		std::vector<BlockSpec> gameoverblockSpecs = { {"block", 2.0, 11.0}, {"block", 3.0, 11.0}, {"block", 4.0, 11.0}, {"block", 5.0, 11.0}, {"block", 6.0, 11.0}, {"block", 7.0, 11.0}, {"block", 8.0, 11.0},
		{"block", 15.0, 11.0}, {"block", 16.0, 11.0}, {"block", 17.0, 11.0}, {"block", 18.0, 11.0}, {"block", 19.0, 11.0}, {"block", 20.0, 11.0},
		{"block", -10.0, 11.0}, {"block", -11.0, 11.0}, {"block", -12.0, 11.0}, {"block", -13.0, 11.0}, {"block", -14.0, 11.0}, {"block", -15.0, 11.0}, {"block", -16.0, 11.0},
		{"block", 25.0, 11.0}, {"block", 26.0, 11.0}, {"block", 27.0, 11.0}, {"block", 28.0, 11.0} };

		for (const auto& spec : gameoverblockSpecs) {
			Block* newBlock = new Block(spec.name, spec.posX, spec.posY);
			newBlock->init();
			newBlock->setActive(0);
			m_gameover_blocks.push_back(newBlock);
		}

	}
	else if (m_name == "2.lvl")
	{
		// SCORE GARBAGES
		std::vector<GarbageSpec> scoreGarbageSpecs = { {"good1", -3.0, 5.0}, {"good2", 0.0, 5.0}, {"good3", -3.0, 1.0}, {"good4", 1.0, 1.0}, {"good3", 5.0, 0.0}, {"good1", 3.0, 1.0}, {"good2", 4.0, 4.0}, {"good3", 5.0, 7.0}, {"good4", 20.0, 6.0} };
		
		for (const auto& spec : scoreGarbageSpecs) {
			ScoreGarbage* newBlock = new ScoreGarbage(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_garbages.push_back(newBlock);
		}

		// ENERGY GARBAGES
		std::vector<GarbageSpec> energyGarbageSpecs = { {"toxic1", 4.0, 1.0}, {"toxic2", 3.0, 4.0}, {"toxic3", 17.0, 7.0} };

		for (const auto& spec : energyGarbageSpecs) {
			EnergyGarbage* newBlock = new EnergyGarbage(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_garbages.push_back(newBlock);
		}

		// BLOCKS
		std::vector<BlockSpec> blockSpecs = {
		{"eve", 24.0, 7.0},
		{"bridge", 2.0, 8.0}, {"bridge2", 7.0, 8.0},{"bridge", 14.0, 8.0}, {"bridge2", 21.0, 8.0},
		{"block", -6.0, 6.0}, {"block", -2.0, 6.0},{"block", -1.0, 6.0},{"block", 0.0, 6.0}, {"block", -3.0, 2.0}, {"block", -2.0, 3.0}, {"block", -1.0, 4.0},
		{"block", 1.0, 6.0}, {"block", 1.0, 5.0},{"block", 1.0, 4.0},{"block", 1.0, 3.0}, {"block", 1.0, 2.0}, {"block", 1.0, 1.0}, {"block", -9.0, 8.0},
		{"block", -8.0, 8.0}, {"block", -7.0, 8.0}, {"block", -6.0, 8.0}, {"block", -5.0, 8.0}, {"block", -4.0, 8.0}, {"block", -3.0, 8.0}, {"block", -2.0, 8.0},
		{"block", -1.0, 8.0}, {"block", -0.0, 8.0}, {"block", 1.0, 8.0}, {"block", 8.0, 8.0},{"block", 9.0, 8.0}, {"block", 10.0, 8.0}, {"block", 11.0, 8.0},
		{"block", 12.0, 8.0}, {"block", 13.0, 8.0},{"block", 22.0, 8.0}, {"block", 23.0, 8.0}, {"block", 24.0, 8.0}, {"block", 25.0, 8.0}, {"block", 25.0, -8.0},
		{"block", 25.0, -7.0}, {"block", 25.0, -6.0}, {"block", 25.0, -5.0},{"block", 25.0, -4.0}, {"block", 25.0, -3.0}, {"block", 25.0, -2.0}, {"block", 25.0, -1.0}, {"block", 25.0, 0.0}
		};

		for (const auto& spec : blockSpecs) {
			Block* newBlock = new Block(spec.name, spec.posX, spec.posY);
			newBlock->init();
			m_blocks.push_back(newBlock);
		}


		// GAMEOVER BLOCKS
		std::vector<BlockSpec> gameoverblockSpecs = { {"block", 1.0, 11.0},{"block", 2.0, 11.0}, {"block", 3.0, 11.0}, {"block", 4.0, 11.0}, {"block", 5.0, 11.0}, {"block", 6.0, 11.0}, {"block", 7.0, 11.0}, {"block", 8.0, 11.0},
		{"block", 13.0, 11.0},{"block", 14.0, 11.0},{"block", 15.0, 11.0}, {"block", 16.0, 11.0}, {"block", 17.0, 11.0}, {"block", 18.0, 11.0}, {"block", 19.0, 11.0}, {"block", 20.0, 11.0},{"block", 21.0, 11.0},{"block", 22.0, 11.0} ,
		{"block", 26.0, 11.0}, {"block", 27.0, 11.0}, {"block", 28.0, 11.0}, {"block", 29.0, 11.0},
		{"block", -10.0, 11.0}, {"block", -11.0, 11.0}, {"block", -12.0, 11.0}, {"block", -13.0, 11.0} };


		for (const auto& spec : gameoverblockSpecs) {
			Block* newBlock = new Block(spec.name, spec.posX, spec.posY);
			newBlock->init();
			newBlock->setActive(0);
			m_gameover_blocks.push_back(newBlock);
		}


		// MOVING BLOCKS
		struct MovingBlockSpec {
			std::string name;
			double posX;
			double posY;
			float mixX;
			float maxX;
		};

		std::vector<MovingBlockSpec> movingblockSpecs = { {"block", -5.0, 6.0, -3.0, -5.0}, {"block", 2.0, 2.0, 6.0, 2.0}, {"block", 5.0, 5.0, 6.0, 2.0}, {"block", 3.0, 8.0, 6.0, 3.0} };

		for (const auto& spec : movingblockSpecs) {
			MovingBlock* newBlock = new MovingBlock(spec.name, spec.posX, spec.posY, spec.maxX, spec.mixX);
			newBlock->init();
			m_moving_blocks.push_back(newBlock);
		}

		// LEVER
		Lever* lever = new Lever("lever", 10.0, 7.0);
		lever->setActive(0);
		lever->init();
		m_levers.push_back(lever);

	}

}

Level::Level(const std::string & name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background.png");

}

Level::~Level()
{

	for (auto block : m_blocks) {
		delete block;
	}

	for (auto block : m_moving_blocks) {
		delete block;
	}
	
	for (auto block : m_gameover_blocks) {
		delete block;
	}
	
	for (auto block : m_garbages) {
		delete block;
	}

	for (auto lever : m_levers) {
		delete lever;
	}

	m_blocks.clear();
	m_moving_blocks.clear();
	m_gameover_blocks.clear();
	m_garbages.clear();
	m_levers.clear();
}
