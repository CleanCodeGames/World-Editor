#pragma once
#include "System.h"

class GameConstants : public B
{
public:

	static uint m_unit_move_speed_max;
	static uint m_unit_move_speed_min;
	static uint m_unit_agr_radius;
	static uint m_unit_agr_help_radius;

	static uint m_gold_start_quantity;
	static uint m_gold_max_quantity;

	static uint m_wood_start_quantity;
	static uint m_wood_max_quantity;

	GameConstants() {

	}

	~GameConstants() {

	}
};