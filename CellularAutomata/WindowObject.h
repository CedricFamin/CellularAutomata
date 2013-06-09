#pragma once
#include "BaseObject.h"

class WindowObject : public BaseObject
{
public:
	WindowObject(void);
	virtual ~WindowObject(void);

	virtual void Draw(sf::RenderWindow & app) const;
	virtual BaseObject * Clone() const;
	virtual void Update(World * parWorld);
	virtual void OnClick(World * parWorld, int parX, int parY);

	void SetEnable(bool parValue) { FEnable = parValue; }
	bool Enable() const { return FEnable; }


private:
	bool FEnable;
};

