#pragma once
#include "System.h"

// Мигающая палочка для активного текстбокса
class TextBoxEditHelper : public B {
public:

	static short timer_blink;		// Время мигания палочки
	static Shape shape_blink_line;	// Сама палочка
	static size_t char_position;	// Номер символа в строке (позиция) для вставки при вводе текста
	static sf::Text text_left;		// Левая часть текста (до выделенного)
	static sf::Text text_middle;	// Средняя часть текста (выделенная)
	static string value_left;		// Содержание строки левой части текста (до выделенного)
	static string value_middle;		// Содержание строки средней части текста (выделенной)
	static bool is_left_mouse_pressed;	// Если клик мышки зажат при выделение участка текста

	TextBoxEditHelper() {
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

short	 TextBoxEditHelper::timer_blink;
Shape	 TextBoxEditHelper::shape_blink_line;
size_t	 TextBoxEditHelper::char_position;
sf::Text TextBoxEditHelper::text_left;
sf::Text TextBoxEditHelper::text_middle;
string	 TextBoxEditHelper::value_left;
string	 TextBoxEditHelper::value_middle;
bool	 TextBoxEditHelper::is_left_mouse_pressed = false;

enum class InputType {
	TEXT = 0u,
	INTEGER = 1u,
	REAL = 2u
};

class TextBox : public B
{
private:

	using TBEH = TextBoxEditHelper;
	InputType m_input_type;

protected:

	Shape m_shape_box;
	sf::Text m_text;
	bool m_is_picked = false;
	string m_value;
	const string m_value_default;
	const string m_data_id;

public:

	TextBox(v2f pos, v2f siz, const string data_id = "", const string value = "")
		:
		m_value_default(value),
		m_data_id(data_id)
	{
		m_shape_box = CreateShape(pos, siz, -1, Color(150, 150, 150), Color(40, 40, 40));
		m_text = CreateText(v2f(pos.x - (m_shape_box.getSize().x / 2) + 2, pos.y), siz.y - 8, m_value_default, font.erica_type, Color::Black);
		m_text.setOrigin(0, (siz.y - 8) / 2);
		m_value = m_value_default;
	}

	virtual void Update() {

	}

	virtual void Action() {
		if (IsMousePressed(sf::Mouse::Left)) {
			m_is_picked = false;
			if (IsContains(m_shape_box, cur_p_wnd)) {
				TBEH::value_left = "";
				TBEH::text_left = m_text;
				TBEH::text_left.setString(TBEH::value_left);

				for (auto c : m_value)
				{
					if (TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width < cur_p_wnd.x) {
						TBEH::value_left += c;
						TBEH::text_left.setString(TBEH::value_left);
						TBEH::Reset(v2f(TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
					}
					else if (m_value == TBEH::value_left) {
						TBEH::Reset(v2f(m_text.getPosition().x + m_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
						break;
					}
					else if (TBEH::value_left == "") {
						TBEH::Reset(v2f(m_text.getPosition().x, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
						break;
					}
				}
				TBEH::char_position = TBEH::value_left.length();
				TBEH::is_left_mouse_pressed = true;
				m_is_picked = true;
			}
		}

		if (m_is_picked) {
			if (IsMouseReleased(sf::Mouse::Left)) {
				TBEH::value_middle = "";
				TBEH::text_middle = m_text;
				TBEH::text_middle.setString(TBEH::value_middle);
				TBEH::is_left_mouse_pressed = false;
			}

			if (IsKeyPressed(Key::Left)) {
				if (!TBEH::value_left.empty()) {
					TBEH::value_left.pop_back();
					TBEH::char_position = TBEH::value_left.length();
					TBEH::text_left.setString(TBEH::value_left);
					TBEH::Reset(v2f(TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
				}
				else {
					TBEH::value_left = "";
				}
			}
			if (IsKeyPressed(Key::Right)) {
				if (TBEH::value_left != m_value) {
					TBEH::value_left.push_back(m_value[TBEH::char_position]);
					TBEH::text_left.setString(TBEH::value_left);
					TBEH::char_position = TBEH::value_left.length();
					TBEH::Reset(v2f(TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
				}
				else {
					TBEH::value_left = m_value;
				}
			}

			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128) {
					if (event.text.unicode == 8) {
						if (!m_value.empty()) {
							if (TBEH::char_position > 0) {
								m_value.erase(TBEH::char_position - 1, 1);
								TBEH::value_left.erase(TBEH::char_position - 1, 1);
								TBEH::text_left.setString(TBEH::value_left);
								TBEH::Reset(v2f(TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
								TBEH::char_position--;
							}
						}
						else {
							m_value = "";
						}
					}
					else { // Если вводим текст
						TBEH::value_left += event.text.unicode;
						TBEH::char_position++;
						TBEH::text_left.setString(TBEH::value_left);
						m_value.insert(TBEH::value_left.length() - 1, &TBEH::value_left.back());
						TBEH::Reset(v2f(TBEH::text_left.getPosition().x + TBEH::text_left.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
					}
					m_text.setString(m_value);
				}
				else {
					TBEH::Reset(v2f(m_text.getPosition().x + m_text.getGlobalBounds().width, m_shape_box.getPosition().y), m_shape_box.getSize().y - 6);
				}
			}
		}
	}

	virtual void Draw() {
		wnd.draw(m_shape_box);
		wnd.draw(m_text);
		if (m_is_picked)
			wnd.draw(TBEH::shape_blink_line);
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