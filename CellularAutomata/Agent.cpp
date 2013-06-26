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
		message->Decode(parBlackBoard, this);
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorHeatAgent::Read(Blackboard & parBlackBoard, TempDemand* parMessage)
{
	// est ce que l'on a besoin de temperature ?
	if (parMessage->InitialTemp < parMessage->WishTemp)
	{
		// est ce que je suis deja allumer ?
		if (!FHeatObject.Enable())
		{
			// est ce que je peux chauffer la cible ?
			int dist = FSMA->GetDistanceBetween(this, parMessage->source);
			if (dist >= 0)
			{
				HeatProposition * proposition = new HeatProposition();
				proposition->source = this;
				proposition->DistWithDestination = dist;
				proposition->Demand = parMessage;
				parBlackBoard.Write(proposition);
			}
		}
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorHeatAgent::Execute()
{
	FHeatObject.SetEnable(true);
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
		message->Decode(parBlackBoard, this);
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorColdAgent::Read(Blackboard & parBlackBoard, TempDemand* parMessage)
{
	// est ce que l'on a besoin de fraicheur ?
	if (parMessage->InitialTemp > parMessage->WishTemp)
	{
		// est ce que je suis deja allumer ?
		if (!FWindowObject.Enable())
		{
			// est ce que je peux refroidir la cible ?
			int dist = FSMA->GetDistanceBetween(this, parMessage->source);
			if (dist >= 0)
			{
				ColdProposition * proposition = new ColdProposition();
				proposition->source = this;
				proposition->DistWithDestination = dist;
				proposition->Demand = parMessage;
				parBlackBoard.Write(proposition);
			}
		}
	}
}

// --------------------------------------------
// --------------------------------------------
void DistributorColdAgent::Execute()
{
	FWindowObject.SetEnable(true);
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

// --------------------------------------------
// DecisionalAgent
// --------------------------------------------
DecisionalAgent::DecisionalAgent(SMAHeat * parSMA)
	: Agent(parSMA, -1, -1)
{
}

// --------------------------------------------
// --------------------------------------------
DecisionalAgent::~DecisionalAgent()
{
}

// --------------------------------------------
// --------------------------------------------
void DecisionalAgent::ReadAndWriteSomething(Blackboard & parBlackBoard)
{
	FDemandWithBestProposition.clear();
	FColdDemandWithBestProposition.clear();
	Blackboard::MessageList messages = parBlackBoard.GetMessages();
	for (Message * message : messages)
	{
		message->Decode(parBlackBoard, this);
	}

	for (auto bestHeat : FDemandWithBestProposition)
	{
		bestHeat.second->Execute();
	}

	for (auto bestHeat : FColdDemandWithBestProposition)
	{
		bestHeat.second->Execute();
	}
}

// --------------------------------------------
// --------------------------------------------
void DecisionalAgent::Read(Blackboard & parBlackBoard, HeatProposition* parMessage)
{
	if (FDemandWithBestProposition.find(parMessage->Demand) == FDemandWithBestProposition.end())
	{
		FDemandWithBestProposition[parMessage->Demand] = parMessage;
	}
	else if (FDemandWithBestProposition[parMessage->Demand]->DistWithDestination > parMessage->DistWithDestination)
	{
		FDemandWithBestProposition[parMessage->Demand] = parMessage;
	}

}

// --------------------------------------------
// --------------------------------------------
void DecisionalAgent::Read(Blackboard & parBlackBoard, ColdProposition* parMessage)
{
	if (FColdDemandWithBestProposition.find(parMessage->Demand) == FColdDemandWithBestProposition.end())
	{
		FColdDemandWithBestProposition[parMessage->Demand] = parMessage;
	}
	else if (FColdDemandWithBestProposition[parMessage->Demand]->DistWithDestination > parMessage->DistWithDestination)
	{
		FColdDemandWithBestProposition[parMessage->Demand] = parMessage;
	}

}