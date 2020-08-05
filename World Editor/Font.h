#pragma once
#include "Define.h"
// ������

struct Font
{
	// ���������� ���� �����, ������� ������ ���������� � ����� � �������� ���������
	sf::Font stix_two_text;
	sf::Font smeshariki;
	sf::Font super_mario;
	sf::Font erica_type;
	sf::Font common;

	Font() {
		stix_two_text.loadFromFile("Resources/Fonts/stix_two_text.otf");
		smeshariki.loadFromFile("Resources/Fonts/smeshariki_regular.ttf");
		super_mario.loadFromFile("Resources/Fonts/super_mario.ttf");
		erica_type.loadFromFile("Resources/Fonts/erica_type.ttf");
		common.loadFromFile("Resources/Fonts/common.ttf");
	}

	~Font() {

	}
};