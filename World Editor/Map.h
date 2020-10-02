#pragma once
#include "Cell.h"
#include "UI_Manager.h"
#include "WorldSetting.h"

class Map : public B
{
private:
	
	Layer m_layer;			// ����� ����
	string m_name;			// �������� �����
	v2i m_size;				// ������ �����
	v2f m_center_map;		// ����� �����
	v2f m_click_position;	// ������� ����� �����

	vector<Cell> m_vec_cell[Layer::ALL];								// ������� ����� �����
	vector<unique_ptr<AbstractGameObject>> m_vec_object[Layer::ALL];	// ������� �� �����

	bool m_is_created = false;				// ���� �� ����� �������
	bool m_is_mouse_left_pressed = false;	// ���� ��� ������
	bool m_is_mouse_right_pressed = false;	// ���� ��� ������

	UI_Manager m_ui_manager;	// �������� ����������
	// ������ �� ��������������� ����� ������� ������ ���������
	const TopPanelMode& m_top_panel_mode = m_ui_manager.m_panel_top->m_top_panel_mode;

public:

	Shape m_shape_pick_cell;
	

	Map(v2i size, string name) {
		Create(size, name);
	}
	Map() {
		m_size = v2i(32, 32);
		m_name = "NoName";
	}

	bool SaveToFile(string fnime) {
		const string path = "maps\\";
		const string extension = ".wemap";
		ofstream file;
		file.open(path + fnime + extension);
		if (file.is_open()) 
		{
			file << "0" << "\n";
			for (int i = 0; i < m_layer.ALL; i++) {
				file << "START_LAYER " << i << " : ";
				for (auto& object : m_vec_object[i]) {
					file << " { " << object->GetNameID() << " } ";
					file << " { " << int(object->GetType()) << " } ";
					file << " : ";
				}
				file << "END_LAYER " << i << " :" << "\n";
			}
			file.close();
			return true;
		}
		else return false;
	}
	bool LoadFromFile(string fname) {
		return true;
	}

	void Create(v2i size = v2i(32,32), string name = "NoName")
	{
		m_size = size;
		m_name = name;

		m_shape_pick_cell = CreateShape(v2f(0, 0), v2f(CELL_SIZE, CELL_SIZE), size_cell_out, Color::Transparent, Color::Green);

		// ���� ����� ��� ���� ������� �� ������ �
		if (m_is_created) {
			for (int i = 0; i < Layer::ALL; i++) {
				m_vec_cell[i].clear();
			}
			m_vec_object[m_layer.get].clear();
		}
		// ��������� ������� ������� ��������
		for (int i = 0; i < m_size.y; i++) {
			for (int j = 0; j < m_size.x; j++) {
				for (int x = 0; x < Layer::ALL; x++) {
					m_vec_cell[x].push_back(Cell(v2f(j, i) * CELL_SIZE, x));
					m_vec_object[x].push_back(make_unique<GameObject>(CreateShape(m_vec_cell[x].back().GetPosition(), 
						v2f(CELL_SIZE, CELL_SIZE), size_cell_out, color_cell_in, color_cell_out), x, "Empty"));
				}
			}
		}
		// ������ ������ � ����� �����
		m_center_map = v2f(m_size.x, m_size.y) * CELL_SIZE / 2.f;
		cam_p = m_center_map;
		cam.setCenter(cam_p);
		wnd.setView(cam);
		m_is_created = true; // ����� ������� �������
	}

	// ������� ���� ������� � ��������������
	Layer GetFocusLayer() {
		return m_layer;
	}
	// ������ ���� ������� � ��������������
	void SetFocusLayer(const uint layer) {
		m_layer.get = layer;
	}
	// ������� ������ ��� ������� �������
	const v2f& GetFocusCellPosition() {
		return m_shape_pick_cell.getPosition();
	}

	// �������� ������ �� ������� ������ ��������
	const bool IsFocusObject(unique_ptr<AbstractGameObject>& obj) {
		return obj->GetShape().getGlobalBounds().contains(cur_p);
	}

	// �������� ������ �� ������ �����
	const bool IsFocusCell(Cell& cell) {
		return sf::FloatRect(cell.GetPosition() - v2f(CELL_SIZE / 2, CELL_SIZE / 2), v2f(CELL_SIZE, CELL_SIZE)).contains(cur_p);
	}

	// ������������ ������ �����
	void ActTransperentEmptyCell() {
		if (IsKeyPressed(Key::T)) { 
			m_ui_manager.m_is_transparent_empty_cell = !m_ui_manager.m_is_transparent_empty_cell;
			if (m_ui_manager.m_is_transparent_empty_cell) color_cell_in = Color::Transparent;
			else color_cell_in = Color(100, 100, 100, 100);

			for (int i = 0; i < m_layer.ALL; i++) {
				for (auto& object : m_vec_object[i]) {
					if (object->GetNameID() == "Empty") {
						object->GetShape().setFillColor(color_cell_in);
					}
				}
			}
		}
	}

	// ������������ �����
	void ActTransparentGreedCell() {
		if (IsKeyPressed(Key::G)) { 
			m_ui_manager.m_is_show_greed_cell = !m_ui_manager.m_is_show_greed_cell;
			if (m_ui_manager.m_is_show_greed_cell) color_cell_out = Color(45, 70, 45);
			else color_cell_out = Color::Transparent;
			for (int i = 0; i < m_layer.ALL; i++) {
				for (auto& object : m_vec_object[i]) {
					object->GetShape().setOutlineColor(color_cell_out);
				}
			}
		}
	}

