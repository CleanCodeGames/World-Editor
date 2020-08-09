#pragma once
#include"Define.h"

class Texture
{
private:
	IMG Load(const string file) {
		IMG texture;
		texture.loadFromFile("resources/images/" + file);
		texture.setSmooth(true);
		return texture;
	}

public:
	vector<IMG> Terrain;
	vector<IMG> Grass;
	vector<IMG> Border;
	vector<IMG> Destructible;
	vector<IMG> Decor;
	vector<IMG> Unit;
	vector<IMG> Flying;
	vector<IMG> Region;
	vector<IMG> Edit;

	Texture() {
		for (int i = 0; i < 50; i++) Terrain.push_back(Load("Map/terrain/terrain" + to_string(i) + ".png"));
		for (int i = 0; i < 3; i++) Border.push_back(Load("Map/border/border" + to_string(i) + ".png"));
		for (int i = 0; i < 8; i++) Destructible.push_back(Load("Map/tree/tree" + to_string(i) + ".png"));
		for (int i = 0; i < 10; i++) Grass.push_back(Load("Map/grass/grass" + to_string(i) + ".png"));
		for (int i = 0; i < 19; i++) Decor.push_back(Load("Map/decor/decor" + to_string(i) + ".png"));
		for (int i = 0; i < 1; i++) Unit.push_back(Load("Map/unit/unit" + to_string(i) + ".png"));
		for (int i = 0; i < 1; i++) Flying.push_back(Load("Map/flying/flying" + to_string(i) + ".png"));
		for (int i = 0; i < 1; i++) Region.push_back(Load("Map/region/region" + to_string(i) + ".png"));
		for (int i = 0; i < 3; i++) Edit.push_back(Load("UI/edit" + to_string(i) + ".png"));
	}

	~Texture(void) {
	}
};
