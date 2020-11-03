#pragma once
#include "Map.h"

class Launcher : public B
{
private:

	vector<unique_ptr<AbstractButton>> m_vec_button;
	string m_action_name;
	sf::Text m_text_world_editor;
	unique_ptr<TextBox> m_text_box_map_size[2];
	unique_ptr<TextBox> m_text_box_map_name;
	sf::Text m_text_map_size;
	sf::Text m_text_map_name;
	Shape m_shape_back_ground;

public:

	v2i GetMapSize() {
		return v2i(std::stoi(m_text_box_map_size[0]->GetData()), std::stoi(m_text_box_map_size[1]->GetData()));
	}

	Launcher() {
		using Button = ButtonClickActiveWithText;
		m_text_world_editor = CreateText(v2f(scr_W / 2, 72), 64, "  HEISENBERG\nWORLD  EDITOR", font.common, Color(170, 160, 100));
		m_text_map_size = CreateText(v2f(256, 36), 36, "Map Size", font.common, Color(50, 160, 255));
		m_text_box_map_size[0] = std::move(make_unique<TextBoxInt>(TextBoxInt(v2f(228, 64), v2f(48, 32), "32", "32", 256)));
		m_text_box_map_size[1] = std::move(make_unique<TextBoxInt>(TextBoxInt(v2f(284, 64), v2f(48, 32), "32", "32", 256)));

		m_text_map_name = CreateText(v2f(256, 100), 36, "Map Name", font.common, Color(50, 160, 255));
		m_text_box_map_name = std::move(make_unique<TextBox>(TextBox(v2f(256, 132), v2f(128, 32), "New map", "New map", 64)));

		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(256, 170), v2f(80, 24), Color(100, 100, 100)), "New"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(256, 197), v2f(80, 24), Color(100, 100, 100)), "Load"));
		m_vec_button.push_back(make_unique<Button>(CreateShape(v2f(256, 224), v2f(80, 24), Color(100, 100, 100)), "Quit" ));

		m_shape_back_ground = CreateShape(v2f(scr_W / 2, scr_H / 2), v2f(scr_W,scr_H), texture.BackGroundForMainMenu);
	}

	const string GetMapName() {
		return m_text_box_map_name->GetData();
	}

	bool IsAction() {
		cout << cur_p_wnd << endl;
		for (auto& tb : m_text_box_map_size) {
			tb->Action();
		}
		m_text_box_map_name->Action();
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
		wnd.draw(m_shape_back_ground);
		for (auto& tb : m_text_box_map_size) {
			tb->Update();
			tb->Draw();
		}
		wnd.draw(m_text_map_name);
		m_text_box_map_name->Update();
		m_text_box_map_name->Draw();
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