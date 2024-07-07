#pragma once
enum class MouseAction {
    PRESS = 0,
    RELEASE = 1,
    SCROLL_UP = 0x78,
    SCROLL_DOWN = 0x80
};

enum MouseButton { None = 0, Left = 1, Right = 2, Middle = 3, Scroll = 4, Button5 = 5, Button6 = 6 };

class MouseEvent {
public:
    MouseButton button;
    MouseAction action;
    short mouseX;
    short mouseY;
    bool mCancel = false;
};

class MouseScrollEvent {
public:
    bool ScrollUp;
    bool mCancel;
};