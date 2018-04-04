#include "eLabel.hpp"
#include "eTheme.hpp"

namespace gui
{

	eLabel::eLabel(const sf::String& string)
	{
		m_text.setFont(eTheme::getFont());
		m_text.setPosition(eTheme::PADDING, eTheme::PADDING);
		m_text.setFillColor(eTheme::click.textColor);
		m_text.setCharacterSize(eTheme::textSize);
		setSelectable(false);
		setText(string);
	}


	void eLabel::setText(const sf::String& string)
	{
		m_text.setString(string);
		updateGeometry();
	}


	const sf::String& eLabel::getText() const
	{
		return m_text.getString();
	}


	void eLabel::setColor(const sf::Color& color)
	{
		m_text.setFillColor(color);
	}


	const sf::Color& eLabel::getColor() const
	{
		return m_text.getFillColor();
	}


	void eLabel::setTextSize(size_t size)
	{
		m_text.setCharacterSize(size);
		updateGeometry();
	}


	size_t eLabel::getTextSize() const
	{
		return m_text.getCharacterSize();
	}


	void eLabel::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_text, states);
	}


	void eLabel::updateGeometry()
	{
		Widget::setSize(m_text.getLocalBounds().width + eTheme::PADDING * 2,
			m_text.getLocalBounds().height + eTheme::PADDING * 2);
	}

}
