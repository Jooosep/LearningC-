#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <Gui.hpp>
#include "Menu.hpp"
#include "Theme.hpp"
#include "SHA256hash.h"
#include "stdafx.h"
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>


//window parameters
int width = 1200;
int height = 800;

//colormap holding a color for each pixel in the window
sf::Color* colorMap = new sf::Color[width*height];

//compasspoint related variables
sf::Vector2i n = sf::Vector2i(0, -1);
sf::Vector2i w = sf::Vector2i(-1, 0);
sf::Vector2i s = sf::Vector2i(0, 1);
sf::Vector2i e = sf::Vector2i(1, 0);
sf::Vector2i vtbl[4] = { n,w,s,e };
char cptbl[4] = { 'n','w','s','e' };
char rptbl[4] = { 's','e','n','w' };
char metacptbl[4][2] = { {'w','e'} ,{'n','s'}, {'w','e'}, {'n','s'} };


//check colormap color ffor certain position
sf::Color check(sf::Color* map, sf::Vector2i pos, sf::Vector2i cp)
{	
	if (pos.x < (width-2) && pos.x>2 && pos.y < (height - 2) && pos.y>2)
	{
		sf::Vector2i v = sf::Vector2i(pos + cp);
		return map[int(v.x + v.y * width)];
	}
	else return sf::Color::Black;
}


int centerDis(sf::Vector2i pos)
{
	return int(sqrt(pow((width/2) - pos.x, 2) + pow((height/2)- pos.y, 2)));
}


int checkClearing(sf::Color* map, sf::Vector2i pos, char cp)
{
	if (cp == 'n')
	{
		for (int i = 1; i < 500; i++)
		{
			sf::Vector2i v = sf::Vector2i(0, -i)+pos;
			if (v.x<(width - 1) && v.x>1 && v.y<(height - 1) && v.y>1 && map[int(v.x + v.y * width)].r > 50)
			{
				return i;
			}
		}
	}
	if (cp == 'w')
	{
		for (int i = 1; i < 500; i++)
		{
			sf::Vector2i v = sf::Vector2i(-i, 0) + pos;
			if (v.x<(width - 1) && v.x>1 && v.y<(height - 1) && v.y>1 && map[int(v.x + v.y * width)].r > 50)
			{
				return i;
			}
		}
	}
	if (cp == 's')
	{
		for (int i = 1; i < 500; i++)
		{
			sf::Vector2i v = sf::Vector2i(0, i) + pos;
			if (v.x<(width - 1) && v.x>1 && v.y<(height - 1) && v.y>1 &&(map[int(v.x + v.y * width)].r > 50))
			{
				return i;
			}
		}
	}
	if (cp == 'e')
	{
		for (int i = 1; i < 500; i++)
		{
			sf::Vector2i v = sf::Vector2i(i, 0) + pos;
			if (v.x<(width - 1) && v.x>1 && v.y<(height - 1) && v.y>1 && map[int(v.x + v.y * width)].r > 50)
			{
				return i;
			}
		}
	}
	return 1000;
}
int checkCrowd(sf::Color* map, sf::Vector2i position)
{
	int crowd[8];
	if (position.x < (width - 2) && position.x>2 && position.y < (height - 2) && position.y>2)
	{
		sf::Vector2i nn = position + sf::Vector2i(0, -1);
		crowd[0] = map[int(nn.x + nn.y * width)].r;
		sf::Vector2i ss = position + sf::Vector2i(0, 1);
		crowd[1] = map[int(ss.x + ss.y * width)].r;
		sf::Vector2i ww = position + sf::Vector2i(-1, 0);
		crowd[2] = map[int(ww.x + ww.y * width)].r;
		sf::Vector2i ee = position + sf::Vector2i(1, 0);
		crowd[3] = map[int(ee.x + ee.y * width)].r;
		sf::Vector2i ne = position + sf::Vector2i(1, 1);
		crowd[4] = map[int(ne.x + ne.y * width)].r;
		sf::Vector2i se = position + sf::Vector2i(1, -1);
		crowd[5] = map[int(se.x + se.y * width)].r;
		sf::Vector2i nw = position + sf::Vector2i(-1, 1);
		crowd[6] = map[int(nw.x + nw.y * width)].r;
		sf::Vector2i sw = position + sf::Vector2i(-1, -1);
		crowd[7] = map[int(sw.x + sw.y * width)].r;
		int crowded = 0;

		for (int i = 0; i < 8; i++)
		{
			if (crowd[i] > 50)
			{
				crowded += 1;
			}
		}
		return crowded;
	}
	else
		return 4;
}

