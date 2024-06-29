#pragma once
#include "../Hook.hpp"
#include "../../../Events/Event.hpp"
#include "../../../SDK/SDKs/Render/MCUIRC.hpp"
#include "../../../SDK/SDK.hpp"
#include "../../../SDK/SDKs/Render/ScreenView/ScreenView.hpp"

class SetupAndRender : public Hook {
public:
	typedef void(__thiscall *detour)(ScreenView* a1, MCUIRC* mcuirc);
	static inline detour func_original = 0;

	SetupAndRender() : Hook("Setup And Render", "SetupAndRender") {
		this->HookFunc(callback, (void**)&func_original);
	}

	static void callback(ScreenView* a1, MCUIRC* mcuirc) {

		SDK::CI = mcuirc->getclientInstance();
		SDK::TopLayer = (a1->TopLayer() != "toast_screen" and a1->TopLayer() != "debug_screen" ? a1->TopLayer() : SDK::TopLayer);

		printf(SDK::TopLayer.c_str());
		
		return func_original(a1, mcuirc);
	}
};