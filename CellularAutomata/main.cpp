#include <iostream>
#include <fstream>
#include <sstream>

// SFML Includes
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <boost/program_options.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "World.h"
#include "ObjectFactory.h"
#include "AllObject.h"
#include "KeyToAction.h"

namespace {
	static unsigned int WindowXSize = 960;
	static unsigned int WindowYSize = 640;
}

ObjectFactory * CreateObjectFactory()
{
    ObjectFactory * factory = new ObjectFactory();
	factory->RegisterObject(new WallObject());
	factory->RegisterObject(new HeatObject());
	factory->RegisterObject(new WindowObject());
	factory->RegisterObject(new Probe());
	factory->RegisterObject(new StoneWallObject());
    return factory;
}

int Configure(World & parWorld, int ac, char **av)
{
    boost::program_options::options_description desc("Options");
    desc.add_options()
    ("help", "produce help message")
    ("map", boost::program_options::value<std::string>(), "Load a map")
    ("load", boost::program_options::value<std::string>(), "Load a save file")
    ("ticksize", boost::program_options::value<float>()->default_value(0.2f), "Set the default tick size")
    ;
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(ac, av, desc), vm);
    boost::program_options::notify(vm);
    
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }
    
    if (!vm.count("load") && !vm.count("map"))
    {
        std::cout << "Option 'load' or 'map' mandatory" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    
    if (vm.count("load") && vm.count("map"))
    {
        std::cout << "Cannot use option 'load' and 'map'" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    
    if (vm.count("load"))
    {
        std::ifstream file;
		file.open(vm["load"].as<std::string>().c_str());
		if (!file.is_open())
		{
			std::cout << "Cannot open file : " << vm["load"].as<std::string>() << std::endl;
			return 1;
		}
		boost::archive::text_iarchive oa(file);
		oa >> parWorld;
    }
    
    if (vm.count("map"))
    {
        if (!parWorld.LoadWorld(vm["map"].as<std::string>()))
		{
			std::cout << "FATAL ERROR : EXIT" << std::endl;
			return 1;
		}
    }
    
    parWorld.SetTickSize(vm["ticksize"].as<float>());
    
    return 0;
}

int main(int ac, char **av)
{
    KeyToAction keyAction;
	ObjectFactory * factory = CreateObjectFactory();

	World world(*factory);
	
    if (Configure(world, ac, av))
        return 1;
    
	//world.AfterLoad();

	sf::RenderWindow app(sf::VideoMode(WindowXSize, WindowYSize, 32), world.GetMapName(), sf::Style::Titlebar | sf::Style::Close);
	sf::Vector2f center((float)world.GetX()/2, (float)world.GetY()/2);
    sf::Vector2f halfsize((float)world.GetX(), (float)world.GetY());
	center *= Config::CellulSize;
	halfsize *= Config::CellulSize;
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
					// Todo faire en sorte de pouvoir garder un clic enfoncer pour setter les zone de chaleur.
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						app.setView(view);
						sf::Vector2i coord = sf::Mouse::getPosition(app);
						world.OnClick(app, coord.x, coord.y);
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
    
    delete factory;
    return 0;
}