#pragma once
#include "GameObject.h"
#include "Button.h"

class AbstractPanel : public B
{
protected:

	Shape m_shape;
	vector<unique_ptr<AbstractButton>> vec_button;
	vector<v2f> vec_button_pos;
	bool m_is_focus_panel = false;

public:

	AbstractPanel() {
	}
	virtual void Update() = 0;
	virtual void Action() = 0;
	virtual void Draw() = 0;
	virtual void UpdateSelectedObject() = 0;
	virtual void DrawSelectedObject() = 0;
	virtual void ActionSelectedObject() = 0;
	
	virtual const v2f GetPosition() {
		return m_shape.getPosition();
	}
	virtual const bool GetIsFocus() {
		return m_is_focus_panel;
	}
	virtual ~AbstractPanel() {

	}
};

class BasePanelObject : public AbstractPanel
{
protected:
	using TGO = TypeGameObject;
	TGO type = TGO::Abstract;
	Shape m_shape_head;
	vector<unique_ptr<AbstractButton>> vec_button;
	vector<v2f> vec_button_pos;
	bool m_is_object_picked = false;
	bool m_is_focus_head = false;
	bool m_is_panel_move = false;
	int but_nums = 0;
	
	static unique_ptr<AbstractGameObject> m_selected_object;

public:
	

	BasePanelObject() {
	}

	virtual const v2f GetPosition() override {
		return m_shape_head.getPosition();
	}
	virtual unique_ptr<AbstractGameObject> GetSelectedObject() {
		
						type  = m_selected_object->GetType();
		const string&	name  = m_selected_object->GetNameID();
		const uint&		layer = m_selected_object->GetLayer();
		Shape			shape = m_selected_object->GetShape();

		switch (type)
		{
			case TGO::Abstract:		return make_unique<GameObject>(shape, layer, "Empty");		break;
			case TGO::Empty:		return make_unique<GameObject>(shape, layer, "Empty");		break;
			case TGO::Terrain:		return make_unique<GameObjectTerrain>(shape, layer, name);	break;
			case TGO::Destructible:	return make_unique<GameObjectDecor>(shape, layer, name);	break;
			case TGO::Decor:		return make_unique<GameObjectDecor>(shape, layer, name);	break;
			case TGO::Unit:			return make_unique<GameObjectUnit>(shape, layer, name);		break;
			case TGO::Flying:		return make_unique<GameObjectFlying>(shape, layer, name);	break;
			case TGO::Region:		return make_unique<GameObjectTerrain>(shape, layer, name);	break;
			default:				return make_unique<GameObject>(shape, layer, "Empty");		break;
		}
	}
	virtual const string GetSelectedObjectNameID() {
		return m_selected_object->GetNameID();
	}
	virtual const uint GetSelectObjectLayerNum() {
		return m_selected_object->GetLayer();
	}
	virtual ~BasePanelObject() {
	}
};
unique_ptr<AbstractGameObject> BasePanelObject::m_selected_object;

class PanelObject : public BasePanelObject
{
private:
	void ButtonPositionUpdate() {
		int count_b = 0;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				if (count_b < but_nums) vec_button_pos[count_b] = v2f(head_position.x - (head_size.x / 2) + (50 * j) + 26, (head_position.y + 75) + (50 * i) - 36);
				count_b++;
			}
		}
	}
protected:
	v2f head_position;
	v2f panel_position;
	const v2f head_size;
	const v2f panel_size;
	Shape shape_picked_border;
	int m_picked_button_num;

