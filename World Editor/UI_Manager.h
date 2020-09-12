#pragma once
#include "PanelObject.h"
#include "TopPanel.h"
#include "PanelObjectConctructor.h"

class UI_Manager : public B
{
private:

	sf::Text m_text_current_layer;
	sf::Text m_text_cam_scale;
	sf::Text m_text_show_layer_all;
	sf::Text m_text_show_greed;
	sf::Text m_text_empty_cell;
	sf::Text m_text_fps_counter;

	float m_sec_timer = 0;
	int m_fps_counter = 0;
	bool is_ctrl_pressed = false;
	uint TEXT_SIZE = 20;
	double m_cam_zoom = 1.0;

	void ZoomCamWheel()
	{
		if (IsMousePressed(sf::Mouse::Middle)) {
			m_cam_zoom = 1.0;
			cam.setSize(scr_W * m_cam_zoom, scr_H * m_cam_zoom);
			// Текст зум камеры
			m_text_cam_scale.setString("ZOOM CAMERA: " + to_string(round(m_cam_zoom * 10) / 10).substr(0, 3));
		}
		else if (is_ctrl_pressed && event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta == -1 && m_cam_zoom < 3.0) m_cam_zoom += 0.1;
			else if (m_cam_zoom > 0.5) m_cam_zoom -= 0.1;
			cam.setSize(scr_W * m_cam_zoom, scr_H * m_cam_zoom);
			// Текст зум камеры
			m_text_cam_scale.setString("ZOOM CAMERA: " + to_string(round(m_cam_zoom * 10) / 10).substr(0, 3));
		}
	}

	string str_off_or_on(bool is)
	{
		if (is) return "ON";
		else return "OFF";
	}

