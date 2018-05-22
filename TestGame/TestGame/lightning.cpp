#pragma once
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <Gui.hpp>
#include "Menu.hpp"
#include "Theme.hpp"
#include <fstream>
#include "SHA256hash.h"
#include "stdafx.h"
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>





int lightningSimulation()
{
	int width = 1200;
	int height = 800;

	srand(time(NULL));

	enum Callback
	{
		APLUSB
	};

	//positionArray

	sf::Font font;
	font.loadFromFile("resources/Fonts/Arial.ttf");

	int chosen = 0;
	//calculations

	// let's define a view
	sf::View view(sf::FloatRect(0, 0, width, height));

	float sSize = 10.f;
	float mSize = 14.f;
	float lSize = 18.f;

	//particle line
	sf::Texture particleTexture;
	particleTexture.loadFromFile("resources/Images/lightning.png");
	sf::Texture particleTexture2;
	particleTexture2.loadFromFile("resources/Images/bigLightning.png");
	sf::RectangleShape lightningSmall(sf::Vector2f(sSize, sSize));
	lightningSmall.setTexture(&particleTexture);
	sf::RectangleShape lightningMedium(sf::Vector2f(mSize, mSize));
	lightningMedium.setTexture(&particleTexture);
	sf::RectangleShape lightningLarge(sf::Vector2f(lSize, lSize));
	lightningLarge.setTexture(&particleTexture);
	sf::RectangleShape bigLightning(sf::Vector2f(lSize, lSize));
	bigLightning.setTexture(&particleTexture2);
	sf::Vector2i pointA = sf::Vector2i(300, 300);
	sf::Vector2i pointB = sf::Vector2i(400, 400);
	int pointA_set = 0;
	int pointB_set = 0;
	sf::Clock timer;
	sf::Time tickRate;

	sf::Texture texture;
	texture.loadFromFile("resources/Images/buttontexture.png");
	sf::RectangleShape rect(sf::Vector2f(6.0f, 6.0f));
	rect.setTexture(&texture);
	rect.setPosition(0.1f, 400.0f);


	sf::RenderWindow window(sf::VideoMode(width, height), "vertex");
	sf::RenderStates states = sf::RenderStates(sf::BlendMode::BlendMode());

	sf::Time t1 = sf::milliseconds(0);
	std::vector<float> u;
	std::vector<sf::Vector2f> zVector;
	std::vector<sf::Vector2f> cVector(500);
	std::vector<sf::Vector2f> pixelVector;
	std::vector<sf::Vector2f> pixelVector2;
	std::vector<sf::Vector2f> bunguVector;
	std::vector<sf::Vector2f> tenguVector;
	std::vector<sf::Vector2f> roamVector;
	sf::Vector2i v;

	bool LMB = false; // is left mouse button hit ?
	bool Left = false;
	bool Right = false;
	bool Up = false;
	bool Down = false;
	bool Zoom = false;
	bool outZoom = false;

	float zoom = 1; // zoom factor controled by Z and S keys
	sf::Vector2f camPos(0, 0); // camera position controled with arrow keys
	sf::Vector2f mousePos(0, 0);
	sf::Clock deltaTime; // SFML clock to keep track of our framerate
	float dt = 0.00000001; // the time a frame requires to be drawn (=1/framerate). I set it at 0.0000001 to avoid getting undefined behaviors during the very first frame


	sf::Vector2f pixelPos = sf::Vector2f(600, 400);
	sf::Vector2i n = sf::Vector2i(0, -1);
	sf::Vector2i w = sf::Vector2i(-1, 0);
	sf::Vector2i s = sf::Vector2i(0, 1);
	sf::Vector2i e = sf::Vector2i(1, 0);
	sf::Vector2i ne = sf::Vector2i(1, -1);
	sf::Vector2i se = sf::Vector2i(1, 1);
	sf::Vector2i nw = sf::Vector2i(-1, -1);
	sf::Vector2i sw = sf::Vector2i(-1, 1);

	std::vector<sf::Vector2f> routes;


	// vector holding positions to be drawn
	std::vector<sf::Vector2i> drawVector;

	//vector holding positions to be drawn temporarily
	std::vector<sf::Vector2i> helpVector;

	sf::Color* colorMap = new sf::Color[width*height];
	sf::Clock clock;

	//1=hit, 0= not hit
	int GroundHit = -1;

	int strikeCounter = 0;

	float size = 7.f;

	float growth = 0.f;

	const int numberOfWaypoints = 50         ;

	int HorizontalWaypoint[numberOfWaypoints];

	int VerticalWaypoint[numberOfWaypoints];

	int HorizontalLeaps[numberOfWaypoints];

	int VerticalLeaps[numberOfWaypoints];

	int endPos = numberOfWaypoints - 1;

	int random2 = 0;

	int counter = 0;

	int min_x2 = 0;
	int verticalLeap = 0;

	int minHorizontalPos = 0;
	int maxHorizontalPos = 0;

	int min_add = 0;
	int max_add = 0;

	const int minStartingX = 200;

	int verticalEndPoint = 0;

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

				if (event.key.code == sf::Keyboard::Space)
				{
					drawVector.push_back(sf::Vector2i(minStartingX + (rand() % (int)(width - minStartingX/2)), 0));

					if(GroundHit == -1)
						GroundHit = 0;

					int min_y_addition = int(1.f / numberOfWaypoints * height * 0.9f);
					int extra_y_addition = int(1.f / numberOfWaypoints * height * 0.4f);

					VerticalWaypoint[0] = min_y_addition + rand() % extra_y_addition;

					for (int i = 1; i <= endPos; i++)
					{
						VerticalWaypoint[i] = VerticalWaypoint[i - 1] + min_y_addition + rand() % extra_y_addition;
					}

					verticalEndPoint = VerticalWaypoint[endPos];

					HorizontalWaypoint[endPos] = drawVector.back().x - (height/4) + rand() % (height/2);

					for (int i = endPos - 1; i > -1; i--)
					{	
						verticalLeap = VerticalWaypoint[i + 1] - VerticalWaypoint[i];
						maxHorizontalPos = drawVector.back().x + VerticalWaypoint[i];
						minHorizontalPos = drawVector.back().x - VerticalWaypoint[i];
						int maxHorizontalLeap = maxHorizontalPos - HorizontalWaypoint[i + 1];
						int minHorizontalLeap = minHorizontalPos - HorizontalWaypoint[i + 1];

						min_add = - verticalLeap;
						max_add = verticalLeap;

 						if (maxHorizontalLeap < verticalLeap)
							max_add = maxHorizontalLeap;

						if (minHorizontalLeap > -verticalLeap) 
							min_add = minHorizontalLeap;
						if (max_add - min_add + 1 == 0)
							HorizontalWaypoint[i] = HorizontalWaypoint[i + 1] + rand() % (max_add - min_add + 2) + min_add;
						else
							HorizontalWaypoint[i] = HorizontalWaypoint[i + 1] + rand() % (max_add - min_add + 1) + min_add;

						HorizontalLeaps[i] = HorizontalWaypoint[i + 1] - HorizontalWaypoint[i];

						VerticalLeaps[i] = VerticalWaypoint[i + 1] - VerticalWaypoint[i];
						
					}


				}
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				LMB = true;
			else
				LMB = false;

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


			mousePos = (sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) / zoom - sf::Vector2f(width / 2, height / 2) / zoom - camPos); // we store the current mouse position in this variable;

			
		}
		window.clear();

		if (Zoom)
		{
			zoom -= 0.2*dt;
		}
 		if (outZoom)
		{
			zoom += 0.2*dt;
		}
		if (Right)
		{
			camPos.x += 200 * dt / zoom*((zoom*width) / width);
		}
		if (Left)
		{
			camPos.x -= 200 * dt / zoom*((zoom*width) / width);
		}
		if (Up)
		{
			camPos.y -= 200 * dt / zoom*((zoom*height) / height);
		}
		if (Down)
		{
			camPos.y += 200 * dt / zoom*((zoom*height) / height);
		}
		view = sf::View(sf::FloatRect(camPos.x + (width - width*zoom) / 2, camPos.y + (height - height*zoom) / 2, width * zoom, height * zoom));

		/*
		for (int i = 0; i < 30000; i++)
		{
			float x = -10.f+i*0.01f;
			float y = sqrt(pow(x, 3.f) - x);
			if (i % 2 != 0)
				lightningSmall.setPosition(x*40.f + 600.f, 400.f - y*20.f);
			else
				lightningSmall.setPosition(x*40.f + 600.f, 400.f + y*20.f);

			window.draw(lightningSmall);
		}
		*/
		t1 = clock.getElapsedTime();

		if (drawVector.size() > 0 && GroundHit == 0)
		{

			int verticalPos = drawVector.back().y;

			for (int i = 0; i < numberOfWaypoints; i++)
			{
				for (; verticalPos < VerticalWaypoint[i];)
				{

					verticalPos = drawVector.back().y;
					int random = rand() % 100;

					sf::Vector2i addition;
					if (random < 20)
						addition = drawVector.back() + w;
					else if (random < 40)
						addition = drawVector.back() + e;
					else if (random < 60)
						addition = drawVector.back() + se;
					else if (random < 80 )
						addition = drawVector.back() + sw;
					else 
						addition = drawVector.back() + s;
					if (abs(addition.x - HorizontalWaypoint[i]) <= VerticalWaypoint[i] - addition.y)
						drawVector.push_back(addition);
				}
			}

		}


		
		if (GroundHit == 0 && drawVector.back().y > 798)
		{
			GroundHit = 1;
			clock.restart();
		}
		
		if (GroundHit == 1)
		{
			if (strikeCounter == 0 && growth < 100)
			{
				random2 = 5 + rand() % 7;
				for (auto& posVect : drawVector)
				{
					sf::Vector2f s = sf::Vector2f(size + growth, size + growth);
					bigLightning.setSize(s);
					bigLightning.setPosition(sf::Vector2f(float(posVect.x) - s.x/2, float(posVect.y) - s.y/2));
					window.draw(bigLightning);
					growth += 0.003f;
				}
			}
			else
			{
				if (clock.getElapsedTime().asMilliseconds() < 50)
				{
					if (strikeCounter % 2 == 0)
					{
						for (auto& posVect : drawVector)
						{
							lightningMedium.setPosition(sf::Vector2f(float(posVect.x) - mSize/2.f, float(posVect.y) - mSize/2.f));
							window.draw(lightningMedium);
						}
					}
					else
					{
						for (auto& posVect : drawVector)
						{
							lightningLarge.setPosition(sf::Vector2f(float(posVect.x) - lSize/2.f, float(posVect.y) - lSize/2.f));
							window.draw(lightningLarge);
						}
					}
				}
				if (clock.getElapsedTime().asMilliseconds() > rand() % 50 + 100)
				{
					strikeCounter++;
					clock.restart(); 
				}
			}
			if (strikeCounter > random2)
			{
				drawVector.clear();
				GroundHit = -1;
				strikeCounter = 0;
				growth = 0;
			}
		}

		if (GroundHit == 0)
		{
			for (auto& posVect : drawVector)
			{
				lightningSmall.setPosition(sf::Vector2f(float(posVect.x) - sSize/2.f, float(posVect.y) - sSize/2.f));
				window.draw(lightningSmall);
			}
		}

		window.setView(view);
		window.display();
		

		dt = deltaTime.restart().asSeconds(); // measure the time it took to complete all the calculations for this particular frame (dt=1/framerate) 
	}
	return 0;
}