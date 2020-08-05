#pragma once
#include "PanelObject.h"

class UI_Manager : public B
{
private:

	sf::Text m_text_current_layer;
	sf::Text m_text_cam_scale;
	sf::Text m_text_show_layer_all;
	sf::Text m_text_show_greed;
	sf::Text m_text_empty_cell;

	bool is_ctrl_pressed = false;
	
	double m_cam_zoom = 1.0;
	void ZoomCamWheel()
	{
		if (event.type == sf::Event::MouseWheelMoved) {
			v2f raznica = v2f(cur_p.x - (scr_W / 2.f), cur_p.y - (scr_H / 2.f));
			if (event.mouseWheel.delta == -1 && m_cam_zoom < 3.0) m_cam_zoom += 0.1;
			else if (m_cam_zoom > 0.5) m_cam_zoom -= 0.1;
			cam.setSize(scr_W * m_cam_zoom, scr_H * m_cam_zoom);
		}
	}

	string str_off_or_on(bool is)
	{
		if (is) return "ON";
		else return "OFF";
	}

public:

	unique_ptr<PanelObject> m_panel_object;
	bool is_show_layer_all = false;
	bool m_is_transparent_empty_cell = false;
	bool m_is_transparent_greed_cell = false;

	UI_Manager() {
		m_panel_object = std::move(make_unique<PanelObjectTerrain>());
		// Текст Текущий слой и ID-name выбранного объект
		m_text_current_layer = CreateText(v2f(), 20, "CURRENT LAYER: " + to_string(m_panel_object->GetSelectObjectLayerNum()) + " - " + m_panel_object->GetSelectedObjectNameID(), font.common, sf::Color::Yellow);
		m_text_current_layer.setPosition(v2f(m_text_current_layer.getGlobalBounds().width/2, m_text_current_layer.getGlobalBounds().height/2));
		// Текст зум камеры
		m_text_cam_scale = CreateText(v2f(), 20, "ZOOM CAMERA: " + to_string(round(m_cam_zoom * 10) / 10).substr(1,3), font.common, sf::Color::Yellow);
		m_text_cam_scale.setPosition(v2f(m_text_cam_scale.getGlobalBounds().width / 2, m_text_cam_scale.getGlobalBounds().height / 2 + 25));
		// Текст "Отобразить все слои"
		m_text_show_layer_all = CreateText(v2f(), 20, "SHOW ALL LAYERS: " + str_off_or_on(is_show_layer_all), font.common, sf::Color::Yellow);
		m_text_show_layer_all.setPosition(v2f(m_text_show_layer_all.getGlobalBounds().width / 2, m_text_show_layer_all.getGlobalBounds().height / 2 + 50));
		// Текст "Отобрвзить сетку"
		m_text_show_greed = CreateText(v2f(), 20, "SHOW GREED: " + str_off_or_on(m_is_transparent_greed_cell), font.common, sf::Color::Yellow);
		m_text_show_greed.setPosition(v2f(m_text_show_greed.getGlobalBounds().width / 2, m_text_show_greed.getGlobalBounds().height / 2 + 75));
		// Текст "Отобрвзить пустые клетки"
		m_text_empty_cell = CreateText(v2f(), 20, "SHOW EMPTY CELL: " + str_off_or_on(m_is_transparent_empty_cell), font.common, sf::Color::Yellow);
		m_text_empty_cell.setPosition(v2f(m_text_empty_cell.getGlobalBounds().width / 2, m_text_empty_cell.getGlobalBounds().height / 2 + 100));
	}

	virtual void Update() {
		wnd.setView(wnd.getDefaultView());
		m_panel_object->Update();
		wnd.setView(cam);
		m_panel_object->UpdateSelectedObject();
	}

	void Action() {
		wnd.setView(wnd.getDefaultView());

		m_panel_object->Action();

		v2f panel_position = m_panel_object->GetPosition();

		ZoomCamWheel();

		if (IsKeyPressed(Key::Num1))		m_panel_object = std::move(make_unique<PanelObjectTerrain>(panel_position));
		else if (IsKeyPressed(Key::Num2))	m_panel_object = std::move(make_unique<PanelObjectGrass>(panel_position));
		else if (IsKeyPressed(Key::Num3))	m_panel_object = std::move(make_unique<PanelObjectBorder>(panel_position));
		else if (IsKeyPressed(Key::Num4))	m_panel_object = std::move(make_unique<PanelObjectDestructible>(panel_position));
		else if (IsKeyPressed(Key::Num5))	m_panel_object = std::move(make_unique<PanelObjectDecor>(panel_position));
		else if (IsKeyPressed(Key::Num6))	m_panel_object = std::move(make_unique<PanelObjectUnit>(panel_position));
		else if (IsKeyPressed(Key::Num7))	m_panel_object = std::move(make_unique<PanelObjectFlying>(panel_position));
		else if (IsKeyPressed(Key::Num8))	m_panel_object = std::move(make_unique<PanelObjectRegion>(panel_position));

		if (IsKeyPressed(Key::LControl))	is_ctrl_pressed = true;
		if (IsKeyReleased(Key::LControl))	is_ctrl_pressed = false;
		if (IsKeyPressed(Key::A))			is_show_layer_all = !is_show_layer_all;

		// Текст Текущий слой и ID-name выбранного объект
		m_text_current_layer.setString("CURRENT LAYER: " + to_string(m_panel_object->GetSelectObjectLayerNum()) + " - " + m_panel_object->GetSelectedObjectNameID());
		// Текст зум камеры
		m_text_cam_scale.setString("ZOOM CAMERA: " + to_string(m_cam_zoom));
		// Текст "Отобразить все слои"
		m_text_show_layer_all.setString("SHOW ALL LAYERS: " + str_off_or_on(is_show_layer_all));
		// Текст "Отобрвзить сетку"
		m_text_show_greed.setString("SHOW GREED: " + str_off_or_on(m_is_transparent_greed_cell));
		// Текст "Отобразить пустые клетки"
		m_text_empty_cell.setString("SHOW EMPTY CELLS: " + str_off_or_on(m_is_transparent_empty_cell));
		wnd.setView(cam);
	}

	void Draw() {
		m_panel_object->DrawSelectedObject();
		wnd.setView(wnd.getDefaultView());
		m_panel_object->Draw();
		wnd.draw(m_text_current_layer);
		wnd.draw(m_text_cam_scale);
		wnd.draw(m_text_show_layer_all);
		wnd.draw(m_text_show_greed);
		wnd.draw(m_text_empty_cell);
		wnd.setView(cam);
	}

	~UI_Manager() {

	}
};