public:

	unique_ptr<BaseTopPanel> m_panel_top;
	unique_ptr<BasePanelObject> m_panel_object;
	unique_ptr<PanelObjectCreator> m_panel_object_creator;
	bool is_show_layer_all = false;
	bool m_is_transparent_empty_cell = false;
	bool m_is_show_greed_cell = true;

	UI_Manager() {
		m_panel_object = std::move(make_unique<PanelObjectTerrain>());
		m_panel_top = std::move(make_unique<TopPanel>());
		m_panel_object_creator = std::move(make_unique<PanelObjectCreator>());

		// Текст Текущий слой и ID-name выбранного объект
		
		m_text_current_layer = CreateText(v2f(), TEXT_SIZE, "CURRENT LAYER: " + to_string(m_panel_object->GetSelectObjectLayerNum()) + " - " + m_panel_object->GetSelectedObjectNameID(), font.common, sf::Color::Yellow);
		m_text_current_layer.setPosition(v2f(m_text_current_layer.getGlobalBounds().width/2, m_text_current_layer.getGlobalBounds().height/2 + 100));
		// Текст зум камеры
		m_text_cam_scale = CreateText(v2f(), TEXT_SIZE, "ZOOM CAMERA: " + to_string(round(m_cam_zoom * 10) / 10).substr(0,3), font.common, sf::Color::Yellow);
		m_text_cam_scale.setPosition(v2f(m_text_cam_scale.getGlobalBounds().width / 2, m_text_cam_scale.getGlobalBounds().height / 2 + 125));
		// Текст "Отобразить все слои"
		m_text_show_layer_all = CreateText(v2f(), TEXT_SIZE, "SHOW ALL LAYERS: " + str_off_or_on(is_show_layer_all), font.common, sf::Color::Yellow);
		m_text_show_layer_all.setPosition(v2f(m_text_show_layer_all.getGlobalBounds().width / 2, m_text_show_layer_all.getGlobalBounds().height / 2 + 150));
		// Текст "Отобрвзить сетку"
		m_text_show_greed = CreateText(v2f(), TEXT_SIZE, "SHOW GREED: " + str_off_or_on(m_is_show_greed_cell), font.common, sf::Color::Yellow);
		m_text_show_greed.setPosition(v2f(m_text_show_greed.getGlobalBounds().width / 2, m_text_show_greed.getGlobalBounds().height / 2 + 175));
		// Текст "Отобрвзить пустые клетки"
		m_text_empty_cell = CreateText(v2f(), TEXT_SIZE, "SHOW EMPTY CELL: " + str_off_or_on(m_is_transparent_empty_cell), font.common, sf::Color::Yellow);
		m_text_empty_cell.setPosition(v2f(m_text_empty_cell.getGlobalBounds().width / 2, m_text_empty_cell.getGlobalBounds().height / 2 + 200));
		// Счётчик FPS
		m_text_fps_counter = CreateText(v2f(), TEXT_SIZE, "FPS: " + to_string(m_fps_counter), font.common, sf::Color::Yellow);
		m_text_fps_counter.setPosition(v2f(m_text_fps_counter.getGlobalBounds().width / 2, m_text_fps_counter.getGlobalBounds().height / 2 + 225));
	}

	// Возвращает true если курсор находится на какой либо панели интерфейса
	bool GetIsPanelFocus() const {
		return m_panel_object->GetIsFocus() 
			|| m_panel_object_creator->GetIsFocus() 
			|| m_panel_top->GetIsFocus();
	}

	virtual void Update() {
		if (m_sec_timer < 1000.f) {
			m_sec_timer += time;
			m_fps_counter++;
		}
		else {
			m_text_fps_counter.setString("FPS: " + to_string(m_fps_counter));
			m_sec_timer = 0;
			m_fps_counter = 0;
		}

		wnd.setView(wnd.getDefaultView());
		switch (m_panel_top->m_top_panel_mode)
		{
		case TopPanelMode::PASTE:
			m_panel_object->Update(); 
			break;
		case TopPanelMode::EDIT: m_panel_top->Update(); break;
		default: break;
		}
		// Экранная обработка
		wnd.setView(cam);
		m_panel_object->UpdateSelectedObject();
	}

	void Action() {
		wnd.setView(wnd.getDefaultView());
		m_panel_top->Action();
		static v2f panel_position; // Вспомогательная переменная позиции панели при смене типа

		switch (m_panel_top->m_top_panel_mode)
		{
			case TopPanelMode::PASTE:
				m_panel_object->Action();

				// Запоминаем позицию панели
				panel_position = m_panel_object->GetPosition();

				// Переключение панели расстановки объектов
				if (IsKeyPressed(Key::Num1))		m_panel_object = std::move(make_unique<PanelObjectTerrain>(panel_position));
				else if (IsKeyPressed(Key::Num2))	m_panel_object = std::move(make_unique<PanelObjectGrass>(panel_position));
				else if (IsKeyPressed(Key::Num3))	m_panel_object = std::move(make_unique<PanelObjectBorder>(panel_position));
				else if (IsKeyPressed(Key::Num4))	m_panel_object = std::move(make_unique<PanelObjectDestructible>(panel_position));
				else if (IsKeyPressed(Key::Num5))	m_panel_object = std::move(make_unique<PanelObjectDecor>(panel_position));
				else if (IsKeyPressed(Key::Num6))	m_panel_object = std::move(make_unique<PanelObjectUnit>(panel_position));
				else if (IsKeyPressed(Key::Num7))	m_panel_object = std::move(make_unique<PanelObjectFlying>(panel_position));
				else if (IsKeyPressed(Key::Num8))	m_panel_object = std::move(make_unique<PanelObjectRegion>(panel_position));
				
			break; 

			case TopPanelMode::EDIT:
				m_panel_object_creator->Action();
			break;

			case TopPanelMode::SAVE: break;
			case TopPanelMode::LOAD: break;
			case TopPanelMode::HELP: break;
			default: break;
		}

		ZoomCamWheel();

		if (IsKeyPressed(Key::LControl))	is_ctrl_pressed = true;
		if (IsKeyReleased(Key::LControl))	is_ctrl_pressed = false;

		if (IsKeyPressed(Key::A)) {
			is_show_layer_all = !is_show_layer_all;
			// Текст "Отобразить все слои"
			m_text_show_layer_all.setString("SHOW ALL LAYERS: " + str_off_or_on(is_show_layer_all));
		}
		// Текст Текущий слой и ID-name выбранного объект
		m_text_current_layer.setString("CURRENT LAYER: " + to_string(m_panel_object->GetSelectObjectLayerNum()) + " - " + m_panel_object->GetSelectedObjectNameID());
		// Текст "Отобрвзить сетку"
		m_text_show_greed.setString("SHOW GREED: " + str_off_or_on(m_is_show_greed_cell));
		// Текст "Отобразить пустые клетки"
		m_text_empty_cell.setString("SHOW EMPTY CELLS: " + str_off_or_on(m_is_transparent_empty_cell));
		wnd.setView(cam);
	}

	void Draw() {
		// =================================================#
		// Отображение элементов внутри мировых координатов #
		// =================================================#

		switch (m_panel_top->m_top_panel_mode)
		{
		case TopPanelMode::PASTE:

			// Отображение выбранного элемента 
			// с панели объектов под курсором
			m_panel_object->DrawSelectedObject(); 

			break;
		case TopPanelMode::EDIT: break;
		case TopPanelMode::SAVE: break;
		case TopPanelMode::LOAD: break;
		case TopPanelMode::HELP: break;
		default: break;
		}

		// =================================#
		// Отображение элементов интерфейса #
		//    внутри экранных координатов	#
		// =================================#

		wnd.setView(wnd.getDefaultView());
		switch (m_panel_top->m_top_panel_mode)
		{
		// Отображение панели расстановки игровых объектов
			case TopPanelMode::PASTE:
				m_panel_object->Draw();
				break;
		// Отображение панели добавления нестандартных объектов
			case TopPanelMode::EDIT :
				m_panel_object_creator->Draw();
				break;
		// Отображение панели сохранения игрового мира
			case TopPanelMode::SAVE:

				break;
		// Отображение панели для загрузки карты
			case TopPanelMode::LOAD:

				break;
		// Отображение панели вспомогательной литературы редактора
			case TopPanelMode::HELP:
				//...
				break;
			default: break;
		}
		m_panel_top->Draw();
		wnd.draw(m_text_current_layer);
		wnd.draw(m_text_cam_scale);
		wnd.draw(m_text_show_layer_all);
		wnd.draw(m_text_show_greed);
		wnd.draw(m_text_empty_cell);
		wnd.draw(m_text_fps_counter);
		wnd.setView(cam);
	}

	~UI_Manager() {

	}
};