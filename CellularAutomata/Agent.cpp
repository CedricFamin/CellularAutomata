#include "Agent.h"

#include "Probe.h"
#include "HeatObject.h"
#include "WindowObject.h"
#include "Messages.h"

// --------------------------------------------
// DistributorAgent
// --------------------------------------------
DistributorHeatAgent::DistributorHeatAgent(SMAHeat * parSMA, HeatObject & parHeat)
	: Agent(parSMA, parHeat.Position().MinX<int>() , parHeat.Position().MinY<int>())
	, FHeatObject(parHeat)
{
}

// --------------------------------------------
// --------------------------------------------
DistributorHeatAgent::~DistributorHeatAgent()
{
}

// --------------------------------------------
// --------------------------------------------
void DistributorHeatAgent::ReadAndWriteSomething(Blackboard & parBlackBoard)
{
	FHeatObject.SetEnable(false);

	Blackboard::MessageList messages = parBlackBoard.GetMessages();
	for (Message * message : messages)
	{
		message->Decode(this);
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorHeatAgent::Read(TempDemand* parMessage)
{
	// est ce que l'on a besoin de temperature ?
	if (parMessage->InitialTemp < parMessage->WishTemp)
	{
		// est ce que je suis deja allumer ?
		if (!FHeatObject.Enable())
		{
			// est ce que je peux chauffer la cible ?
			if (FSMA->GetDistanceBetween(this, parMessage->source) >= 0)
				FHeatObject.SetEnable(true);
		}
	}
}


// --------------------------------------------
// DistributorColdAgent
// --------------------------------------------
DistributorColdAgent::DistributorColdAgent(SMAHeat * parSMA, WindowObject & parObject)
	: Agent(parSMA, parObject.Position().MinX<int>() , parObject.Position().MinY<int>())
	, FWindowObject(parObject)
{
}

// --------------------------------------------
// --------------------------------------------
DistributorColdAgent::~DistributorColdAgent()
{
}

// --------------------------------------------
// --------------------------------------------
void DistributorColdAgent::ReadAndWriteSomething(Blackboard & parBlackBoard)
{
	FWindowObject.SetEnable(false);

	Blackboard::MessageList messages = parBlackBoard.GetMessages();
	for (Message * message : messages)
	{
		message->Decode(this);
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorColdAgent::Read(TempDemand* parMessage)
{
	// est ce que l'on a besoin de fraicheur ?
	if (parMessage->InitialTemp > parMessage->WishTemp)
	{
		// est ce que je suis deja allumer ?
		if (!FWindowObject.Enable())
		{
			// est ce que je peux chauffer la cible ?
			if (FSMA->GetDistanceBetween(this, parMessage->source) >= 0)
				FWindowObject.SetEnable(true);
		}
	}
}


// --------------------------------------------
// EnvironmentalAgent
// --------------------------------------------
EnvironmentalAgent::EnvironmentalAgent(SMAHeat * parSMA, Probe & parProbe)
	: Agent(parSMA, parProbe.Position().MinX<int>() , parProbe.Position().MinY<int>())
	, FProbe(parProbe)
{
}

// --------------------------------------------
// --------------------------------------------
EnvironmentalAgent::~EnvironmentalAgent() {}

// --------------------------------------------
// --------------------------------------------
void EnvironmentalAgent::ReadAndWriteSomething(Blackboard & parBlackBoard)
{
	if (std::fabsf(FProbe.GetTemp() - 5.0f) > 1.0f)
	{
		TempDemand* message = new TempDemand;
		message->source = this;
		message->InitialTemp = FProbe.GetTemp();
		message->WishTemp = 5.0f;
		parBlackBoard.Write(message);
	}
}