	// ����� ���� ��������������
	void  ActSelectionLayer() {
		if (IsKeyPressed(Key::Num1)) m_layer.set(Layer::terrain);
		if (IsKeyPressed(Key::Num2)) m_layer.set(Layer::grass);
		if (IsKeyPressed(Key::Num3)) m_layer.set(Layer::border);
		if (IsKeyPressed(Key::Num4)) m_layer.set(Layer::decor);
		if (IsKeyPressed(Key::Num5)) m_layer.set(Layer::decor);
		if (IsKeyPressed(Key::Num6)) m_layer.set(Layer::unit);
		if (IsKeyPressed(Key::Num7)) m_layer.set(Layer::flying);
		if (IsKeyPressed(Key::Num8)) m_layer.set(Layer::region);
	}

	// ���������� ������� ������
	void ActionHotKeyFastFunction() {
		ActSelectionLayer();
		ActTransperentEmptyCell();
		ActTransparentGreedCell();
	}

	void Action() {
		m_ui_manager.Action();
		ActionHotKeyFastFunction();

		for (auto& cell : m_vec_cell[m_layer.get]) {
			if (IsFocusCell(cell)) m_shape_pick_cell.setPosition(cell.GetPosition());
		}

		switch (m_top_panel_mode)
		{
		case TopPanelMode::PASTE:

			if (IsMousePressed(sf::Mouse::Left) && !m_ui_manager.GetIsPanelFocus()) {
				m_is_mouse_left_pressed = true;
			}

			if (IsMousePressed(sf::Mouse::Right) 
				&& !m_ui_manager.m_panel_object->GetIsFocus() 
				&& !m_ui_manager.m_panel_top->GetIsFocus()) {
				m_is_mouse_right_pressed = true;
			}

			if (IsMouseReleased(sf::Mouse::Left))	m_is_mouse_left_pressed = false;
			if (IsMouseReleased(sf::Mouse::Right))	m_is_mouse_right_pressed = false;
			break;

		case TopPanelMode::EDIT:

			m_is_mouse_left_pressed = false;
			m_is_mouse_right_pressed = false;

			if (IsMousePressed(sf::Mouse::Left)) {
				for (auto& object: m_vec_object[m_layer.get]) {
					if (object->GetShape().getGlobalBounds().contains(cur_p)) {
						cout << object->GetNameID() << endl;
						cout << "Layer: " << object->GetLayer() << endl;
						break;
					}
				}
			}
			break;
		case TopPanelMode::HELP: break;
		case TopPanelMode::SAVE:
			if (SaveToFile("test")) cout << "File Save Ok\n";
			else cout << "File no save\n";
			break;
		case TopPanelMode::LOAD: break;
		case TopPanelMode::EXIT: break;
		default: break;
		}

		if (IsKeyPressed(Key::Q)) system("cls");
	}

	// ������� � �������� ��������
	void UpdPasteAndRemoveObject() {
		if (m_top_panel_mode == TopPanelMode::PASTE) {
			for (auto& object : m_vec_object[m_layer.get]) {														// ���������� �� ���� ������� �������� ����
				if (IsFocusObject(object)) {																		// ���� ������ �������� � ������
					if (object->GetLayer() == m_ui_manager.m_panel_object->GetSelectObjectLayerNum()) {				// ���� ����� ���� ������� � ������ � ���������� ������� �� ������ ����������
						if (m_is_mouse_left_pressed) {																// ���� ����� ���� ����� �����
							if (object->GetNameID() != m_ui_manager.m_panel_object->GetSelectedObjectNameID()) {	// ���� ���������� ������������� �� ����������
								object = std::move(m_ui_manager.m_panel_object->GetSelectedObject());				// ������ ������ � ������ �� ��������� ������ �� ������
								object->GetShape().setPosition(GetFocusCellPosition());								// ������ ������ ����� � ������� ������
							}
						}
						else if (m_is_mouse_right_pressed) {
							if (object->GetNameID() != "Empty") {
								object = std::move(make_unique<GameObject>(
									CreateShape(GetFocusCellPosition(), v2f(CELL_SIZE, CELL_SIZE), size_cell_out, color_cell_in, color_cell_out), m_layer.get, "Empty"));
							}
						}
					}
				}
			}
		}
	}

	void Update() {
		m_ui_manager.Update();
		switch (m_top_panel_mode)
		{
		case TopPanelMode::PASTE: UpdPasteAndRemoveObject(); break;
		case TopPanelMode::EDIT: break;
		case TopPanelMode::HELP: break;
		case TopPanelMode::SAVE: break;
		case TopPanelMode::LOAD: break;
		case TopPanelMode::EXIT: break;
		default: break;
		}
	}

	void Draw() {
		if (m_ui_manager.is_show_layer_all == true) { // ���� ���������� ��� ����
			for (int i = 0; i < m_layer.ALL; i++) {
				for (auto& object : m_vec_object[i]) {
					if (GetVisible().intersects(object->GetShape().getGlobalBounds())) {
						if (i == 0 && object->GetNameID() == "Empty") {
							object->Draw();
						}
						else if (object->GetNameID() != "Empty") {
							object->Draw();
						}
					}
				}
			}
		}
		else {
			for (auto& object : m_vec_object[m_layer.get])
				if (GetVisible().intersects(object->GetShape().getGlobalBounds())) {
					object->Draw();
				}
		}
		wnd.draw(m_shape_pick_cell);
		m_ui_manager.Draw();
	}

	~Map() {

	}
};