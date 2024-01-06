#include "stdafx.h"
#include "ShowFPSCheat.h"


FPSCounter::FPSCounter()
{
}


FPSCounter::~FPSCounter()
{
}

// For internal use, do not modify.
int FPSCounter::AddRef()
{
	return DefaultRefCounted::AddRef();
}

// For internal use, do not modify.
int FPSCounter::Release()
{
	return DefaultRefCounted::Release();
}

void time_count(bool* start_counting, bool* finish_counting)
{
	while (*start_counting) {
		std::this_thread::sleep_for(std::chrono::high_resolution_clock::duration(std::chrono::seconds(1)));
		*finish_counting = true;
	}
}

// The method that receives the message. The first thing you should do is checking what ID sent this message...
bool FPSCounter::HandleMessage(uint32_t messageID, void* message)
{
	framerate++;
	if (finish_counting) {
		fps.assign_convert(to_string(framerate));
		if (WindowManager.GetMainWindow()->FindWindowByID(id("fps")) != nullptr)
			WindowManager.GetMainWindow()->FindWindowByID(id("fps"))->SetCaption(fps.c_str());
		framerate = 0;
		finish_counting = false;
	}
	return true;
}

ShowFPSCheat::ShowFPSCheat()
{
}


ShowFPSCheat::~ShowFPSCheat()
{
}


void ShowFPSCheat::ParseLine(const ArgScript::Line& line)
{
	auto option = line.GetArgumentAt(1);
	switch (mpFormatParser->ParseBool(option)) {
	case true:
	{
		start_counting = true;
		if (!alreadyCounting) {
			std::thread time_counter(time_count, &start_counting, &finish_counting);
			time_counter.detach();
			MessageManager.AddListener(new FPSCounter(), App::kMsgAppUpdate);
			fpsCounter = new UTFWin::UILayout();
			fpsCounter->LoadByName(u"FpsCounter");
			if (WindowManager.GetMainWindow()->FindWindowByID(id("FpsCounter")) != nullptr)
				fpsCounter->FindWindowByID(id("FpsCounter"))->AddWinProc(new UTFWin::SimpleLayout(UTFWin::kAnchorTop | UTFWin::kAnchorRight));
			fpsCounter->FindWindowByID(id("FpsCounter"))->SetArea({ -66, 0, 0, 25 });
			alreadyCounting = true;
		}
		else {
			App::ConsolePrintF("Already couting FPS.");
		}
		break;
	}
	case false:
		start_counting = false;
		alreadyCounting = false;
		MessageManager.RemoveListener(&FPSCounter(), App::kMsgAppUpdate);
		if (WindowManager.GetMainWindow()->FindWindowByID(id("fps")) != nullptr)
			WindowManager.GetMainWindow()->RemoveWindow(fpsCounter->FindWindowByID(id("FpsCounter")));
		else
			App::ConsolePrintF("FPS Counter hasn't been turned on yet.");
		fpsCounter = nullptr;
		break;
	}
}

const char* ShowFPSCheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Show current FPS.";
	}
	else {
		return "showFPS <option>\noption: on/off";
	}
}