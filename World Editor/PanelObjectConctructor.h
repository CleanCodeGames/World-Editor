#pragma once
#include"Button.h"

class PanelObjectCreator : public B
{
private:

	bool m_is_focus_panel;

protected:

	Shape m_shape_panel;
	Shape m_shape_head;
	const v2f m_size_head = v2f(480, 24);
	const v2f m_size_panel = v2f(480, 480);
	bool m_is_panel_active_for_user;

public:

	vector <unique_ptr<ButtonClickActive>> m_vec_button;

	PanelObjectCreator() {
		m_is_focus_panel = false;
		m_is_panel_active_for_user = false;
		m_shape_panel = CreateShape(v2f(scr_W / 2, scr_H / 2), m_size_panel, size_panel_out, color_panel_in, color_panel_out);
		v2f head_position = v2f(m_shape_panel.getPosition().x, m_shape_panel.getPosition().y - ((m_size_panel.y / 2) + (m_size_head.y / 2)) + 1);
		m_shape_head = CreateShape(head_position, m_size_head, size_panel_out, color_panel_in, color_panel_out);
	}

	bool GetPanelActiveForUser() const {
		return m_is_panel_active_for_user;
	}

	bool GetIsFocus() const {
		return m_is_focus_panel;
	}

	void SetActivePanelForUser(bool is_active) {
		m_is_panel_active_for_user = is_active;
	}

	virtual void Update() {
		if (m_shape_head.getGlobalBounds().contains(cur_p_wnd) || m_shape_panel.getGlobalBounds().contains(cur_p_wnd)) {
			m_is_focus_panel = true;
		}
		else m_is_focus_panel = false;
	}

	virtual void Action() {
		for (auto& button : m_vec_button) {
			if (button->Action()) {
				if (button->GetActionId() == "") {
					// Action...
				}
			}
		}
	}

	virtual void Draw() {
		wnd.draw(m_shape_head);
		wnd.draw(m_shape_panel);
	}

	virtual ~PanelObjectCreator() {
	}
};