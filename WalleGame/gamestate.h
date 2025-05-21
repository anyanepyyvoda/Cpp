#pragma once

#include <string>
#include <vector>

class GameState
{

private:
	static GameState* m_unique_instance; // instance ths GameState 

	const std::string m_asset_path = "assets\\"; // thesh twn assets

	const float m_canvas_width  = 10.0f; // diastaseis tou camva - width
	const float m_canvas_height = 6.0f; // height
	
	class Level* m_current_level = 0; // current level
	class Level* m_next_level = 0; // next level
	class Player* m_player = 0; // player

	int current_level_index = 0; // keeps track of the current level
	std::vector<Level*> levels; // vector to store all levels
	
	typedef enum { STATUS_START, STATUS_PLAYING, STATUS_HELP } status_t;
	status_t status = STATUS_START;

	void updateStartScreen(float dt);
	void drawStartScreen();
	void updateHelpScreen(float dt);
	void drawHelpScreen();

	GameState(); // Constructor tou GameState

public:
	float m_global_offset_x = 0.0f; // pou exei ftasei o paixths se sxesh me to kentro ekkinhshs tou paixnidiou
	float m_global_offset_y = 0.0f;
	
	bool m_debugging = false; // gia debug mode

	int score = 0; // krataei to score tou paixth

public:
	~GameState(); // Destructor
	static GameState* getInstance(); // dhmiourgei to instance kai kalei ton constructor

	bool init();
	void draw();
	void update(float dt);

	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();

	float getCanvasWidth() const { return m_canvas_width; }
	float getCanvasHeight() const { return m_canvas_height; }

	class Player* getPlayer() { return m_player; }

	void switchToNextLevel(); // ennalagh se epomeno level

	int getScore() const; 
	void addScore(int amount); // prosthetei score
};
