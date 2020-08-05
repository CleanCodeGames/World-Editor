#pragma once
#include "Map.h"

class Launcher : public B
{
private:

	vector<unique_ptr<AbstractButton>> m_vec_button;
	string m_action_name;

public:

	Launcher() {
		using Button = ButtonClickActiveWithText;
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 - 16), v2f(80, 24), Color(100, 100, 100)), "Start"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 + 16), v2f(80, 24), Color(100, 100, 100)), "Quit" ));
	}

	bool IsAction() {
		for (auto& button : m_vec_button) {
			if (button->Action()) {
				m_action_name = button->GetActionId();
				return true;
			}
		}
		return false;
	}

	const string& GetAct() {
		return m_action_name;
	}

	void Draw() {
		for (auto& button : m_vec_button) {
			button->Update();
			button->Draw();
		}
	}

	~Launcher() {

	}
};