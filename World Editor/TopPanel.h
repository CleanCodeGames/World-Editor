#pragma once
#include "PanelObject.h"
// Тип режима редактирования карты
enum class TopPanelMode {
	PASTE = 0u,
	EDIT = 1u,
	SAVE = 2u,
	HELP = 3u,
	LOAD = 4u,
	EXIT = 5u
};

class BaseTopPanel : public B
{
protected:

	Shape m_shape;
	Shape m_shape_secelted_mode_ico_on_panel;
	Shape m_shape_selected_mode_ico_on_cursor;

	vector<unique_ptr<AbstractButton>> vec_button;
	vector<v2f> vec_button_pos;
	bool m_is_focus_panel = false;
	v2f m_panel_size;

public:

	using TPM = TopPanelMode;
	TPM m_top_panel_mode = TPM::PASTE;

	BaseTopPanel() {
	}

	virtual bool GetIsFocus() const {
		return m_is_focus_panel;
	}
	virtual vector<unique_ptr<AbstractButton>>& GetButtonVector() {
		return vec_button;
	}
	virtual void DrawSelectedObject() {
		wnd.draw(m_shape_secelted_mode_ico_on_panel);
		wnd.draw(m_shape_selected_mode_ico_on_cursor);
	}

	virtual void Draw() = 0;
	virtual void Update() {
		m_shape_selected_mode_ico_on_cursor.setPosition(cur_p);
	}

	virtual void Action() = 0;
	virtual const v2f GetPosition() {
		return m_shape.getPosition();
	}
	virtual const bool GetIsFocus() {
		if (m_shape.getGlobalBounds().contains(cur_p_wnd)) {
			m_is_focus_panel = true;
		}
		else m_is_focus_panel = false;

		return m_is_focus_panel;
	}
	virtual ~BaseTopPanel() {

	}
};

class TopPanel : public BaseTopPanel
{
public:

	TopPanel(v2f pos = v2f()) {
		m_panel_size = v2f(scr_W, 100);
		if (pos == v2f()) {
			m_shape = CreateShape(v2f(scr_W/2, 50), m_panel_size, -2, Color(80, 80, 80), Color(40, 40, 40));
			v2f button_size = v2f(48,48);
			v2f half_size = button_size / 2.f;
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x, 2 + half_size.y), button_size, texture.Edit[0]), "draw"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x, half_size.y + button_size.y + 2), button_size, texture.Edit[2]), "edit"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x + button_size.x + 2, half_size.y), button_size, texture.Edit[3]), "save"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x + button_size.x + 2, half_size.y + button_size.y + 2), button_size, texture.Edit[4]), "help"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(scr_W - half_size.x - 2, half_size.y + 2), button_size, texture.Edit[5]), "exit"));
			vec_button.push_back(make_unique<ButtonClickActive>(CreateShape(v2f(2 + half_size.x + (button_size.x * 2) + 2, half_size.y), button_size, texture.Edit[6]), "load"));
		}
		m_shape_secelted_mode_ico_on_panel = CreateShape(vec_button[0]->GetShape().getPosition(), vec_button[0]->GetShape().getSize(), -2, Color::Transparent, Color::Yellow);
		m_shape_selected_mode_ico_on_cursor = CreateShape(cur_p, v2f(vec_button[0]->GetShape().getSize()), texture.Edit[2]);
	}
	virtual void Update() override {
		BaseTopPanel::Update();
		static float alpha_rotation = 0;
		alpha_rotation += 0.015f * time;
		m_shape_selected_mode_ico_on_cursor.setRotation(cos(alpha_rotation) * 10);
	}

	virtual void Action() override {
		for (auto& button : vec_button) {
			if (button->Action()) {
				m_shape_secelted_mode_ico_on_panel.setPosition(button->GetShape().getPosition());
				if (button->GetActionId() == "draw") {
					m_shape_selected_mode_ico_on_cursor = Shape();
					m_top_panel_mode = TPM::PASTE;
				}
				else if (button->GetActionId() == "edit") {
					m_shape_selected_mode_ico_on_cursor = CreateShape(cur_p, v2f(button->GetShape().getSize()), texture.Edit[2]);
					m_top_panel_mode = TPM::EDIT;
				}
				else if (button->GetActionId() == "save") {
					m_shape_selected_mode_ico_on_cursor = Shape();
					m_top_panel_mode = TPM::SAVE;
				}
				else if (button->GetActionId() == "help") {
					m_shape_selected_mode_ico_on_cursor = Shape();
					m_top_panel_mode = TPM::HELP;
				}
				else if (button->GetActionId() == "exit") {
					m_shape_selected_mode_ico_on_cursor = Shape();
					m_top_panel_mode = TPM::EXIT;
					wnd.close();
				}
				else if (button->GetActionId() == "load") {
					m_shape_selected_mode_ico_on_cursor = Shape();
					m_top_panel_mode = TPM::LOAD;
				}
			}
		}
	}

	virtual void Draw() override {
		wnd.draw(m_shape);
		for (auto& button : vec_button) {
			button->Draw();
		}
		wnd.draw(m_shape_secelted_mode_ico_on_panel);
	}

	virtual ~TopPanel() {

	}
};