int aggregation()
{
	srand(unsigned int(time(NULL)));

	//button id's
	enum Callback
	{
		SLIDER_1,
		SLIDER_2,
		SLIDER_3,
		SLIDER_4,
		SLIDER_5,
		START,
		PAUSE,
		SEED_LINE,
		SEED_CIRCLE,
		RESET,
		REFRESH
	};

	// let's define a view
	sf::View view(sf::FloatRect(0.f, 0.f, float(width), float(height)));
	
	//define window
	sf::RenderWindow window(sf::VideoMode(width, height), "vertex");

	// vector for holding positions of random particles
	std::vector<sf::Vector2i> particleVector(100000);

	// vector holding positions to be drawn
	std::vector<sf::Vector2i> drawVector;

	//vector holding positions to be drawn temporarily
	std::vector<sf::Vector2i> helpVector;
	std::vector<sf::Vector2i> help2Vector;


	// are keys hit
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	bool Zoom = false;//Z
	bool outZoom = false;//S

	float zoom = 1; // zoom factor controled by Z and S keys
	sf::Vector2i camPos(0, 0); // camera position controled with arrow keys
	sf::Vector2i mousePos(0, 0);
	sf::Clock deltaTime; // SFML clock to keep track of our framerate
	float dt = 0.00000001f; // the time a frame requires to be drawn (=1/framerate). I set it at 0.0000001 to avoid getting undefined behaviors during the very first frame

	//fill particle vector with random positions
	for (auto& position : particleVector)
	{
		position = sf::Vector2i((rand() % (int)(width + 1)), (rand() % (int)(height + 1)));
	}

	//pixel rect, shape to be drawn
	sf::RectangleShape pixel(sf::Vector2f(1, 1));
	pixel.setFillColor(sf::Color::Magenta);

	//menu 
	gui::Menu runMenu(window);
	gui::Menu pauseMenu(window);

	//Menu theme
	gui::Theme::loadFont("Arial.ttf");
	gui::Theme::textSize = 15;
	gui::Theme::click.textColor = sf::Color(200, 200, 200);
	gui::Theme::click.textColorHover = sf::Color(0, 255, 0);
	gui::Theme::click.textColorFocus = sf::Color(255, 0, 0);
	gui::Theme::input.textColor = sf::Color(150, 200, 205);
	gui::Theme::input.textColorHover = sf::Color(205, 225, 228);
	gui::Theme::input.textColorFocus = sf::Color(0, 255, 0);
	gui::Theme::PADDING = 5.f;
	gui::Theme::loadTexture("texture.png");

	//Layouts
	gui::HBoxLayout* hbox = pauseMenu.addHBoxLayout();
	gui::FormLayout* form = hbox->addFormLayout();
	gui::HBoxLayout* hbox_2 = runMenu.addHBoxLayout();

	//sliders
	gui::Slider* crowd_slider_1 = new gui::Slider();
	crowd_slider_1->setQuantum(10);
	gui::Slider* crowd_slider_2 = new gui::Slider();
	crowd_slider_2->setQuantum(10);
	gui::Slider* init_clearing_slider_1 = new gui::Slider();
	gui::Slider* init_clearing_slider_2 = new gui::Slider();
	gui::Slider* fresh_slider = new gui::Slider();
	fresh_slider->setQuantum(10);

	//Text for slider value
	sf::Font font;
	font.loadFromFile("Arial.ttf");
	sf::Text slider_value("", font);
	slider_value.setCharacterSize(30);
	slider_value.setPosition(250, 600);


	//buttons
	sf::Texture imgbutton2;
	imgbutton2.loadFromFile("btnBG2.png");
	gui::SpriteButton* start_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Start");
	gui::SpriteButton* seed_line_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Seed line");
	gui::SpriteButton* seed_circle_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Seed circle");
	gui::SpriteButton* reset_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Reset");
	gui::SpriteButton* refresh_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Refresh growth");
	gui::SpriteButton* pause_btn = new gui::SpriteButton(imgbutton2, sf::Color(240, 240, 240), "Pause");

	// push buttons and sliders into layouts
	form->add(start_btn, START);
	form->add(seed_line_btn, SEED_LINE);
	form->add(seed_circle_btn, SEED_CIRCLE);
	form->add(reset_btn, RESET);
	form->addRow("crowd restriction", crowd_slider_1, SLIDER_1);
	form->addRow("crowd restriction 2", crowd_slider_2, SLIDER_2);
	form->addRow("clearing ahead", init_clearing_slider_1, SLIDER_3);
	form->addRow("clearing sides", init_clearing_slider_2, SLIDER_4);
	form->addRow("freshness limit", fresh_slider, SLIDER_5);
	hbox_2->add(refresh_btn, REFRESH);
	hbox_2->add(pause_btn, PAUSE);


	//fill colormap with black
	for (int i = 0; i < (width*height); i++)
	{
		colorMap[i] = sf::Color::Black;
	}

	//mode -1: paused, 0: diffusion limited, 1:saturated
	int mode = -1;

	//which menu is usable
	int activeMenu = 0;

	//to lower color of drawn pixels gradually
	int counter = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

			// Request for closing the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				//draw pixel in mouse position
				if (event.key.code == sf::Keyboard::P)
				{
					sf::Vector2i x = sf::Mouse::getPosition(window);
					sf::Vector2i xview = sf::Vector2i(int(window.mapPixelToCoords(x).x),int(window.mapPixelToCoords(x).y));
					colorMap[int(xview.x + xview.y * width)] = sf::Color(255, 0, 0, 0);
					helpVector.push_back(sf::Vector2i(xview.x, xview.y));
				}

				if (event.key.code == sf::Keyboard::M)
				{
					if (mode == 0)
						mode = 1;
					else if (mode == 1)
						mode = 0;
				}
				//renew random particles and treat all drawn as new
				if (event.key.code == sf::Keyboard::R)
				{
					particleVector.clear();
					particleVector.resize(100000);
					for (auto& position : particleVector)
					{
						position = sf::Vector2i((rand() % (int)(width + 1)), (rand() % (int)(height + 1)));
					}
					for (auto& posVect : drawVector)
					{
						colorMap[int(posVect.x + posVect.y * 1200)] = sf::Color(255, 0, 0, 0);
					}
					break;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				Zoom = true;
			}
			else
			{
				Zoom = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				outZoom = true;
			}
			else
			{
				outZoom = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				Right = true;
			}
			else
			{
				Right = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				Down = true;
			}
			else
			{
				Down = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				Left = true;
			}
			else
			{
				Left = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				Up = true;
			}
			else
			{
				Up = false;
			}


			int id = 0;

			//runmenu
			if (activeMenu == 1)
			{
				id = runMenu.onEvent(event);
				switch (id)
				{
				case PAUSE:
				{
					mode = -1;
					activeMenu = 0;
					break;
				}
				case REFRESH:
				{
					particleVector.clear();
					particleVector.resize(100000);
					for (auto& position : particleVector)
					{
						position = sf::Vector2i((rand() % (int)(width + 1)), (rand() % (int)(height + 1)));
					}
					for (auto& posVect : drawVector)
					{
						colorMap[int(posVect.x + posVect.y * 1200)] = sf::Color(255, 0, 0, 0);
					}
					break;
				}
				}
			}
			//pausemenu
			else if (activeMenu == 0)
			{
				id = pauseMenu.onEvent(event);
				switch (id)
				{
				case START:
				{
					if (mode == -1)
					{
						mode = 0;
					}
					activeMenu = 1;
					break;
				}
				case SEED_LINE:
				{
					for (int i = 0; i < 1200; i++)
					{
						if (i % 2 == 0)
						{
							helpVector.push_back(sf::Vector2i(i, 500));
							colorMap[int(i + 500 * 1200)] = sf::Color(255, 0, 0, 0);
						}
					}
					break;
				}
				case SEED_CIRCLE:
				{
					for (int i = 0; i < 100; i++)
					{
						int x = int(600.f + 200.f * cos(i / 100.f*(2 * 3.14)));
						int y = int(400.f + 200.f * sin(i / 100.f * (2 * 3.14)));
						helpVector.push_back(sf::Vector2i(x, y));
						colorMap[int(x + y * 1200)] = sf::Color(255, 0, 0, 0);
					}
					break;
				}

				//reset screen position and clear screen
				case RESET:
				{
					zoom = 1;
					camPos = sf::Vector2i(0, 0);
					for (int i = 0; i < (width * height); i++)
					{
						colorMap[i] = sf::Color::Black;
					}
					drawVector.clear();
					particleVector.clear();
					particleVector.resize(100000);
					for (auto& position : particleVector)
					{
						position = sf::Vector2i((rand() % (int)(width + 1)), (rand() % (int)(height + 1)));
					}
					break;
				}
				case SLIDER_1:
				{
					slider_value.setString(std::to_string(int(crowd_slider_1->getValue() / 10)));
					break;
				}
				case SLIDER_2:
				{
					slider_value.setString(std::to_string(int(crowd_slider_2->getValue() / 10)));
					break;
				}
				case SLIDER_3:
				{
					slider_value.setString(std::to_string(int(init_clearing_slider_1->getValue() * 3)));
					break;
				}
				case SLIDER_4:
				{
					slider_value.setString(std::to_string(int(init_clearing_slider_2->getValue() * 3)));
					break;
				}
				case SLIDER_5:
				{
					slider_value.setString(std::to_string(int(fresh_slider->getValue() *2.50)));
					break;
				}
				}
			}
		}
			window.clear();

			if (Zoom)
			{
				zoom -= 0.2f*dt;
			}
			if (outZoom)
			{
				zoom += 0.2f*dt;
			}
			if (Right)
			{
				camPos.x += int(200 * dt / zoom*((zoom*width) / width));
			}
			if (Left)
			{
				camPos.x -= int(200 * dt / zoom*((zoom*width) / width));
			}
			if (Up)
			{
				camPos.y -= int(200 * dt / zoom*((zoom*height) / height));
			}
			if (Down)
			{
				camPos.y += int(200 * dt / zoom*((zoom*height) / height));
			}
			view = sf::View(sf::FloatRect(camPos.x + (width - width*zoom) / 2, camPos.y + (height - height*zoom) / 2, width * zoom, height * zoom));
			int clearing1 = init_clearing_slider_1->getValue() * 3;
			int clearing2 = init_clearing_slider_2->getValue() * 3;
			int freshness = int(fresh_slider->getValue()*2.50);


			if (mode == 0)
			{
				for (auto pos_iter = particleVector.begin(); pos_iter != particleVector.end(); pos_iter++)
				{
					sf::Vector2i v = sf::Vector2i(*pos_iter + sf::Vector2i(rand() % 40 - 20, rand() % 40 - 20));
					if ((v.x > 0 && v.x < (width - 1) && v.y>0 && v.y < (height - 1)))
					{
						*pos_iter = v;
						if (colorMap[int((*pos_iter).x) + width * int((*pos_iter).y)].r == 0)
							colorMap[int((*pos_iter).x) + width * int((*pos_iter).y)] = sf::Color(50, 0, 0, 0);
						if (colorMap[int((*pos_iter).x) + width * int((*pos_iter).y)].r > 50)
						{
							pos_iter = particleVector.erase(pos_iter);
							pos_iter--;
						}
					}
				}
			}
			else if (mode == 1)
			{
				for (int i = 0; i < (width * height); i++)
				{
					if (colorMap[i].r == 0)
						colorMap[i] = sf::Color(50, 0, 0, 0);
				}
			}

			//when running
			if (mode == 0 || mode == 1)
			{
				for (auto& posVect : drawVector)
				{
					int curCrowd = checkCrowd(colorMap, posVect);
					if (curCrowd < crowd_slider_1->getValue() / 10)
					{
						for (int i = 0; i < 4; i++)
						{
							sf::Vector2i Vec = sf::Vector2i(posVect + vtbl[i]);
							int cpCrowd = checkCrowd(colorMap, sf::Vector2i(posVect + vtbl[i]));
							if (check(colorMap, posVect, vtbl[i]).r == 50 && colorMap[int(posVect.x + posVect.y * width)].r > freshness)
							{
								if (cpCrowd < crowd_slider_2->getValue() / 10 && (checkClearing(colorMap, Vec, cptbl[i]) > (clearing1) && (checkClearing(colorMap, Vec, metacptbl[i][0]) > (clearing2) || (checkClearing(colorMap, Vec, metacptbl[i][1])) > (clearing2))))
								{

									helpVector.push_back(Vec);
									colorMap[int(Vec.x + Vec.y * width)] = sf::Color(255, 0, 0, 0);
								}
							}
						}
					}
				}
			}

			drawVector.insert(drawVector.end(), helpVector.begin(), helpVector.end());

			for (auto& posVect : drawVector)
			{
				if (posVect.x > 0 && posVect.x < 1200 && posVect.y>0 && posVect.y < 800)
				{
					pixel.setPosition(sf::Vector2f(float(posVect.x),float(posVect.y)));
					pixel.setFillColor(sf::Color(colorMap[int(posVect.x + posVect.y * width)].r, 100, 200, 255));
					window.draw(pixel);
				}
			}

			if (mode == -1)
			{
				window.draw(pauseMenu);
				window.draw(slider_value);
			}
			else
				window.draw(runMenu);

			window.setView(view);
			window.display();
			helpVector.clear();

			if (mode == 0)
			{
				for (int i = 0; i < (width * height); i++)
				{
					if (colorMap[i] == sf::Color(50, 0, 0, 0))
						colorMap[i] = sf::Color::Black;
				}
			}

			//Gradually lower r value of drawn pixels
			counter++;
			if (counter % 1 == 0 && mode == 0 || mode == 1)
			{
				for (auto& posVect : drawVector)
				{
					if (posVect.x > 0 && posVect.x < width && posVect.y > 0 && posVect.y < height)
					{
						if (colorMap[int(posVect.x + posVect.y * width)].r >= freshness)
							colorMap[int(posVect.x + posVect.y * width)].r -= 1;
					}
				}
			}

			dt = deltaTime.restart().asSeconds(); // measure the time it took to complete all the calculations for this particular frame (dt=1/framerate) 
		}
		return 0;
	}