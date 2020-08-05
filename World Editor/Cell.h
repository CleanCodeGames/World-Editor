#pragma once
#include "GameObject.h"

// Слой выборки и вставки объектов
struct Layer
{
	Layer() {
		get = terrain;
	}

	static uint get;
	void set(const uint& set) {
		get = set;
	}
	static constexpr uint terrain	= 0u;
	static constexpr uint grass		= 1u;
	static constexpr uint border	= 2u;
	static constexpr uint decor		= 3u;
	static constexpr uint unit		= 4u;
	static constexpr uint flying	= 5u;
	static constexpr uint region	= 6u;
	static constexpr uint ALL		= 7u;
};
uint Layer::get;

// Ячейка игровой карты
class Cell : public B
{
private:

	const uint m_layer;
	const v2f m_position;

public:

	Cell(const v2f position, uint layer) : m_layer(layer), m_position(position) {}
	virtual const v2f& GetPosition() { return m_position; }
	virtual const unsigned int GetNumLayer() { return m_layer; }

	virtual ~Cell() { 
	}
};
