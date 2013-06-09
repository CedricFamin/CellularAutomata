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
#include "WallObject.h"
#include "HeatObject.h"
#include "WindowObject.h"

namespace {
	static int VIEW_SPEED = 10;
	static unsigned int WindowXSize = 960;
	static unsigned int WindowYSize = 640;
	static std::string MapFileName = "../Map/BaseMap.map";
}

void SaveWorld(World & parWorld, std::string const & parFilename)
{
	std::ofstream file;

	// Generate filename 
	std::stringstream filename("");
	filename << "../Save/save-" << std::time(NULL) << ".save";

	file.open(filename.str(), std::fstream::trunc | std::ios::out);
	if (!file.is_open())
	{
		std::cout << "Cannot open file : " << filename.str() << std::endl;
		return; 
	}
	boost::archive::text_oarchive oa(file);
	oa << parWorld;
}

int main(int ac, char **av)
{
 
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

    sf::RenderWindow app(sf::VideoMode(WindowXSize, WindowYSize, 32), "Cellular Automata", sf::Style::Titlebar | sf::Style::Close);
	sf::Vector2f center((float)world.GetX()/2, (float)world.GetY()/2);
    sf::Vector2f halfsize((float)world.GetX(), (float)world.GetY());
    sf::View view(center, halfsize);
    float zoom = 1.0f;
	view.zoom(1.0f);
	app.setView(view);
	float tickSize = .01f;
	sf::Clock deltaClock;
	bool pause = false;
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
                switch (event.key.code)
                {
					case sf::Keyboard::Escape :
                        app.close();
                        break;
                    case sf::Keyboard::Up :
                        view.move(0, -VIEW_SPEED / zoom);
                        break;
                    case sf::Keyboard::Down :
                        view.move(0, VIEW_SPEED / zoom);
                        break;
                    case sf::Keyboard::Right :
                        view.move(VIEW_SPEED / zoom, 0);
                        break;
                    case sf::Keyboard::Left :
                        view.move(-VIEW_SPEED / zoom, 0);
                        break;
                    case sf::Keyboard::Subtract:
                        view.zoom(1.2f);
                        break;
					case sf::Keyboard::Add:
                        view.zoom(0.8f);
                        break;
					case sf::Keyboard::Space:
						pause = !pause;
						break;
					case sf::Keyboard::F5:
						SaveWorld(world, MapFileName);
						break;
                    default:
                        break;
                }
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
		if (!pause && deltaClock.getElapsedTime().asSeconds() > tickSize)
		{
			world.OnTick();
			deltaClock.restart();
		}

		world.Draw(app);
		app.setView(view);
		app.display();
    }
    return 0;
}