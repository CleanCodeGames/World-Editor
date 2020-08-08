#pragma once
#include "PanelObject.h"
// Тип режима редактирования карты
enum class TypeMode {
	DRAW = 0u,
	EDIT = 1u
};

// Тип всплывающих настроек для панели редактирования объекта
enum class TypeEditPickedObject {
	FLYING, // 
	UNIT,
	DECOR,
	WATER
};

class BasePanelSelectorTypeEdit : public B
{
protected:

	Shape m_shape;
	vector<unique_ptr<AbstractButton>> vec_button;
	vector<v2f> vec_button_pos;
	bool m_is_focus_panel = false;
	v2f m_panel_size;

public:

	using TM = TypeMode;
	TM m_type_mode = TM::DRAW;

	BasePanelSelectorTypeEdit() {
	}
	virtual void Update() = 0;
	virtual void Action() = 0;
	virtual void Draw() = 0;
	virtual const v2f GetPosition() {
		return m_shape.getPosition();
	}
	virtual const bool GetIsFocus() {
		return m_is_focus_panel;
	}
	virtual ~BasePanelSelectorTypeEdit() {

	}
};

class PanelSelectorTypeEdit : public BasePanelSelectorTypeEdit
{
public:

	PanelSelectorTypeEdit(v2f pos = v2f()) {
		m_panel_size = v2f(scr_W, 100);
		if (pos == v2f()) {
			m_shape = CreateShape(v2f(scr_W/2, 50), m_panel_size, -2, Color(80, 80, 80), Color(40, 40, 40));
			v2f button_size = v2f(48,48);
			v2f half_size = button_size / 2.f;
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x, 2 + half_size.y), button_size, texture.Edit[0]), "type_mode_draw"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x, half_size.y + button_size.y + 2), button_size, texture.Edit[1]), "type_mode_edit"));
		}
	}

	virtual void Update() override {
		
	}

	virtual void Action() override {

	}

	virtual void Draw() override {
		wnd.draw(m_shape);
		for (auto& button : vec_button) {
			button->Draw();
		}
	}

	virtual ~PanelSelectorTypeEdit() {

	}
};

