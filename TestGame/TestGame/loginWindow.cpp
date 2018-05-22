 // TestGame.cpp : Defines the entry point for the console application.
//

#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <Gui.hpp>
#include "Menu.hpp"
#include "Theme.hpp"
#include <string>
#include <iostream>
#include <conio.h>
#include "SHA256hash.h"
#include "SQL.h"
#include "RNG.h"
#include <vector>

int login()
{
	// menu widget trigger codes
	enum Callback
	{
		C_TEXT,
		C_REGISTER,
		C_LOGIN,
	};

	//error variable
	int loginError = 0;

	//window object for drawing
	sf::RenderWindow window(sf::VideoMode(800, 600), "Login");

	// Limit the framerate to 60 frames per second (this step is optional)
	window.setFramerateLimit(60);

	//background for window
	sf::Texture backGround;
	backGround.loadFromFile("resources/Images/background.png");
	sf::Sprite sprite(backGround);

	//menu object
	gui::Menu menu(window);
	menu.setPosition(0, 0);

	//theme settings
	gui::Theme::loadFont("resources/Fonts/MATURASC.ttf");
	gui::Theme::textSize = 26;
	gui::Theme::click.textColor = sf::Color(200, 200, 200);
	gui::Theme::click.textColorHover = sf::Color(0, 255, 0);
	gui::Theme::click.textColorFocus = sf::Color(255, 0, 0);
	gui::Theme::input.textColor = sf::Color(100, 180, 205);
	gui::Theme::input.textColorHover = sf::Color(205, 225, 228);
	gui::Theme::input.textColorFocus = sf::Color(0, 255, 0);
	gui::Theme::PADDING = 2.f;

	//error/alternative theme settings
	gui::eTheme::loadFont("resources/Fonts/arial.ttf");
	gui::eTheme::textSize = 20;
	gui::eTheme::click.textColor = sf::Color(255, 0, 0);
	gui::eTheme::click.textColorHover = sf::Color(0, 255, 0, 255);
	gui::eTheme::click.textColorFocus = sf::Color(255, 0, 0, 255);
	gui::eTheme::input.textColor = sf::Color(150, 200, 205);
	gui::eTheme::input.textColorHover = sf::Color(205, 225, 228);
	gui::eTheme::input.textColorFocus = sf::Color(0, 255, 0);
	gui::eTheme::PADDING = 5.f;

	//horizontal layout into menu
	gui::HBoxLayout* hbox = menu.addHBoxLayout();
	//formlayout into horizontal layout
	gui::FormLayout* newb = hbox->addFormLayout();
	//vertical layout into horizontal layout
	gui::VBoxLayout* vbox = hbox->addVBoxLayout();

	// Custom buttons with .png backgrounds
	sf::Texture imgbutton;
	imgbutton.loadFromFile("resources/Images/btnBG.png");
	sf::Texture imgbutton2;
	imgbutton2.loadFromFile("resources/Images/btnBG2.png");
	gui::SpriteButton* loginButton = new gui::SpriteButton(imgbutton, sf::Color(200, 200, 200), "Login");
	gui::SpriteButton* registerButton = new gui::SpriteButton(imgbutton2, sf::Color(200, 200, 200), "Register");

	//Textboxes for username and pw
	gui::TextBox* username = new gui::TextBox(200);
	gui::pwBox* password = new gui::pwBox(200);

	//insert textboxes into formlayout
	newb->addRow("username", username, C_TEXT);
	newb->addRow("password", password, C_TEXT);

	//insert buttons into formlayout
	newb->add(loginButton, C_LOGIN);
	newb->add(registerButton, C_REGISTER);

	//clock object to time temporary messages
	sf::Clock clock;

	//time variables
	sf::Time t1 = sf::milliseconds(0);


	//open the window
	while (window.isOpen())
	{
		// Event processing
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			// Request for closing the window
			if (evnt.type == sf::Event::Closed)
				return 2;

			//retrieve id of triggered widget
			int id = menu.onEvent(evnt);
			switch (id)
			{
			case C_LOGIN:
			{
				//check that fields aren't empty
				if (!username->getText().isEmpty() && !password->getText().isEmpty())
				{
					//object for sql connection and operations
					sqlConnect sql;

					//string from username box
					std::string user = username->getText();
					
					//retrieve hashed pw and salt from db, corresponding to the user
					std::vector<std::string> hashVector = sql.fetch(user);

					//combine pw entered into textbox and salt retrieved from db
					std::string pwSalt = password->getpwText() + hashVector[1];

					//hashing object (windows cng)
					SHA256hash newhash;

					//test hash with all possible peppers
					for (int i = 0; i < 1000; i++)
					{
						//array for holding pepper in string form
						char pepper[4];
						//array for holding hash in string form
						char hash[50];

						//convert i into string
						_itoa_s(i, pepper, sizeof(pepper), 10);

						//add pepper
						std::string pwSaltnPepa = pwSalt + pepper;

						//convert hash to string
						_itoa_s(newhash.hash(pwSaltnPepa), hash, sizeof(hash), 10);

						//compare newly created hash to hash from db
						if (hash == hashVector[0])
						{
							std::cout << "Successful login!" << std::endl;
							return 1;
						}
					}
					vbox->addeLabel("Login failed!", 55);
					loginError = 1;
					clock.restart();
				}
				break;
			}
			// go to register window
			case C_REGISTER:
			{
				return 0;
			}
			}
			// Clear the whole window before rendering a new frame
			window.clear();

			//draw the background
			window.draw(sprite);

			//remove error message after time
			if (loginError == 1)
			{
				t1 = clock.getElapsedTime();
				if (t1.asSeconds() > 3)
				{
					vbox->depush(55);
					loginError = 0;
				}
			}
			//draw menu object
			window.draw(menu);

			//display drawed objects
			window.display();
		}
	}
	return 0;
}


