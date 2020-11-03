#pragma once
#include "System.h"

enum class TypeGameObject : uint {
	Abstract, 
	Empty, 
	Terrain, 
	Destructible,
	Decor,
	Unit, 
	Flying, 
	Region
};

class AbstractGameObject : public B
{
protected:
	TypeGameObject type;
	const uint m_layer; 
	const string m_name_id;
	Shape m_shape;

public: 

	AbstractGameObject(Shape shape, const uint layer, const string name) 
		: 
		m_shape(shape), 
		m_layer(layer), 
		m_name_id(name) {
		type = TypeGameObject::Abstract;
	}
	virtual const uint GetLayer() { return m_layer; }
	virtual const TypeGameObject GetType() { return type; }
	virtual const string GetNameID() { return m_name_id; }
	virtual Shape& GetShape() { return m_shape; }
	virtual void Update() = 0;
	virtual void Action() = 0;
	virtual void Draw() = 0;
	virtual ~AbstractGameObject() = default;
};

class GameObject : public AbstractGameObject
{
private:

public:

	GameObject(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Empty;
	}
	virtual void Update() override {
		// Do nothing...
	}
	virtual void Action() override {
		// Do nothing...
	}
	virtual void Draw() override { 
		wnd.draw(m_shape);
	}
	virtual ~GameObject() override {
	}
};

class GameObjectTerrain : public AbstractGameObject
{
private:

public:

	GameObjectTerrain(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Terrain;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectTerrain() override {
	}
};

class GameObjectDestructible : public AbstractGameObject
{
private:

public:

	GameObjectDestructible(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Destructible;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectDestructible() override {
	}
};

class GameObjectDecor : public AbstractGameObject
{
private:

public:

	GameObjectDecor(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Decor;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectDecor() override {
	}
};

class GameObjectUnit : public AbstractGameObject
{
private:

public:

	GameObjectUnit(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Unit;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectUnit() override {
	}
};

class GameObjectFlying : public AbstractGameObject
{
private:

public:

	GameObjectFlying(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Flying;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectFlying() override {
	}
};

class GameObjectRegion : public AbstractGameObject
{
private:

public:

	GameObjectRegion(Shape shape, const uint layer, const string name) : AbstractGameObject(shape, layer, name) {
		type = TypeGameObject::Region;
	}
	void Update() override {
		// Do nothing...
	}
	void Action() override {
		// Do nothing...
	}
	void Draw() override {
		wnd.draw(m_shape);
	}
	~GameObjectRegion() override {
	}
};