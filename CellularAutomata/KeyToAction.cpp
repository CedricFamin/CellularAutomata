#include <fstream>
#include <sstream>

#include "KeyToAction.h"

namespace {
	float VIEW_SPEED = 10;
}

void ActionOnEscape(sf::RenderWindow & app, sf::View & view, World & world)
{
	app.close();
}

void ActionOnKeyUp(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.move(0, -VIEW_SPEED);
}

void ActionOnKeyDown(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.move(0, VIEW_SPEED);
}

void ActionOnKeyRight(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.move(VIEW_SPEED, 0);
}

void ActionOnKeyLeft(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.move(-VIEW_SPEED, 0);
}

void ActionOnSubstract(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.zoom(1.2f);
}

void ActionOnAdd(sf::RenderWindow & app, sf::View & view, World & world)
{
	view.zoom(.8f);
}

void ActionOnPageUp(sf::RenderWindow & app, sf::View & view, World & world)
{
	world.SetTickSize(world.GetTickSize() * 0.5f);
}

void ActionOnPageDown(sf::RenderWindow & app, sf::View & view, World & world)
{
	world.SetTickSize(world.GetTickSize() * 2.0f);
}

void ActionOnSpace(sf::RenderWindow & app, sf::View & view, World & world)
{
	world.TogglePause();
}

void ActionOnF5(sf::RenderWindow & app, sf::View & view, World & world)
{
	std::ofstream file;

	// Generate filename 
	std::stringstream filename("");
	filename << "../Save/" << world.GetMapName() << "-save-" << std::time(NULL) << ".casave";

	file.open(filename.str().c_str(), std::fstream::trunc | std::ios::out);
	if (!file.is_open())
	{
		std::cout << "Cannot open file : " << filename.str() << std::endl;
		return; 
	}
	boost::archive::text_oarchive oa(file);
	oa << world;
}
					
void ActionOnF1(sf::RenderWindow & app, sf::View & view, World & world)
{
	world.ToggleAutoMode();
}

void ActionOnF2(sf::RenderWindow & app, sf::View & view, World & world)
{
	world.ToggleDebug();
}

void ActionOnP(sf::RenderWindow & app, sf::View & view, World & world)
{
	sf::Vector2f position = app.mapPixelToCoords(sf::Mouse::getPosition(app), view);
	position /= Config::CellulSize;
	if (position.x >= 0.0f && position.y >= 0.0f && (int)position.x < world.GetX() && (int)position.y < world.GetY())
	{
		BaseObject * object = new Probe();
		World::ParamList params;
		params.push_back("oject");
		params.push_back("probe");
		{
			std::stringstream istr;
			istr << position.x;
			params.push_back(istr.str());
		}
		{
			std::stringstream istr;
			istr << position.y;
			params.push_back(istr.str());
		}
		
		{
			std::stringstream istr;
			istr << position.x;
			params.push_back(istr.str());
		}
		{
			std::stringstream istr;
			istr << position.y;
			params.push_back(istr.str());
		}

		object->Init(params);
		world.AddObject(object);
	}
}

KeyToAction::KeyToAction()
{
	FKeyToAction[sf::Keyboard::Escape] = ActionOnEscape;
	FKeyToAction[sf::Keyboard::Up] = ActionOnKeyUp;
	FKeyToAction[sf::Keyboard::Down] = ActionOnKeyDown;
	FKeyToAction[sf::Keyboard::Left] = ActionOnKeyLeft;
	FKeyToAction[sf::Keyboard::Right] = ActionOnKeyRight;
	FKeyToAction[sf::Keyboard::Subtract] = ActionOnSubstract;
	FKeyToAction[sf::Keyboard::Add] = ActionOnAdd;
	FKeyToAction[sf::Keyboard::PageUp] = ActionOnPageUp;
	FKeyToAction[sf::Keyboard::PageDown] = ActionOnPageDown;
	FKeyToAction[sf::Keyboard::Space] = ActionOnSpace;
	FKeyToAction[sf::Keyboard::F5] = ActionOnF5;
	FKeyToAction[sf::Keyboard::F1] = ActionOnF1;
	FKeyToAction[sf::Keyboard::F2] = ActionOnF2;
	FKeyToAction[sf::Keyboard::P] = ActionOnP;
}

KeyToAction::ActionType KeyToAction::operator[](sf::Keyboard::Key const & parKey)
{
	if (FKeyToAction.find(parKey) == FKeyToAction.end())
		return NULL;
	return FKeyToAction[parKey];
}
