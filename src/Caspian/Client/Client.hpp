#pragma once
#include <chrono>
#include <vector>
#include <functional>
#include "../Events/Event.hpp"

class Client {
	static inline std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> LeftCPS = {};
	static inline std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> RightCPS = {};

	static inline std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();
	static inline int frameCount = 0;
public:
	static void InitClientInfo() {
		EventDispature.listen<MouseEvent>([&](MouseEvent& event) {
			if (event.action == MouseAction::PRESS) {
				if (event.button == MouseButton::Left) {
					LeftCPS.insert(LeftCPS.begin(), std::chrono::high_resolution_clock::now());
				}
				else if (event.button == MouseButton::Right) {
					RightCPS.insert(RightCPS.begin(), std::chrono::high_resolution_clock::now());
				}
			}
		});
		EventDispature.listen<RenderEvent>([&](RenderEvent& event) {
			frameCount++;

			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();

			if (elapsedTime >= 0.25) {
				FPS = frameCount / elapsedTime;
				frameCount = 0;
				lastTime = currentTime;
			}
			});
	}
	static inline int FPS = 0;

	static int GetRightCPS() {
		int cps = 0;
		for (auto x : RightCPS) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - x).count();

			if (elapsedTime < 1.0f) {
				cps++;
			}
		}
		return cps;
	}

	static int GetLeftCPS() {
		int cps = 0;
		for (auto x : LeftCPS) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - x).count();

			if (elapsedTime < 1.0f) {
				cps++;
			}
		}
		return cps;
	}

	static inline float Delta = 0;
};