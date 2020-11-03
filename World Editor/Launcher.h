#pragma once
#include "Map.h"

class Launcher : public B
{
private:

	vector<unique_ptr<AbstractButton>> m_vec_button;
	string m_action_name;
	sf::Text m_text_world_editor;
	unique_ptr<TextBox> m_text_box_map_size[2];
	sf::Text m_text_map_size;

public:

	v2i GetMapSize() {
		return v2i(std::stoi(m_text_box_map_size[0]->GetData()), std::stoi(m_text_box_map_size[1]->GetData()));
	}

	Launcher() {
		using Button = ButtonClickActiveWithText;
		m_text_world_editor = CreateText(v2f(scr_W / 2, scr_H / 2 - 72), 48, "World Editor", font.common, Color(100, 160, 100));
		m_text_map_size = CreateText(v2f(scr_W / 2, scr_H / 2 - 60), 18, "Map Size", font.erica_type, sf::Color::Green);
		m_text_box_map_size[0] = std::move(make_unique<TextBoxInt>(TextBoxInt(v2f(scr_W / 2 - 32, scr_H / 2 - 264), v2f(64, 24), "32", "32", 256)));
		m_text_box_map_size[1] = std::move(make_unique<TextBoxInt>(TextBoxInt(v2f(scr_W / 2 + 32, scr_H / 2 - 264), v2f(64, 24), "32", "32", 256)));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 - 25), v2f(80, 24), Color(100, 100, 100)), "New"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 - 0), v2f(80, 24), Color(100, 100, 100)), "Load"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(scr_W / 2, scr_H / 2 + 25), v2f(80, 24), Color(100, 100, 100)), "Quit" ));
	}

	bool IsAction() {
		for (auto& tb : m_text_box_map_size) {
			tb->Action();
		}
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
		for (auto& tb : m_text_box_map_size) {
			tb->Update();
			tb->Draw();
		}
		for (auto& button : m_vec_button) {
			button->Update();
			wnd.draw(m_text_world_editor);
			button->Draw();
		}
		wnd.draw(m_text_map_size);
	}

	~Launcher() {

	}
};