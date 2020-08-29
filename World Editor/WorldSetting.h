#pragma once
#include "System.h"

enum class TypeController {
	RTC = 0u,
	RPG = 1u,
	PLATFORMER = 2u
};

class WorldSetting : public B
{
private:

	TypeController m_type_controller = TypeController::RTC;

public:

	WorldSetting() {

	}

	virtual void SetTypeController(TypeController tc) {
		m_type_controller = tc;
	}

	virtual TypeController GetTypeController() const {
		return m_type_controller;
	}

	virtual ~WorldSetting() {

	}
};