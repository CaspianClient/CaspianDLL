#include "../Module.hpp"
#include <format>

class MemoryDisplay : public Module {
public:
	MemoryDisplay() : Module("Memory Display", "Shows your memory usage") {
		this->set("enabled", false, false);
		this->set("posX", 0, false);
		this->set("posY", 0, false);
		this->set("Size", 1, false);
		this->set("Rounding", 0, false);
		this->set("BGcolor", std::vector<float>{0, 0, 0, .5f}, false);
		this->set("TEXTcolor", std::vector<float>{1, 1, 1, 1}, false);
		this->set("UsePercentage", false, false);

		EventDispatcher.listen<RenderEvent>(renderEvent);
	}

	std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
		MEMORYSTATUSEX memory_status;
        memory_status.dwLength = sizeof(memory_status);
        GlobalMemoryStatusEx(&memory_status);
		float total_memory = static_cast<float>(memory_status.ullTotalPhys);
		float free_memory = static_cast<float>(memory_status.ullAvailPhys);
		float used_memory = total_memory - free_memory;

		int MemoryUsagePercent = (used_memory * 100 / total_memory);

		float MemoryUsedInGB = used_memory / (1024 * 1024 * 1024);
		float TotalMemoryInGB = total_memory / (1024 * 1024 * 1024);

		std::string text = "";

		if (get<bool>("UsePercentage"))
			text = "Mem: " + std::to_string(MemoryUsagePercent) + "%";
		else
			text = std::format("{:.1f}/{:.1f} GB", MemoryUsedInGB, TotalMemoryInGB);

		this->RenderHud(text);
	};

	void RenderSettings() override {
		AddSlider("Size", "Size", 0.5, 2.5);
		AddSlider("Rounding", "Rounding", 0, 1);
		AddColorPicker("BGcolor", "Background Color");
		AddColorPicker("TEXTcolor", "Text Color");
		AddToggle("UsePercentage", "Show Memory Usage in Percentage");
	}
};