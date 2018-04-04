#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP

#include <SFML/OpenGL.hpp>
#include "Widget.hpp"
#include "Utils/Box.hpp"

namespace gui
{

/**
 * The TextBox widget is a one-line text editor.
 * It allows the user to enter and edit a single line of plain text.
 */
class TextBox: public Widget
{
public:
	TextBox(float width = 200.f);

	/**
	 *
	 */
	void setText(const sf::String& string);

	/**
	 *
	 */
	const sf::String& getText() const;

	/**
	  Set the cursor position
	 */
	void setCursor(size_t index);

	/**
	 * Get the cursor position
	 */
	size_t getCursor() const;

	sf::Clock TextBox::getCursorTimer();
	//get cursorTimer
	void TextBox::setCursorTimer(sf::Clock timer);

protected:
	// Callbacks
	void onKeyPressed(sf::Keyboard::Key key);
	void onMousePressed(float x, float y);
	void onTextEntered(sf::Uint32 unicode);
	void onStateChanged(State state);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Text    m_text;
	Box         m_box;
	mutable sf::RectangleShape m_cursor;
	mutable sf::Clock  m_cursor_timer;
	size_t             m_cursor_pos;
	int                m_max_length;
};

}

#endif // GUI_TEXTBOX_HPP
