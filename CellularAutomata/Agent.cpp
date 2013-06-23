#include "Agent.h"

#include "Probe.h"
#include "HeatObject.h"
#include "Messages.h"

// --------------------------------------------
// DistributorAgent
// --------------------------------------------
DistributorAgent::DistributorAgent(SMAHeat * parSMA, HeatObject & parHeat)
	: Agent(parSMA, parHeat.Position().MinX<int>() / 10, parHeat.Position().MinY<int>() / 10)
	, FHeatObject(parHeat)
{
}

// --------------------------------------------
// --------------------------------------------
DistributorAgent::~DistributorAgent()
{
}

// --------------------------------------------
// --------------------------------------------
void DistributorAgent::ReadAndWriteSomething(Blackboard & parBlackBoard)
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
void DistributorAgent::Read(TempDemand* parMessage)
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
// EnvironmentalAgent
// --------------------------------------------
EnvironmentalAgent::EnvironmentalAgent(SMAHeat * parSMA, Probe & parProbe)
	: Agent(parSMA, parProbe.Position().MinX<int>() / 10, parProbe.Position().MinY<int>() / 10)
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
