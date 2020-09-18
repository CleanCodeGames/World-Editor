#pragma once
#include "System.h"

// Мигающая палочка для активного текстбокса
class BlinkLineOnTextBox : public B {
public:
	static short timer_blink;
	static Shape shape_blink_line;

	BlinkLineOnTextBox() {

	}

	static void Reset(v2f pos, int height) {
		shape_blink_line.setPosition(pos);
		shape_blink_line.setSize(v2f(1, height));
		shape_blink_line.setOrigin(shape_blink_line.getSize() / 2.f);
	}

	static void Update() {
		timer_blink < 60 ? timer_blink++ : timer_blink = 0;
		if (timer_blink > 30) shape_blink_line.setFillColor(Color(0, 0, 0, 255));
		else shape_blink_line.setFillColor(Color(0, 0, 0, 0));
	}
};

short BlinkLineOnTextBox::timer_blink;
Shape BlinkLineOnTextBox::shape_blink_line;

class TextBox : public B
{
protected:

	Shape m_shape_box;
	sf::Text m_text;
	bool m_is_picked = false;
	string m_value;
	const string DEFAULT_VALUE;
	const string m_data_id;

public:

	TextBox(v2f pos, v2f siz, const string data_id = "", const string value = "") : DEFAULT_VALUE(value), m_data_id(data_id) {
		m_shape_box = CreateShape(pos, siz, -1, Color(150, 150, 150), Color(40, 40, 40));
		m_text = CreateText(v2f(pos.x - (m_shape_box.getSize().x / 2) + 2, pos.y), siz.y - 8, DEFAULT_VALUE, font.erica_type, Color::Black);
		m_text.setOrigin(0, (siz.y - 8) / 2);
		m_value = DEFAULT_VALUE;
	}

	virtual void Update() {

	}

	virtual void Action() {
		static size_t d_char_position = 0;
		static sf::Text d_text;
		static string d_value;

		if (IsMousePressed(sf::Mouse::Left)) {
			m_is_picked = false;
			if (IsContains(m_shape_box, cur_p_wnd)) {
				d_value = "";
				d_text = m_text;
				d_text.setString(d_value);
				for (auto c : m_text.getString()) 
				{
					if (d_text.getPosition().x + d_text.getGlobalBounds().width < cur_p_wnd.x) {
						d_value += c;
						d_text.setString(d_value);
						BlinkLineOnTextBox::Reset(v2f(d_text.getPosition().x + d_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
					}
					else if (m_value == d_value) {
						BlinkLineOnTextBox::Reset(v2f(m_text.getPosition().x + m_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
						break;
					}
					else if (d_value == "") {
						BlinkLineOnTextBox::Reset(v2f(m_text.getPosition().x, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
						break;
					}
				}
				d_char_position = d_value.length();
				m_is_picked = true;
			}
		}

		if (m_is_picked) {
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) {
					if (event.text.unicode == 8) {
						if (!m_value.empty()) {
							if (d_char_position > 0) {
								m_value.erase(d_char_position - 1, 1);
								d_value.erase(d_char_position - 1, 1);
								d_text.setString(d_value);
								BlinkLineOnTextBox::Reset(v2f(d_text.getPosition().x + d_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
								d_char_position--;
							}
						}
						else {
							m_value = "";
						}
					}
					else { // Если вводим текст
						d_value += event.text.unicode;
						d_char_position++;
						d_text.setString(d_value);
						m_value.insert(d_value.length()-1, &d_value.back());
						BlinkLineOnTextBox::Reset(v2f(d_text.getPosition().x + d_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
					}
					m_text.setString(m_value);
				}
				else {
					BlinkLineOnTextBox::Reset(v2f(m_text.getPosition().x + m_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
				}
			}
		}
	}

	virtual void Draw() {
		wnd.draw(m_shape_box);
		wnd.draw(m_text);
		if (m_is_picked) 
			wnd.draw(BlinkLineOnTextBox::shape_blink_line);
	}

	void SetData(string data) {
		m_text.setString(data);
	}

	string GetData() {
		return m_text.getString();
	}

	// Вызывать во время перемещения панели курсором мышки
	// на которой находится текущий текстбокс
	void SetPosition(const v2f position) {
		m_shape_box.setPosition(position);
	}
};