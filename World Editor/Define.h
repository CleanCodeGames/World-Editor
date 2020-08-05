#pragma once
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>
#include <fstream>
#include <memory>
#include <cmath>
#include <list>
#include <algorithm>
#include <thread>
#include <functional>
#include <SFML\Graphics.hpp>

#define PI  3.141590
#define DEG 57.29582
#define RAD 0.017453

class System;
typedef System B;

using std::cout;
using std::endl;
using std::to_string;
using std::cos;
using std::cosf;
using std::sin;
using std::sinf;
using std::atanf;
using std::make_shared;
using std::make_unique;
using std::list;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::ifstream;
using std::ofstream;
using std::thread;

typedef unsigned int uint;
typedef sf::Vector2i v2i;
typedef sf::Vector2f v2f;
typedef sf::Keyboard Key;
typedef sf::RectangleShape Shape;
typedef sf::Texture IMG;
typedef sf::Color Color;

std::ostream& operator << (std::ostream& out, const v2f& p)
{
	out << "X: " << p.x << " | Y: " << p.y;
	return out;
}

std::ostream& operator << (std::ostream& out, const v2i& p)
{
	out << "X: " << p.x << " | Y: " << p.y;
	return out;
}

const string& operator + (string s, int i)
{
	s += to_string(i);
	return s;
}

std::ostream& operator << (std::ostream& out, const Shape& s)
{
	out << "Position: " << s.getPosition() << endl;
	out << "Size: " << s.getSize() << endl;
	const auto& c = s.getFillColor();
	out << "Color: R = " << c.r << " G = " << c.g << " B = " << c.b << endl;
	return out;
}