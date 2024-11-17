#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>

using namespace juce;

enum LightType
{
    STATIC,
    PAULSE,
    FLASH
};

enum MidiType
{
    Note,
    CC
};

class LightInfo
{
public:
    LightInfo();
    LightInfo(const LightType lt, MidiType mt, const uint8 n, const uint8 cl);
    juce::MidiMessage getMidi() const;
    void setColor(const uint8 cl);

private:
    LightType lightType;
    const MidiType midiType;
    const uint8 note;
    uint8 color;
    const uint8 lead[3][3]{{0x90, 0xB0, 0x01}, {0x91, 0xB1, 0x02}, {0x92, 0xB2, 0x03}};
};

class LaunchpadLayout : private juce::AsyncUpdater
{

public:
    class LayoutListener
    {
    };

    LaunchpadLayout();
    void initChomatic();

    juce::MidiMessage getFunction(const int index) const;

    juce::MidiMessage getScene(const int index) const;

    juce::MidiMessage getLogo() const;

    juce::MidiMessage getGridItem(const int row, const int col) const;

    static const bool major[12];
    static const uint8 NOTE_COLOR;
    static const uint8 OFF_COLOR;
    static const uint8 ROOT_COLOR;
    static const uint8 NOTIFY_COLOR;
    static const uint8 UP_COLOR;
    static const uint8 DOWN_COLOR;
    static const uint8 LEFT_COLOR;
    static const uint8 RIGHT_COLOR;
    static const uint8 SESSION_COLOR;
    static const uint8 DRUMES_COLOR;
    static const uint8 KEYS_COLOR;
    static const uint8 USER_COLOR;
    static const uint8 SCENE_COLOR_0;
    static const uint8 SCENE_COLOR_1;
    static const uint8 SCENE_COLOR_2;
    static const uint8 SCENE_COLOR_3;
    static const uint8 SCENE_COLOR_4;
    static const uint8 SCENE_COLOR_5;
    static const uint8 SCENE_COLOR_6;
    static const uint8 SCENE_COLOR_7;
    static const uint8 LOGO_COLOR;

private:
    void handleAsyncUpdate() override {}
    LightInfo grid[8][8] = {
        {LightInfo{LightType::STATIC, MidiType::Note, 0x0B, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x0C, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x0D, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x0E, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x0F, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x10, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x11, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x12, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x15, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x16, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x17, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x18, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x19, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x1A, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x1B, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x1C, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x1F, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x20, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x21, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x22, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x23, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x24, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x25, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x26, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x29, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2A, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2B, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2C, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2D, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2E, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x2F, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x30, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x33, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x34, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x35, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x36, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x37, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x38, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x39, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x3A, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x3D, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x3E, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x3F, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x40, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x41, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x42, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x43, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x44, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x47, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x48, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x49, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x4A, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x4B, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x4C, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x4D, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x4E, NOTE_COLOR}},
        {LightInfo{LightType::STATIC, MidiType::Note, 0x51, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x52, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x53, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x54, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x55, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x56, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x57, NOTE_COLOR},
         LightInfo{LightType::STATIC, MidiType::Note, 0x58, NOTE_COLOR}},
    };

    LightInfo functions[8] =
        {
            LightInfo{LightType::STATIC, MidiType::CC, 0x5B, UP_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x5C, DOWN_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x5D, LEFT_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x5E, RIGHT_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x5F, SESSION_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x60, DRUMES_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x61, KEYS_COLOR},
            LightInfo{LightType::STATIC, MidiType::CC, 0x62, USER_COLOR}};
    LightInfo logo{LightType::STATIC, MidiType::CC, 0x63, LOGO_COLOR};
    LightInfo scenes[8] =
        {
            LightInfo{LightType::STATIC, MidiType::CC, 0x59, SCENE_COLOR_0},
            LightInfo{LightType::STATIC, MidiType::CC, 0x4F, SCENE_COLOR_1},
            LightInfo{LightType::STATIC, MidiType::CC, 0x45, SCENE_COLOR_2},
            LightInfo{LightType::STATIC, MidiType::CC, 0x3B, SCENE_COLOR_3},
            LightInfo{LightType::STATIC, MidiType::CC, 0x31, SCENE_COLOR_4},
            LightInfo{LightType::STATIC, MidiType::CC, 0x27, SCENE_COLOR_5},
            LightInfo{LightType::STATIC, MidiType::CC, 0x1D, SCENE_COLOR_6},
            LightInfo{LightType::STATIC, MidiType::CC, 0x13, SCENE_COLOR_7}};
};