#pragma once
#include "System.h"

class TextBox : public B
{
private:

	static Shape m_shape_blink_line;

protected:

	Shape m_shape_box;
	sf::Text m_text;
	bool m_is_picked = false;

public:

	TextBox(v2f size) {

	}

	virtual void Update() {

	}

	virtual void Action() {
		if (IsMousePressed(sf::Mouse::Left)) {
			m_is_picked = false;
			if (IsContains(m_shape_box, cur_p_wnd)) {
				m_is_picked = true;
			}
		}
	}

	virtual void Draw() {
		wnd.draw(m_shape_box);
		wnd.draw(m_text);
	}

	sf::String GetData() {
		return m_text.getString();
	}

	// Вызывать во время перемещения панели курсором мышки
	// на которой находится текущий текстбокс
	void SetPosition(const v2f position) {
		m_shape_box.setPosition(position);
	}
};