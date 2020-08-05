#pragma once
#include "PanelObject.h"

class UI_Manager : public B
{
private:

	sf::Text m_text_current_layer;

public:

	unique_ptr<PanelObject> m_panel_object;

	UI_Manager() {
		m_panel_object = std::move(make_unique<PanelObjectTerrain>());
		m_text_current_layer = CreateText(v2f(), 20, "Current layer " + to_string(m_panel_object->GetSelectObjectLayerNum()) + ": " + m_panel_object->GetSelectedObjectNameID(), font.common, sf::Color::Yellow);
		m_text_current_layer.setPosition(v2f(m_text_current_layer.getGlobalBounds().width/2, m_text_current_layer.getGlobalBounds().height/2));
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

		if (IsKeyPressed(Key::Num1))		m_panel_object = std::move(make_unique<PanelObjectTerrain>(panel_position));
		else if (IsKeyPressed(Key::Num2))	m_panel_object = std::move(make_unique<PanelObjectGrass>(panel_position));
		else if (IsKeyPressed(Key::Num3))	m_panel_object = std::move(make_unique<PanelObjectBorder>(panel_position));
		else if (IsKeyPressed(Key::Num4))	m_panel_object = std::move(make_unique<PanelObjectDestructible>(panel_position));
		else if (IsKeyPressed(Key::Num5))	m_panel_object = std::move(make_unique<PanelObjectDecor>(panel_position));
		else if (IsKeyPressed(Key::Num6))	m_panel_object = std::move(make_unique<PanelObjectUnit>(panel_position));
		else if (IsKeyPressed(Key::Num7))	m_panel_object = std::move(make_unique<PanelObjectFlying>(panel_position));
		else if (IsKeyPressed(Key::Num8))	m_panel_object = std::move(make_unique<PanelObjectRegion>(panel_position));
		m_text_current_layer.setString("Current layer " + to_string(m_panel_object->GetSelectObjectLayerNum()) + ": " + m_panel_object->GetSelectedObjectNameID());
		wnd.setView(cam);
	}

	void Draw() {
		m_panel_object->DrawSelectedObject();
		wnd.setView(wnd.getDefaultView());
		m_panel_object->Draw();
		wnd.draw(m_text_current_layer);
		wnd.setView(cam);
	}

	~UI_Manager() {

	}
};