public:
	PanelObject(v2f pos = v2f()) : head_size(252, 24), panel_size(252, 504) {
		type = TypeGameObject::Abstract;
		m_picked_button_num = 0;
		if (pos == v2f()) head_position = v2f(scr_W - head_size.x / 2, head_size.y / 2 + 98);
		else head_position = pos;
		panel_position = v2f(head_position.x, head_position.y + (head_size.y / 2) + (panel_size.y / 2) - 2);
		m_shape_head = CreateShape(head_position, head_size, -2, Color(80, 80, 80), Color(40, 40, 40));
		m_shape = CreateShape(panel_position, panel_size, -2, Color(80, 80, 80), Color(40, 40, 40));
		m_is_object_picked = true;
		m_selected_object = std::move(make_unique<GameObjectTerrain>(CreateShape(cur_p, v2f(CELL_SIZE, CELL_SIZE), -1, Color(80, 80, 255, 100), Color(40, 40, 40)), Layer::terrain, btncmd_Terrain + to_string(0)));
		m_selected_object->GetShape().setScale(0.75, 0.75);
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 5; j++)
				vec_button_pos.push_back(v2f(head_position.x - (head_size.x / 2) + (50 * j) + 26, (head_position.y + 75) + (50 * i) - 36));
		ButtonPositionUpdate();
		shape_picked_border = CreateShape(vec_button_pos[0], v2f(48, 48), -2, Color::Transparent, Color::Yellow);
	}

	virtual void Update() override {
		m_is_focus_head = IsContains(m_shape_head, B::cur_p_wnd);
		m_is_focus_panel = (IsContains(m_shape, B::cur_p_wnd) || m_is_focus_head);
		if (m_is_panel_move) {
			head_position = cur_p_wnd;
			if (head_position.x > (B::scr_W - head_size.x / 2)) head_position.x = (B::scr_W - head_size.x / 2);
			else if (head_position.x < head_size.x / 2) head_position.x = head_size.x / 2;
			if (head_position.y < head_size.y / 2 + 98) head_position.y = head_size.y / 2 + 98;
			else if (head_position.y > B::scr_H * 0.9) head_position.y = B::scr_H * 0.9;
			panel_position = v2f(head_position.x, head_position.y + (head_size.y / 2) + (panel_size.y / 2) - 2);
			m_shape_head.setPosition(head_position);
			m_shape.setPosition(panel_position);
			ButtonPositionUpdate();
		}
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				vec_button[i]->Update();
				vec_button[i]->GetShape().setPosition(vec_button_pos[i]);
			}
		}
		shape_picked_border.setPosition(vec_button[m_picked_button_num]->GetShape().getPosition());
	}
	virtual void UpdateSelectedObject() override {
		if (m_is_object_picked) {
		}
		m_selected_object->GetShape().setPosition(cur_p);
	}
	virtual void Action() override {
		if (IsKeyPressed(Key::R)) m_selected_object->GetShape().rotate(90);
		if (IsKeyPressed(Key::I)) m_selected_object->GetShape().setScale(m_selected_object->GetShape().getScale().x * -1.f, 1);
		if (IsMousePressed(sf::Mouse::Left) && m_is_focus_head) m_is_panel_move = true;
		if (IsMouseReleased(sf::Mouse::Left)) m_is_panel_move = false;
	}
	virtual void ActionSelectedObject() override {
		if (m_is_object_picked) {
		}
	}
	virtual void Draw() override {
		wnd.draw(m_shape_head);
		wnd.draw(m_shape);
		for (auto& b : vec_button) {
			b->Draw();
			wnd.draw(shape_picked_border);
		}
	}
	virtual void DrawSelectedObject() override {
		if (m_is_object_picked && !m_is_focus_panel) {
			wnd.draw(m_selected_object->GetShape());
		}
	}
	virtual ~PanelObject() override {
	}
};
class PanelObjectTerrain : public PanelObject {
private:

public:

	PanelObjectTerrain(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Terrain;
		but_nums = texture.Terrain.size();
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Terrain[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Terrain + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectTerrain>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Terrain[0]), Layer::terrain, btncmd_Terrain + to_string(0)));
	}
	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Terrain + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectTerrain>(vec_button[i]->GetShape(), Layer::terrain, btncmd_Terrain + to_string(i)));
					}
				}
			}
		}
	}
	virtual ~PanelObjectTerrain() override{
	}
};
class PanelObjectGrass : public PanelObject {
private:

public:

