#pragma once
#include"Button.h"
#include"TextBox.h"

class PanelObjectCreator : public B
{
private:

	bool m_is_focus_panel = false;
	bool m_is_focus_head = false;

	void ButtonPositionUpdate() {
		int count_b = 0;
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				if (count_b < m_but_nums) m_vec_button_pos[count_b] = v2f(m_head_position.x - (m_head_size.x / 2) + (50 * j) + 26, (m_head_position.y + 75) + (50 * i) - 36);
				count_b++;
			}
		}
	}

protected:

	int m_but_nums = 0;
	Shape m_shape_panel;
	Shape m_shape_head;
	v2f m_head_position;
	v2f m_panel_position;
	const v2f m_head_size = v2f(480, 24);
	const v2f m_panel_size = v2f(480, 720);
	bool m_is_panel_active_for_user;
	bool m_is_panel_move = false;
	vector<v2f> m_vec_button_pos;

public:

	vector <unique_ptr<ButtonClickActive>> m_vec_button;


	PanelObjectCreator(v2f pos = v2f()) {

		if (pos == v2f()) m_head_position = v2f(scr_W - m_head_size.x / 2, m_head_size.y / 2 + 98);
		else m_head_position = pos;

		m_shape_head = CreateShape(m_head_position, m_head_size, size_panel_out, color_panel_in, color_panel_out);

		m_is_panel_active_for_user = false;
		m_panel_position = v2f(m_head_position.x, m_head_position.y + ((m_panel_size.y + m_head_size.y) / 2) - 2);
		m_shape_panel = CreateShape(m_panel_position, m_panel_size, size_panel_out, color_panel_in, color_panel_out);
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

		m_is_focus_head = IsContains(m_shape_head, B::cur_p_wnd);
		m_is_focus_panel = (IsContains(m_shape_panel, B::cur_p_wnd) || m_is_focus_head);

		if (m_is_panel_move) {
			m_head_position = cur_p_wnd;

			if (m_head_position.x > (B::scr_W - m_head_size.x / 2)) m_head_position.x = (B::scr_W - m_head_size.x / 2);
			else if (m_head_position.x < m_head_size.x / 2) m_head_position.x = m_head_size.x / 2;

			if (m_head_position.y < m_head_size.y / 2 + 98) m_head_position.y = m_head_size.y / 2 + 98;
			else if (m_head_position.y > B::scr_H * 0.9) m_head_position.y = B::scr_H * 0.9;

			m_panel_position = v2f(m_head_position.x, m_head_position.y + (m_head_size.y / 2) + (m_panel_size.y / 2) - 2);
			m_shape_head.setPosition(m_head_position);
			m_shape_panel.setPosition(m_panel_position);
			ButtonPositionUpdate();
		}
	}

	virtual void Action() {

		if (IsMousePressed(sf::Mouse::Left) && m_is_focus_head) m_is_panel_move = true;
		if (IsMouseReleased(sf::Mouse::Left)) m_is_panel_move = false;

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