#pragma once
#include "PanelObject.h"

class BasePanelObjectCustom : public BasePanelObject
{
protected:

	v2f head_position;
	v2f panel_position;
	const v2f head_size;
	const v2f panel_size;
	Shape shape_picked_border;
	int m_picked_button_num;

	unique_ptr<ButtonClickActiveWithText> m_button_add_new_object;
	unique_ptr<ButtonClickActiveWithText> m_button_del_lst_object;
	vector<ButtonClickActive> m_vec_button;

public:

	BasePanelObjectCustom() {
		m_button_add_new_object = std::move(make_unique<ButtonClickActiveWithText>(CreateShape(panel_position, v2f(64, 16)), "Create new"));
	}

	virtual void DrawSelectedObject() override {
		if (m_is_object_picked && !m_is_focus_panel) {
			wnd.draw(m_selected_object->GetShape());
		}
	}
	virtual void ActionSelectedObject() override {
		if (m_is_object_picked) {
		}
	}
	virtual void UpdateSelectedObject() override {
		if (m_is_object_picked) {
		}
		m_selected_object->GetShape().setPosition(cur_p);
	}

	virtual void Update() override {
		m_button_add_new_object->Update();
	}

	virtual void Action() override {
		if (m_button_add_new_object->Action())
		{
			if (m_button_add_new_object->GetActionId() == "Create new") {
				cout << "Create\n";
			}
		}
	}

	virtual void Draw() override {
		m_button_add_new_object->Draw();
	}

	~BasePanelObjectCustom() {

	}
};