#ifndef GUI_PWBOX_HPP
#define GUI_PWBOX_HPP

#include <SFML/OpenGL.hpp>
#include "Widget.hpp"
#include "Utils/Box.hpp"

namespace gui
{

	/**
	* The TextBox widget is a one-line text editor.
	* It allows the user to enter and edit a single line of plain text.
	*/
	class pwBox : public Widget
	{
	public:
		pwBox(float width = 200.f);

		/**
		*
		*/
		void setText(const sf::String& string);

		/**
		*
		*/
		const sf::String& getText() const;
		const sf::String& getpwText() const;

		/**
		Set the cursor position
		*/
		void setCursor(size_t index);

		/**
		* Get the cursor position
		*/
		size_t getCursor() const;

	protected:
		// Callbacks
		void onKeyPressed(sf::Keyboard::Key key);
		void onMousePressed(float x, float y);
		void onTextEntered(sf::Uint32 unicode);
		void onStateChanged(State state);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		char		pw[32];
		sf::Text	m_pwtext;
		sf::Text    m_text;
		Box         m_box;
		mutable sf::RectangleShape m_cursor;
		mutable sf::Clock  m_cursor_timer;
		size_t             m_cursor_pos;
		int                m_max_length;
	};

}

#endif // GUI_PWBOX_HPP
