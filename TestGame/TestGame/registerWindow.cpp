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
#include <sstream>
#include <conio.h>
#include <fstream>
#include "SHA256hash.h"
#include "SQL.h"
#include "RNG.h"
#include <chrono>
#include <thread>




int registration()
{
	//menu events for widgets
	enum Callback
	{
		C_CREATE,
		C_PW,
		C_PW2,
		C_BACK,
		C_QUIT,
	};

	//errors
	int pwMatchError = 0;
	int duplicateUserError = 0;

	//window for drawing
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Limit the framerate to 60 frames per second (this step is optional)
	window.setFramerateLimit(60);

	//background sprite
	sf::Texture backGround;
	backGround.loadFromFile("resources/Images/background.png");
	sf::Sprite sprite(backGround);

	//menu object
	gui::Menu menu(window);
	menu.setPosition(0, 0);

	// main theme settings
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

	// Custom buttons, bg from .png files
	sf::Texture imgbutton;
	imgbutton.loadFromFile("resources/Images/btnBG2.png");
	sf::Texture imgbutton2;
	imgbutton2.loadFromFile("resources/Images/widebtnBG.png");
	gui::SpriteButton* registerButton = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Create new account");
	gui::SpriteButton* backButton = new gui::SpriteButton(imgbutton, sf::Color(240, 240, 240), "Back");

	//textbox
	gui::TextBox* newuser = new gui::TextBox(200);

	//textboxes for passwords
	gui::pwBox* password = new gui::pwBox(200);
	gui::pwBox* pwConfirm = new gui::pwBox(200);

	//put boxes into formlayout
	newb->addRow("username", newuser);
	newb->addRow("password", password, C_PW);
	newb->addRow("confirm password", pwConfirm, C_PW2);

	//put buttons into formlayout
	newb->add(registerButton, C_CREATE);
	newb->add(backButton, C_BACK);

	//account creation variable
	int creation = 0;

	//clock objects
	sf::Clock clock;
	sf::Clock clock2;
	//time variables
	sf::Time t1 = sf::milliseconds(0);
	sf::Time t2 = sf::milliseconds(0);

	//create window
	while (window.isOpen())
	{
		// Event processing
		//event object
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			// Request for closing the window
			if (evnt.type == sf::Event::Closed)
				return 2;
			//menu event code
			int id = menu.onEvent(evnt);
			switch (id)
			{
			// return to login
			case C_BACK:
			{
				return 0;
			}
			//create new account
			case C_CREATE:
			{
				//check that passwords match
				if (pwConfirm->getpwText() != password->getpwText() && pwMatchError == 0)
				{
					pwMatchError = 1;
					clock2.restart();
					vbox->addeLabel("passwords don't match!", 90);
				}
				// check fields aren't empty
				if (!newuser->getText().isEmpty() && !password->getText().isEmpty() && pwConfirm->getpwText() == password->getpwText())
				{
					//rng object for creating the salt
					RNG salt;
					//array for the the salt in string form
					char salty[20];
					//array for pepper in string form
					char pepper[4];
					// take last three digits of an rng for pepper
					UINT rand = salt.rng();
					int peppa = rand % 1000;
					
					// array for pw hash
					char pwhash[100];
					//allocation of salt into char array
					_itoa_s(salt.rng(), salty, sizeof(salty), 10);

					//allocation of pepper into char array
					_itoa_s(peppa, pepper, sizeof(peppa), 10);

					//combine  pw, salt and pepper
					std::string pwSaltnPepper = password->getpwText() + salty + pepper;

					//object for type SHA256 hash (windows cng)
					SHA256hash newhash;

					// push hashed(pw, salt and pepper) into char array
					_itoa_s(newhash.hash(pwSaltnPepper), pwhash, sizeof(pwhash), 10);

					//object for sql connection
					sqlConnect sql;

					// insert username, hash and salt into db
					// check for error
					if (!sql.insert(newuser->getText(), pwhash, salty))
					{
						duplicateUserError++;
						vbox->addeLabel("creation failed!", 91);
						duplicateUserError++;
						clock2.restart();
					}
					//confirm successful conversion
					else
					{
						creation = 1;
						vbox->addeLabel("Account successfully created!", 92);
						clock.restart();
					}
				}
				break;
			}
			case C_QUIT:
				window.close();
				break;
			}
		}

		// Clear the whole window before rendering a new frame
		window.clear();

		//draw background
		window.draw(sprite);

		if (pwMatchError == 1)
		{
			t2 = clock2.getElapsedTime();
			if (t2.asSeconds() > 4)
			{
				vbox->depush(90);
				pwMatchError = 0;
			}
		}
		//check if error has occured, in case of error display label
		if (duplicateUserError == 1)
		{
			t1 = clock.getElapsedTime();
			if (t1.asSeconds() > 3)
			{
				vbox->depush(91);
				duplicateUserError = 0;
			}
		}
		//check for successful creation, in case of creation display message, return automatically to login
		if (creation == 1)
		{
			t1 = clock.getElapsedTime();
			if (t1.asSeconds() >2)
			return 1;
		}

		//draw menu into window
		window.draw(menu);

		// display drawed objects
		window.display();
	}
	return 1;
}
  
