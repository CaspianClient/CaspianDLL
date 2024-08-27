#include "../SettingsMenu.hpp"
#include "../../RenderUtils.hpp"
#include "../../../../Utils/Utils.hpp"
#include <math.h>

Vec3 RGBToHSV(float r, float g, float b)
{
    Vec3 hsv;
    float min, max, delta;
    min = r < g ? r : g;
    min = min < b ? min : b;

    max = r > g ? r : g;
    max = max > b ? max : b;

    hsv.z = max;                                // v
    delta = max - min;
    if (delta < 0.00001f)
    {
        hsv.y = 0.0f;
        hsv.x = 0.0f; // undefined, maybe nan?
        return hsv;
    }
    if (max > 0.0f) {
        hsv.y = (delta / max);                  // s
    } else {
        hsv.y = 0.0f;
        hsv.x = NAN; // undefined
        return hsv;
    }
    if (r >= max)
        hsv.x = (g - b) / delta;        // between yellow & magenta
    else
    if (g >= max)
        hsv.x = 2.0f + (b - r) / delta;  // between cyan & yellow
    else
        hsv.x = 4.0f + (r - g) / delta;  // between magenta & cyan

    hsv.x *= 60.0f;                              // degrees

    if (hsv.x < 0.0f)
        hsv.x += 360.0f;

    return hsv;
}

ImColor HSVToRGB(float h, float s, float v)
{
    ImColor rgb;
    int i;
    float f, p, q, t;

    if (s == 0.0f)
    {
        // achromatic (grey)
        rgb.Value.x = rgb.Value.y = rgb.Value.z = v;
        return rgb;
    }

    h /= 60.0f;            // sector 0 to 5
    i = (int)floor(h);
    f = h - i;          // factorial part of h
    p = v * (1.0f - s);
    q = v * (1.0f - s * f);
    t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0:
        rgb.Value.x = v;
        rgb.Value.y = t;
        rgb.Value.z = p;
        break;
    case 1:
        rgb.Value.x = q;
        rgb.Value.y = v;
        rgb.Value.z = p;
        break;
    case 2:
        rgb.Value.x = p;
        rgb.Value.y = v;
        rgb.Value.z = t;
        break;
    case 3:
        rgb.Value.x = p;
        rgb.Value.y = q;
        rgb.Value.z = v;
        break;
    case 4:
        rgb.Value.x = t;
        rgb.Value.y = p;
        rgb.Value.z = v;
        break;
    default:        // case 5:
        rgb.Value.x = v;
        rgb.Value.y = p;
        rgb.Value.z = q;
        break;
    }

    rgb.Value.w = 255;

    return rgb;
}

void SettingsMenu::ColorPicker(Vec2 pos, ImColor& color) { 
    SizeComponent hueBarSize(0.03, 0.25);
    SizeComponent PickerRectSize(0.35, 0.25);

    SizeComponent background(0.45, 0.35);

    SizeComponent offset(0.025, 0.025);

    RndrUtils.RoundedRectFilled(pos, background, ImColor(80, 80, 80));

    pos = pos + offset;

    Vec3 hsv = RGBToHSV(color.Value.x, color.Value.y, color.Value.z);

    ImColor hue = HSVToRGB(hsv.x, 1, 1);

    RndrUtils.RectMultiColor(pos, PickerRectSize, IM_COL32_WHITE, hue, IM_COL32_WHITE, hue);
    RndrUtils.RectMultiColor(pos, PickerRectSize, ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 255), ImColor(0, 0, 0, 255));

    float hueBarX = PickerRectSize.x + (hueBarSize.x/2) + pos.x;

    for (int i = 0; i < 6; ++i) {

        int j = i;
        int r = i + 1;

        if (j == 5) {
            j = 5;
            r = 0;
        }

        ImColor col0 = HSVToRGB(j * 60, 1, 1);
        ImColor col1 = HSVToRGB(r * 60, 1, 1);

        RndrUtils.RectMultiColor(Vec2(hueBarX, pos.y + (i * hueBarSize.y / 6)), Vec2(hueBarSize.x, hueBarSize.y / 6), col0, col0, col1, col1);
    }

    float curX = hsv.y;
    float curY = (1 - hsv.z);

    ImColor CursorCol = color;
    CursorCol.Value.w = 1;

    RndrUtils.CircleFilled((Vec2(curX, curY) * PickerRectSize) + pos, Client::WindowSize.y * 0.01, IM_COL32_WHITE);
    RndrUtils.CircleFilled((Vec2(curX, curY) * PickerRectSize) + pos, Client::WindowSize.y * 0.007, CursorCol);

    float HueYPos = ((hsv.x / 360) * hueBarSize.y) + pos.y;
    float HueXPos = hueBarX + hueBarSize.x;

    float thing = Client::WindowSize.y * 0.008;

    RndrUtils.TriangleFilled(Vec2(HueXPos, HueYPos), Vec2(HueXPos + (thing), HueYPos - thing), Vec2(HueXPos + (thing), HueYPos + thing), IM_COL32_BLACK);

    if (Utils::MouseInRect(pos, PickerRectSize) and Client::MouseHoldLeft) {
        hsv.y = 1 - ((PickerRectSize.x - (Client::MousePos.x - pos.x + 1)) / PickerRectSize.x);
        hsv.z = (PickerRectSize.y - (Client::MousePos.y - pos.y - 1)) / PickerRectSize.y;
    }

    if (Utils::MouseInRect(Vec2(hueBarX, pos.y), hueBarSize) and Client::MouseHoldLeft) {
        hsv.x = max(360 - (((hueBarSize.y - (Client::MousePos.y - pos.y - 1)) / hueBarSize.y) * 360), 0);
    }

    SizeComponent Alphabar(0.25, 0.03);
    SizeComponent AlphabarIMG(0.27, 0.18);

    pos.y += Client::WindowSize.x * 0.015 + hueBarSize.y;

    ImColor col = HSVToRGB(hsv.x, hsv.y, hsv.z);
    ImColor col1 = col;
    col1.Value.w = 0;

    RndrUtils.PushClipRect(pos, Alphabar);
    RndrUtils.RenderImage(pos, AlphabarIMG, "Transparent", IM_COL32_WHITE);
    RndrUtils.PopClipRect();
    RndrUtils.RectMultiColor(pos, Alphabar, col1, col, col1, col);

    float AlphaBarKnobY = pos.y + Alphabar.y;
    float AlphaBarKnobX = (color.Value.w * Alphabar.x) + pos.x;

    RndrUtils.TriangleFilled(Vec2(AlphaBarKnobX - thing, AlphaBarKnobY + thing), Vec2(AlphaBarKnobX, AlphaBarKnobY), Vec2(AlphaBarKnobX + thing, AlphaBarKnobY + thing), IM_COL32_BLACK);

    if (Utils::MouseInRect(pos, Alphabar) and Client::MouseHoldLeft) {
        color.Value.w = 1 - ((Alphabar.x - (Client::MousePos.x - pos.x)) / Alphabar.x);
    }

    col.Value.w = color.Value.w;

    color = col;
}