#include "player.h"
#include "util.h"
#include <cmath>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f; // in sec

	movePlayer(dt);
	
	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;  // kentro - trexousa thesh tou paixth ston x
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y; // kentro - trexousa thesh tou paixth ston y
	
	GameObject::update(dt); // kalw tis methodous twn progonwn

}

void Player::draw()
{
	int sprite = (int)fmod(100.0f - m_pos_x * 3.0f, m_sprites.size());

	m_brush_player.texture = m_sprites[sprite];
	
	if (m_vx < 0 && m_vy == 0)  // otan paei aristera
	{
		int x = (int)fmod(100.0f - m_pos_x * 3.0f, m_sprites_left.size());
		m_brush_player.texture = m_sprites_left[x];
	}

	if (m_vy != 0) // otan phdaei
	{
		int y = (int)fmod(100.0f - m_pos_y * 3.0f, m_sprites_jump.size());
		m_brush_player.texture = m_sprites_jump[y];
	}

	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	
	if (m_state->m_debugging)
		debugDraw();
}


void Player::init()
{
	m_pos_x = -7.0f;
	m_pos_y = 6.0f;
	
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;

	m_sprites.push_back(m_state->getFullAssetPath("walle1.png"));
	m_sprites.push_back(m_state->getFullAssetPath("walle2.png"));
	
	m_sprites_left.push_back(m_state->getFullAssetPath("walle1left.png"));
	m_sprites_left.push_back(m_state->getFullAssetPath("walle2left.png"));

	m_sprites_jump.push_back(m_state->getFullAssetPath("jump.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump2.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump3.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump4.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump5.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump6.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump7.png"));
	m_sprites_jump.push_back(m_state->getFullAssetPath("jump8.png"));

	// Adjust width for finer collision detection
	m_width = 0.5f;
}


void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s,"(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	// Acceleration-based velocity
	float move = 0.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
		move -= 1.0f;

	if (graphics::getKeyState(graphics::SCANCODE_D))
		move = 1.0f;

	m_vx = std::min<float>(m_max_velocity, m_vx + delta_time * move * m_accel_horizontal); // orizontia taxythta tou paixth
	m_vx = std::max<float>(-m_max_velocity, m_vx);

	// friction
	m_vx -= 0.2f * m_vx / (0.1f + fabs(m_vx));

	// apply static friction threshold
	if (fabs(m_vx) < 0.01f)
		m_vx = 0.0f;

	// adjust horizontal position
	m_pos_x += m_vx * delta_time; // update orizontia thesh tou paixth (taxhthta * xronos)

	// jump only when not in flight:
	if (m_vy == 0.0f) 
		m_vy -= (graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f) * 0.02f;// not delta_time!! Burst 

	// add gravity
	m_vy += delta_time * m_gravity;

	// adjust vertical position
	m_pos_y += m_vy * delta_time;

}


void Player::drainLife(float amount)
{
	life = std::max<float>(0.0f, life - amount);
}


void Player::addLife(float amount)
{
	life = std::min<float>(1.0f, life + amount);
}


float Player::getRemainingLife() const
{
	return life;
}


void Player::fillLife()
{
	life = 1.0f;
}


Player::~Player()
{
	m_sprites.clear();
	m_sprites_left.clear();
	m_sprites_jump.clear();
}
