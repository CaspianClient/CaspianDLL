#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class Tablist : public Module {
public:
	bool EnabledTalbist = false;
	Tablist() : Module("Tablist", "Shows the players on the server.") {
		this->set("enabled", false, false);
		this->set("Size", 1, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);
		this->set("keybind", 9, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
		EventDispatcher.listen<KeyboardEvent>(KeyEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		if (!CanRenderHud() or !EnabledTalbist) {
			return;
		}

		std::unordered_map<UUID, PlayerListEntry>& players = SDK::CI->getLocalPlayer()->level->getPlayerList();

		int rows = players.size() / 10;
		if (players.size() % 10 != 0) {
			rows++;
		}
		int cols = (players.size() >= 10) ? 10 : players.size();

		float SizeModifier = get<float>("Size");
		Vec2 SizeEachCell = SizeComponent(0.2, 0.04) * SizeModifier;
		Vec2 Size = Vec2(SizeEachCell.x * rows, SizeEachCell.y * cols);
		Vec2 Pos = PositionComponent(0.5, 0.05);
		Pos.x -= Size.x / 2;

		float xSpacing = SizeEachCell.y * 0.25;

		RndrUtils.RoundedRectFilled(Pos, Size, RndrUtils.VecToImcolor(get<std::vector<float>>("BGcolor")));

		Vec2 posNames = Pos;
		int count = 0;
		for (auto& pair : players) {
			count++;
			RndrUtils.Text(posNames + Vec2(xSpacing, 0), SizeEachCell, RndrUtils.VecToImcolor(get<std::vector<float>>("TEXTcolor")), Utils::StripColorCode(pair.second.name), 0.25 * SizeModifier, 1);
			if (count < 10) {
				posNames.y += SizeEachCell.y;
			}
			else {
				posNames.y = Pos.y;
				posNames.x += SizeEachCell.x;
				count = 0;
			}
		}

		for (int i = 0; i < (rows + 1); i++) {
			RndrUtils.Line(Vec2(Pos.x + (SizeEachCell.x * i), Pos.y), Vec2(Pos.x + (SizeEachCell.x * i), Pos.y + (SizeEachCell.y * cols)), ImColor(0.5, 0.5, 0.5, 1.0f), SizeEachCell.y * 0.05);
		}
		for (int i = 0; i < (cols + 1); i++) {
			RndrUtils.Line(Vec2(Pos.x, Pos.y + (SizeEachCell.y * i)), Vec2(Pos.x + (SizeEachCell.x * rows), Pos.y + (SizeEachCell.y * i)), ImColor(0.5, 0.5, 0.5, 1.0f), SizeEachCell.y * 0.05);
		}
	};

	std::function<void(KeyboardEvent &)> KeyEvent = [&](KeyboardEvent &event)
	{
		if (event.key == get<int>("keybind"))
		{
			EnabledTalbist = event.state;
		};
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
	}
};