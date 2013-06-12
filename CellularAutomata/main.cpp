#include <iostream>
#include <fstream>
#include <sstream>

// SFML Includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>

#include "World.h"
#include "ObjectFactory.h"
#include "AllObject.h"
#include "KeyToAction.h"

namespace {
	static unsigned int WindowXSize = 960;
	static unsigned int WindowYSize = 640;
	static std::string MapFileName = "../Map/BaseMap.map";
}

int main(int ac, char **av)
{
	KeyToAction keyAction;
	ObjectFactory factory;
	factory.RegisterObject(new WallObject());
	factory.RegisterObject(new HeatObject());
	factory.RegisterObject(new WindowObject());

	World world(factory);
	if (!world.LoadWorld(MapFileName))
	{
		std::cout << "FATAL ERROR : EXIT" << std::endl;
		return 1;
	}

	sf::RenderWindow app(sf::VideoMode(WindowXSize, WindowYSize, 32), world.GetMapName(), sf::Style::Titlebar | sf::Style::Close);
	sf::Vector2f center((float)world.GetX()/2, (float)world.GetY()/2);
    sf::Vector2f halfsize((float)world.GetX(), (float)world.GetY());
    sf::View view(center, halfsize);
	view.zoom(1.0f);
	app.setView(view);
	sf::Clock deltaClock;
	while (app.isOpen())
    {
        sf::Event event;
		while (app.pollEvent(event))
        {
			switch (event.type)
            {
                case sf::Event::Closed :
                    app.close();
                    break;
                case sf::Event::KeyPressed :
					if (keyAction[event.key.code])
						keyAction[event.key.code](app, view, world);
				case sf::Event::MouseButtonPressed:
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2f realPosition = app.mapPixelToCoords(sf::Mouse::getPosition(app), view);
						world.OnClick((int)realPosition.x, (int)realPosition.y);
					}

				}
                default :
                    break;
            }
        }
        
		app.clear();
		if (!world.Pause() && deltaClock.getElapsedTime().asSeconds() > world.GetTickSize())
		{
			world.OnTick();
			deltaClock.restart();
		}
		app.setView(view);
		world.Draw(app);
		app.setView(app.getDefaultView());
		world.DrawInterface(app);
		app.display();
    }
    return 0;
}