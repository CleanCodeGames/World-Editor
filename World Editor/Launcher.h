#pragma once
#include "Map.h"

class Launcher : public B
{
private:

	vector<unique_ptr<AbstractButton>> m_vec_button;
	string m_action_name;
	sf::Text m_text_world_editor;

public:

	Launcher() {
		using Button = ButtonClickActiveWithText;
		m_text_world_editor = CreateText(v2f(scr_W / 2, scr_H / 2 - 72), 48, "World Editor", font.common, Color(100, 160, 100));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 - 25), v2f(80, 24), Color(100, 100, 100)), "New"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 - 0), v2f(80, 24), Color(100, 100, 100)), "Load"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 + 25), v2f(80, 24), Color(100, 100, 100)), "Quit" ));
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
			wnd.draw(m_text_world_editor);
			button->Draw();
		}
	}

	~Launcher() {

	}
};