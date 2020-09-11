#pragma once
#include"Button.h"

class PanelObjectCreator : public B
{
private:

	bool m_is_focus_panel;

protected:

	Shape m_shape_panel;
	Shape m_shape_head;
	
	bool m_is_panel_active_for_user;

public:

	vector <unique_ptr<ButtonClickActive>> m_vec_button;

	PanelObjectCreator() {
		m_is_focus_panel = false;
		m_is_panel_active_for_user = false;
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
		if (m_shape_head.getGlobalBounds().contains(cur_p_wnd) || m_shape_panel.getGlobalBounds().contains(cur_p_wnd))
			m_is_focus_panel = true;
	}

	virtual void Action() {
		
	}

	virtual void Draw() {
		wnd.draw(m_shape_head);
		wnd.draw(m_shape_panel);
	}

	virtual ~PanelObjectCreator() {

	}
};