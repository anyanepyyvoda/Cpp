#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "garbage.h"
#include "util.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

GameState::GameState()
{

}


GameState::~GameState()
{
	for (auto level : levels)  // diagrafh twn levels
	{
		delete level;
		level = nullptr; // thetei ton pointer se null meta thn diagrafh
	}
	levels.clear();
	delete m_player; // diagrafh paixth
}


GameState* GameState::getInstance()
{
	if (!m_unique_instance) // an einai null dhmiourgei to monadiko instance
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance; 
}


void GameState::switchToNextLevel() {
	
	// tsekarei an uparxei epomeno level
	if (current_level_index < levels.size()-1) {

		current_level_index++;

		score = 0;
		getPlayer()->fillLife(); // epanafora life
		getPlayer()->m_pos_x = -6.0f; // metakinhsh tou paixth sthn arxikh thesh
		getPlayer()->m_pos_y = 7.0f;

		// allagh sto epomeno level
		levels[current_level_index]->init();
	}
}


bool GameState::init()
{
	graphics::playMusic(m_asset_path + "musicPressSpaceToStart.wav", 0.3f);

	m_current_level = new Level("1.lvl"); // orisma 1ou level
	levels.push_back(m_current_level); // prosthiki ston vector me ta levels
	
	m_next_level = new Level("2.lvl"); // orisma 2ou level
	levels.push_back(m_next_level);

	levels[0]->init(); 
	
	m_player = new Player("Player"); // arxikopoihsh paixth
	m_player->init(); 

	graphics::preloadBitmaps(getAssetDir()); // preload ola ta assets
	graphics::setFont(m_asset_path + "OpenSans-Bold.ttf"); 

	return true;
}


void GameState::drawStartScreen()
{
	graphics::Brush m_brush_start;
	m_brush_start.outline_opacity = 0.0f; // xwris perigramma
	m_brush_start.texture = getFullAssetPath("start.png");

	graphics::drawRect(m_canvas_width / 2, m_canvas_height / 2, m_canvas_width, m_canvas_height, m_brush_start);

	// ektypwsh mynhmatwn sthn arxikh othonh
	float p = 0.5f + fabs(cos(graphics::getGlobalTime() / 500.0f));
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	SETCOLOR(br.fill_color, p-0.9f, p - 0.9f, p - 0.9f);
	graphics::drawText(m_canvas_width / 2-1.2, m_canvas_height / 2-1.9, 0.3, "Press SPACE to start", br);

	SETCOLOR(br.fill_color, 0.0f, 0.0f, 0.0f);
	graphics::drawText(m_canvas_width / 2 - 1.9, m_canvas_height / 2-1.5, 0.18, "Goal: successfully deliver the last plant on earth to eve.", br);
	graphics::drawText(m_canvas_width / 2 - 3, m_canvas_height / 2-1.2, 0.18, "Collect recyclable waste, avoid toxic barrels and collect batteries to regain energy.", br);
	
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(m_canvas_width / 2 + 3.4, m_canvas_height / 2 + 2.60, 0.35, "H: Help", br);

}


void GameState::drawHelpScreen()
{
	graphics::Brush m_brush_help;
	m_brush_help.outline_opacity = 0.0f; // xwris perigramma
	m_brush_help.texture = getFullAssetPath("help.png");

	graphics::drawRect(m_canvas_width / 2, m_canvas_height / 2, m_canvas_width, m_canvas_height, m_brush_help);

}


void GameState::draw()
{
	if (m_unique_instance->status == STATUS_START) // an einai sthn arxikh othonh
	{
		drawStartScreen();
	}
	else if (m_unique_instance->status == STATUS_HELP)
	{
		drawHelpScreen();
	}
	else {
		
		if (!levels[current_level_index]) // an den uparxei to current level mhn kaneis tpt
			return;

		levels[current_level_index]->draw(); 
	}
	
}


void GameState::updateStartScreen(float dt)
{
	
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		status = STATUS_PLAYING;
		graphics::stopMusic();
	}

	if (graphics::getKeyState(graphics::SCANCODE_H))
	{
		status = STATUS_HELP;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


void GameState::updateHelpScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		status = STATUS_PLAYING;
		graphics::stopMusic();
	}

	if (graphics::getKeyState(graphics::SCANCODE_H))
	{
		status = STATUS_START;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}


void GameState::update(float dt)
{
	if (m_unique_instance->status == STATUS_START)
	{
		updateStartScreen(dt);
	}
	else if (m_unique_instance->status == STATUS_HELP)
	{
		updateHelpScreen(dt);
	}
	else {
		// Skip an update if a long delay is detected 
		// to avoid messing up the collision simulation
		if (dt > 500) // ms - an o xronos pou exei mesolavhsei apo to prohgoumeno kare einai panw apo miso deuterolepto mhn kaneis update
			return;

		// Avoid too quick updates
		float sleep_time = std::max(17.0f - dt, 0.0f);
		if (sleep_time > 0.0f)
		{
			std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
		}

		if (!levels[current_level_index])
			return;

		levels[current_level_index]->update(dt);

		m_debugging = graphics::getKeyState(graphics::SCANCODE_0);  // debug me 0

	}
}


std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}


std::string GameState::getAssetDir()
{
	return m_asset_path;
}


int GameState::getScore() const
{
	return score;
}


void GameState::addScore(int amount)
{
	score += amount;
}


GameState* GameState::m_unique_instance = nullptr;