	PanelObjectGrass(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Terrain;
		but_nums = 10;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Grass[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Grass + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectTerrain>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Grass[0]), Layer::grass, btncmd_Grass + to_string(0)));
	}
	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Grass + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectTerrain>(vec_button[i]->GetShape(), Layer::grass, btncmd_Grass + to_string(i)));
					}
				}
			}
		}
	}
	virtual ~PanelObjectGrass() override {
	}
};
class PanelObjectBorder : public PanelObject {
private:

public:

	PanelObjectBorder(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Terrain;
		but_nums = 3;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Border[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Border + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectTerrain>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Border[0]), Layer::border, btncmd_Border + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Border + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectTerrain>(vec_button[i]->GetShape(), Layer::border, btncmd_Border + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectBorder() override {
	}
};
class PanelObjectDestructible : public PanelObject {
private:

public:

	PanelObjectDestructible(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Destructible;
		but_nums = 8;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Destructible[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Destructible + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectDestructible>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Destructible[0]), Layer::decor, btncmd_Destructible + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Destructible + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectDestructible>(vec_button[i]->GetShape(), Layer::decor, btncmd_Destructible + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectDestructible() override {
	}
};
class PanelObjectDecor : public PanelObject {
private:

public:

	PanelObjectDecor(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Decor;
		but_nums = texture.Decor.size();
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Decor[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Decor + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectDecor>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Decor[0]), Layer::decor, btncmd_Decor + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Decor + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectDestructible>(vec_button[i]->GetShape(), Layer::decor, btncmd_Decor + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectDecor() override {
	}
};
class PanelObjectUnit : public PanelObject {
private:

public:

	PanelObjectUnit(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Unit;
		but_nums = 1;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Unit[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Unit + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectUnit>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Unit[0]), Layer::unit, btncmd_Unit + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Unit + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectUnit>(vec_button[i]->GetShape(), Layer::unit, btncmd_Unit + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectUnit() override {
	}
};
class PanelObjectFlying : public PanelObject {
private:

public:

	PanelObjectFlying(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Flying;
		but_nums = 1;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Flying[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Flying + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectFlying>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Flying[0]), Layer::flying, btncmd_Flying + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Flying + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectFlying>(vec_button[i]->GetShape(), Layer::flying, btncmd_Flying + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectFlying() override {
	}
};
class PanelObjectRegion : public PanelObject {
private:

public:

	PanelObjectRegion(v2f pos = v2f()) : PanelObject(pos)
	{
		type = TypeGameObject::Region;
		but_nums = 1;
		for (int i = 0; i < but_nums; i++) {
			const Shape& shape = CreateShape(vec_button_pos[i], v2f(CELL_SIZE, CELL_SIZE), texture.Region[i]);
			vec_button.push_back(make_unique<ButtonClickActive>(ButtonClickActive(shape, btncmd_Region + to_string(i))));
		}
		m_selected_object = std::move(make_unique<GameObjectRegion>(CreateShape(cur_p_wnd, v2f(CELL_SIZE, CELL_SIZE), texture.Region[0]), Layer::region, btncmd_Region + to_string(0)));
	}

	virtual void Action() override {
		PanelObject::Action();
		if (!vec_button.empty()) {
			for (int i = 0; i < but_nums; i++) {
				if (vec_button[i]->Action()) {
					m_picked_button_num = i;
					if (vec_button[i]->GetActionId() == btncmd_Region + to_string(i)) {
						m_selected_object = std::move(make_unique<GameObjectRegion>(vec_button[i]->GetShape(), Layer::region, btncmd_Region + to_string(i)));
					}
				}
			}
		}
	}

	virtual ~PanelObjectRegion() override {
	}
};