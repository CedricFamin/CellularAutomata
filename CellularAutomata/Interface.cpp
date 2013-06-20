#include <sstream>

#include "Interface.h"

#include "World.h"


Interface::Interface(void)
	: FTemperatureSelected(0)
	, FAuto(false)
{
	FDefaultFont.loadFromFile("../font/arial.ttf");
}


Interface::~Interface(void)
{
}

void Interface::Draw(sf::RenderWindow& app, World const & parWorld) const
{
	{ 
		// background 
		sf::Vector2f size((float)app.getSize().x, 45.0f);
		sf::RectangleShape shape(size);
		shape.setPosition(0, 0);
		shape.setFillColor(sf::Color(0, 0, 0, 155));
		shape.setOutlineColor(sf::Color(200, 200, 200, 200));
		shape.setOutlineThickness(-3.0f);
		app.draw(shape);
	}

	// stat
	{ // tick
		std::ostringstream strText;
		strText << "TickSize : " << parWorld.GetTickSize();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(10, 10);
		app.draw(txt);
	}
	{ // average temp
		std::ostringstream strText;
		strText << "Average Temp : " << parWorld.GetCelluls().GetAverageTemp();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(10, 25);
		app.draw(txt);
	}
	{ // average temp
		std::ostringstream strText;
		strText << "Delta temp : " << parWorld.GetCelluls().GetDeltaTemp();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(150, 25);
		app.draw(txt);
	}
	{ // Tick
		std::ostringstream strText;
		strText << "Tick : " << parWorld.GetTick();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(150, 10);
		app.draw(txt);
	}
	{ // rectangle pour la chaleur
		for (unsigned int i = 0; i <= 10; ++i)
		{
			sf::RectangleShape rect(sf::Vector2f(20.0f, 20.0f));
			rect.setPosition(400.0f + 30.0f * i, 15.0f);
			float temperature = (float)i;
			int colorR = (int)(temperature / 10.0f * 255);
			int colorB = 255 - colorR;
			int colorV = 255 - abs(colorR - colorB);
			rect.setFillColor(sf::Color(colorR, colorV, colorB));
			if (i == FTemperatureSelected)
				rect.setOutlineColor(sf::Color::White);
			else
				rect.setOutlineColor(sf::Color(colorR/2, colorV/2, colorB/2));
			rect.setOutlineThickness(-3.0f);
			app.draw(rect);
		}
	}

	{ // Boutton Autpo
		std::ostringstream strText;
		strText << "heating regulation system : " << FAuto;
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition((float)app.getSize().x - 200.0f, 10.0f);
		app.draw(txt);
	}
}

bool Interface::OnClick(World * parWorld, int parX, int parY)
{
	{ // rectangle pour la chaleur
		for (unsigned int i = 0; i <= 10; ++i)
		{
			std::pair<int, int> position(400 + 30 * i, 15);
			sf::RectangleShape rect(sf::Vector2f(20.0f, 20.0f));
			if (position.first <= parX && parX - position.first <= 20)
			{
				if (position.second <= parY && parY - position.second <= 20)
				{
					FTemperatureSelected = i;
					return true;
				}
			}
		}
	}
	return false;
}