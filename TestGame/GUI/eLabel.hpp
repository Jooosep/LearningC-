#ifndef GUI_ELabel_HPP
#define GUI_ELabel_HPP

#include "Widget.hpp"


namespace gui
{

	/**
	* Widget for displaying a simple text
	* Passive widget: cannot be focused or trigger event
	*/
	class eLabel : public Widget
	{
	public:

		eLabel(const sf::String& string = "");

		/**
		* eLabel's text
		*/
		void setText(const sf::String& string);
		const sf::String& getText() const;

		/**
		* eLabel's color
		*/
		void setColor(const sf::Color& color);
		const sf::Color& getColor() const;

		void setTextSize(size_t size);
		size_t getTextSize() const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void updateGeometry();

		sf::Text m_text;
	};

}

#endif // GUI_ELabel_HPP

