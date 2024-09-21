#pragma once
#include <chrono>
#include <vector>
#include <functional>
#include <Vectors.hpp>
#include "../Events/Event.hpp"
#include "../Render/RenderUtils/RenderUtils.hpp"

class Notif {
public:

	std::string text;
	std::chrono::time_point<std::chrono::high_resolution_clock> time;
	Vec2 pos;

	Notif(std::string tex) {
		text = tex;
		time = std::chrono::high_resolution_clock::now();
		pos = PositionComponent(1, 1);
	}
};

class Client {
	static inline std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> LeftCPS = {};
	static inline std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> RightCPS = {};

	static inline std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();
	static inline std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
	static inline int frameCount = 0;

	static inline std::vector<Notif*> Notifications = {};

	static float lerp(float& current, float endValue, float delta) {
		current = current + (endValue - current) * delta;
		return current;
	}

	static void RenderNotifications() {
		int count = 0;
		Vec2 Size = SizeComponent(0.5, 0.1);
		float Spacing = Size.y * 0.2;
		Vec2 PosNotif = PositionComponent(1, 1);
		PosNotif.x -= Size.x;
		PosNotif.y -= Size.y * 2;
		for (auto Notif : Notifications) {

			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - Notif->time).count();

			if (Notif->pos.x > PositionComponent(1, 0).x) return;

			if (elapsedTime > 5)
				Notif->pos.x = lerp(Notif->pos.x, PositionComponent(1.1, 0).x, Delta * 0.3);
			else
				Notif->pos.x = lerp(Notif->pos.x, PosNotif.x, Delta * 0.3);

			Notif->pos.y = lerp(Notif->pos.y, PosNotif.y, Delta * 0.3);

			RndrUtils.RoundedRectFilled(Notif->pos, Size, ImColor(0.0f, 0.0f, 0.0f, 0.5f), 0);

			Vec2 PosText = Vec2(Notif->pos.x + Spacing, Notif->pos.y);
			RndrUtils.Text(PosText, Size / 2, IM_COL32_WHITE, "Notification", 0.45, 1);
			RndrUtils.Text(PosText + Vec2(0, Size.y/2), Size / 2, IM_COL32_WHITE, Notif->text, 0.25, 1);

			PosNotif.y -= Size.y + Spacing;
		}
	}
public:
	static void InitClientInfo() {
		EventDispatcher.listen<MouseEvent, nes::event_priority::FIRST>([&](MouseEvent& event) { 

			MousePos = Vec2(event.mouseX, event.mouseY);

			if (event.action == MouseAction::PRESS) {
				if (event.button == MouseButton::Left) {
					LeftCPS.insert(LeftCPS.begin(), std::chrono::high_resolution_clock::now());
					MouseClickLeft = true;
					MouseHoldLeft = true;
				}
				else if (event.button == MouseButton::Right) {
					RightCPS.insert(RightCPS.begin(), std::chrono::high_resolution_clock::now());
					MouseClickRight = true;
					MouseHoldRight = true;
				}
			}

			else if (event.action == MouseAction::RELEASE) {
				if (event.button == MouseButton::Left) {
					MouseHoldLeft = false;
				}
				else if (event.button == MouseButton::Right) {
					MouseHoldRight = false;
				}
			}

			else if (event.button == MouseButton::Scroll) {
				if (event.action == MouseAction::SCROLL_UP) ScrollUP = true;
				else ScrollDOWN = true;
			}
		});
		EventDispatcher.listen<RenderEvent, nes::event_priority::LAST>([&](RenderEvent& event) {
			frameCount++;

			auto currentTime = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
			std::chrono::duration<float> elapsed = currentTime - start;

			if (elapsed.count() >= 0.5f) {
				// Calculate frame rate based on elapsed time
				FPS = static_cast<int>(frameCount / elapsed.count());
				// Reset frame counter and update start time
				frameCount = 0;
				start = currentTime;
			}

			std::chrono::duration<float> frameTime = currentTime - lastTime;
			lastTime = currentTime;
			currentFrameRate = 1.0f / frameTime.count();

			Delta = 60 / currentFrameRate;

			Delta = min(Delta, 1);

			WindowSize = Vec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

			MouseClickLeft = false;
			MouseClickRight = false;
			ScrollUP = false;
			ScrollDOWN = false;
			KeyThisFrame = 0;
			RenderNotifications();
			});

		EventDispatcher.listen<KeyboardEvent, nes::event_priority::FIRST>([&](KeyboardEvent& event) {
			keypressed[event.key] = event.state;
			KeyThisFrame = event.key;
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

	static inline float currentFrameRate = 0;

	static inline Vec2 WindowSize = Vec2();
	static inline Vec2 ScreenSize = Vec2();

	static inline Vec2 MousePos = Vec2();

	static inline bool MouseClickLeft = false;
	static inline bool MouseClickRight = false;

	static inline bool MouseHoldLeft = false;
	static inline bool MouseHoldRight = false;

	static inline bool ScrollUP = false;
	static inline bool ScrollDOWN = false;

	static inline int KeyThisFrame = 0;

	static inline std::map<int, bool> keypressed = {};

	static void PushNotification(std::string text) {
		Notifications.push_back(new Notif(text));
	}
};