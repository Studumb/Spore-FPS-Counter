#pragma once

#include <Spore\BasicIncludes.h>
#include <chrono>
#include <thread>

#define FPSCounterPtr intrusive_ptr<FPSCounter>

static bool start_counting;
static bool finish_counting;
static void time_count(bool* start_counting, bool* finish_counting);

class FPSCounter
	: public App::IMessageListener
	, public DefaultRefCounted
{	
public:
	static const uint32_t TYPE = id("FPSCounter");
	int framerate{ 0 };
	string16 fps;

	FPSCounter();
	~FPSCounter();

	int AddRef() override;
	int Release() override;

	// This is the function you have to implement, called when a message you registered to is sent.
	bool HandleMessage(uint32_t messageID, void* message) override;
};

class ShowFPSCheat 
	: public ArgScript::ICommand
{
public:
	UILayoutPtr fpsCounter;
	bool alreadyCounting = false;
	ShowFPSCheat();
	~ShowFPSCheat();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

