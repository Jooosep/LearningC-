#include "eTheme.hpp"

namespace gui
{

	size_t    eTheme::textSize = 12;
	eTheme::Style eTheme::click;
	eTheme::Style eTheme::input;
	sf::Color eTheme::windowBgColor;
	sf::Texture eTheme::windowBgTexture;
	int       eTheme::borderSize = 2.f;
	int       eTheme::minWidgetWidth = 86;
	float     eTheme::PADDING = 2.f;
	float     eTheme::MARGIN = 7.f;

	sf::Keyboard::Key eTheme::nextWidgetKey = sf::Keyboard::Down;
	sf::Keyboard::Key eTheme::previousWidgetKey = sf::Keyboard::Up;

	sf::Font eTheme::m_font;
	sf::Texture eTheme::m_texture;
	sf::IntRect eTheme::m_subrects[_TEX_COUNT];


	bool eTheme::loadFont(const std::string& filename)
	{
		return m_font.loadFromFile(filename);
	}


	bool eTheme::loadTexture(const std::string& filename)
	{
		if (m_texture.loadFromFile(filename))
		{
			sf::IntRect subrect;
			subrect.width = m_texture.getSize().x;
			subrect.height = m_texture.getSize().y / _TEX_COUNT;

			for (int i = 0; i < _TEX_COUNT; ++i)
			{
				m_subrects[i] = subrect;
				subrect.top += subrect.height;
			}

			borderSize = subrect.width / 3;
			return true;
		}
		return false;
	}


	const sf::Font& eTheme::getFont()
	{
		return m_font;
	}


	const sf::Texture& eTheme::getTexture()
	{
		return m_texture;
	}


	const sf::IntRect& eTheme::getTextureRect(Box::Type type, State state)
	{
		TextureID id(BOX_DEFAULT);
		switch (state)
		{
		case StateDefault:
			id = type == Box::Click ? BOX_DEFAULT : BOX_INPUT_DEFAULT;
			break;
		case StateHovered:
			id = type == Box::Click ? BOX_HOVERED : BOX_INPUT_DEFAULT;
			break;
		case StatePressed:
			id = type == Box::Click ? BOX_PRESSED : BOX_INPUT_FOCUSED;
			break;
		case StateFocused:
			id = type == Box::Click ? BOX_FOCUSED : BOX_INPUT_FOCUSED;
			break;
		}
		return m_subrects[id];
	}


	const sf::IntRect& eTheme::getCrossTextureRect()
	{
		return m_subrects[CROSS];
	}


	const sf::IntRect& eTheme::getArrowTextureRect()
	{
		return m_subrects[ARROW];
	}


	float eTheme::getBoxHeight()
	{
		return getLineSpacing() + borderSize * 2 + PADDING * 2;
	}


	int eTheme::getLineSpacing()
	{
		return m_font.getLineSpacing(textSize);
	}

}
