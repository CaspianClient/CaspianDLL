#pragma once
#include "../Module.hpp"
#include "../../Events/Event.hpp"
#include "../../Config/ConfigManager.hpp"
#include "../../SDK/SDK.hpp"
#include <functional>

class Keystrokes : public Module {
public:
	Keystrokes() : Module("Keystokes", "Shows your Keystrokes")
	{
		this->set("enabled", true, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("anim", 1, false);
		this->set("BGcolorDisabled", std::vector<float>{0, 0, 0, .5f});
		this->set("TEXTcolorDisabled", std::vector<float>{1, 1, 1, 1});
		this->set("BGcolorEnabled", std::vector<float>{1, 1, 1, .5f});
		this->set("TEXTcolorEnabled", std::vector<float>{1, 1, 1, 1});

		resetButtonStates();

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddSlider("anim", "Animation Speed", 0, 3);
		AddColorPicker("BGcolorDisabled", "Disabled Background Color");
		AddColorPicker("TEXTcolorDisabled", "Disabled Text Color");
		AddColorPicker("BGcolorEnabled", "Enabled Background Color");
		AddColorPicker("TEXTcolorEnabled", "Enabled Text Color");
	}

	struct ButtonState {
        float size = 0;
        ImColor bgColor;
        ImColor textColor;
    };

    ButtonState W, A, S, D, LC, RC, SPACE;

	void resetButtonStates() {
        ImColor initialBgColor = RndrUtils.VecToImcolor(get<std::vector<float>>("BGcolorDisabled"));
        ImColor initialTextColor = RndrUtils.VecToImcolor(get<std::vector<float>>("TEXTcolorDisabled"));

        W = A = S = D = LC = RC = SPACE = {0, initialBgColor, initialTextColor};
    }

	void updateButtonState(ButtonState& state, bool active, float deltaTime) {
		ImColor EnabledText = RndrUtils.VecToImcolor(this->get<std::vector<float>>("TEXTcolorEnabled"));
		ImColor DisabledBG = ImColor(0, 0, 0, 0);
		ImColor ActualDisabledBG = RndrUtils.VecToImcolor(this->get<std::vector<float>>("BGcolorDisabled"));
		ImColor DisabledText = RndrUtils.VecToImcolor(this->get<std::vector<float>>("TEXTcolorDisabled"));

        state.textColor = Utils::LerpImColor(state.textColor, active ? EnabledText : DisabledText, deltaTime * 0.6);
        state.size = Utils::lerp(state.size, active ? 1 : 0, deltaTime);
        state.bgColor = Utils::LerpImColor(state.bgColor, active ? DisabledBG : ActualDisabledBG, deltaTime);
    }

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		if (!get<bool>("enabled"))
			return;

		if (!SDK::CI)
			return;

		if (!SDK::CI->getLocalPlayer())
			return;

		auto moveInput = SDK::CI->getLocalPlayer()->getMoveInputHandler();

		PositionComponent pos = PositionComponent(this->get<float>("posX"), this->get<float>("posY"));
		float Size = this->get<float>("Size");

		SizeComponent ButtonSizes(0.05 * Size, 0.05 * Size);
		SizeComponent Spacing(0.003 * Size, 0.003 * Size);
		Vec2 MBsizes = Vec2((ButtonSizes.x * 1.5) + (Spacing.x / 2), ButtonSizes.y);
		Vec2 SBsize = Vec2((ButtonSizes.x * 3) + (Spacing.x * 2), ButtonSizes.y);

		Vec2 TotalModuleSize = Vec2((ButtonSizes.x * 3) + (Spacing.x * 2), (ButtonSizes.x * 4) + (Spacing.x * 3));

		ImColor EnabledBG = RndrUtils.VecToImcolor(this->get<std::vector<float>>("BGcolorEnabled"));
		ImColor EnabledText = RndrUtils.VecToImcolor(this->get<std::vector<float>>("TEXTcolorEnabled"));
		ImColor DisabledBG = ImColor(0, 0, 0, 0);
		ImColor ActualDisabledBG = RndrUtils.VecToImcolor(this->get<std::vector<float>>("BGcolorDisabled"));
		ImColor DisabledText = RndrUtils.VecToImcolor(this->get<std::vector<float>>("TEXTcolorDisabled"));

		handleDragging(pos, TotalModuleSize);

		float deltaTime = Client::Delta * 0.5f * get<float>("anim");

        updateButtonState(W, moveInput->forward, deltaTime);
        updateButtonState(A, moveInput->left, deltaTime);
        updateButtonState(S, moveInput->backward, deltaTime);
        updateButtonState(D, moveInput->right, deltaTime);
        updateButtonState(LC, Client::MouseHoldLeft, deltaTime);
        updateButtonState(RC, Client::MouseHoldRight, deltaTime);
        updateButtonState(SPACE, moveInput->jumping, deltaTime);

		Vec2 pos_W = Vec2(pos.x + ButtonSizes.x + Spacing.x, pos.y);

		Vec2 pos_A = Vec2(pos.x, pos.y + ButtonSizes.y + Spacing.y);

		Vec2 pos_S = Vec2(pos.x + ButtonSizes.x + Spacing.x, pos.y + ButtonSizes.y + Spacing.y);

		Vec2 pos_D = Vec2(pos.x + ((ButtonSizes.x + Spacing.x) * 2), pos.y + ButtonSizes.y + Spacing.y);

		Vec2 pos_LC = Vec2(pos.x, pos.y + ((ButtonSizes.y + Spacing.y) * 2));

		Vec2 pos_RC = Vec2(pos.x + MBsizes.x + Spacing.x, pos.y + ((ButtonSizes.y + Spacing.y) * 2));

		Vec2 pos_SPACE = Vec2(pos.x, pos.y + ((ButtonSizes.y + Spacing.y) * 3));

		
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes, pos_W), ButtonSizes, W.bgColor, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes, pos_A), ButtonSizes, A.bgColor, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes, pos_S), ButtonSizes, S.bgColor, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes, pos_D), ButtonSizes, D.bgColor, 10);

		RndrUtils.RoundedRectFilled(Utils::CenterRect(MBsizes, MBsizes, pos_LC), MBsizes, LC.bgColor, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(MBsizes, MBsizes, pos_RC), MBsizes, RC.bgColor, 10);

		RndrUtils.RoundedRectFilled(Utils::CenterRect(SBsize, SBsize, pos_SPACE), SBsize, SPACE.bgColor, 10);


		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes * W.size, pos_W), ButtonSizes * W.size, EnabledBG, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes * A.size, pos_A), ButtonSizes * A.size, EnabledBG, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes * S.size, pos_S), ButtonSizes * S.size, EnabledBG, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(ButtonSizes, ButtonSizes * D.size, pos_D), ButtonSizes * D.size, EnabledBG, 10);

		RndrUtils.RoundedRectFilled(Utils::CenterRect(MBsizes, MBsizes * LC.size, pos_LC), MBsizes * LC.size, EnabledBG, 10);
		RndrUtils.RoundedRectFilled(Utils::CenterRect(MBsizes, MBsizes * RC.size, pos_RC), MBsizes * RC.size, EnabledBG, 10);

		RndrUtils.RoundedRectFilled(Utils::CenterRect(SBsize, SBsize * SPACE.size, pos_SPACE), SBsize * SPACE.size, EnabledBG, 10);



		RndrUtils.Text(pos_W, ButtonSizes, W.textColor, "W", 0.4 * Size, 2);
		RndrUtils.Text(pos_A, ButtonSizes, A.textColor, "A", 0.4 * Size, 2);
		RndrUtils.Text(pos_S, ButtonSizes, S.textColor, "S", 0.4 * Size, 2);
		RndrUtils.Text(pos_D, ButtonSizes, D.textColor, "D", 0.4 * Size, 2);

		RndrUtils.Text(pos_LC, MBsizes, LC.textColor, std::to_string(Client::GetLeftCPS()), 0.3 * Size, 2);
		RndrUtils.Text(pos_RC, MBsizes, RC.textColor, std::to_string(Client::GetRightCPS()), 0.3 * Size, 2);

		RndrUtils.Text(pos_SPACE, SBsize, SPACE.textColor, "-----", 0.4 * Size, 2);
	};
};