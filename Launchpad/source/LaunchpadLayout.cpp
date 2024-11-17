#include "LaunchpadLayout.h"

LightInfo::LightInfo() : lightType{LightType::STATIC}, midiType{MidiType::Note}, note{0x0B}, color{0x00}
{
}

LightInfo::LightInfo(const LightType lt, MidiType mt, const uint8 n, const uint8 cl)
    : lightType{lt}, midiType{mt}, note{n}, color{cl}
{
}

juce::MidiMessage LightInfo::getMidi() const
{
    auto message = juce::MidiMessage{new uint8[9]{lead[lightType][midiType], note, color}, 3};
    message.setChannel(lead[lightType][2]);
    return message;
}

void LightInfo::setColor(const uint8 cl)
{
    color = cl;
}

LaunchpadLayout::LaunchpadLayout()
{
    initChomatic();
}

void LaunchpadLayout::initChomatic()
{
    int start = 0;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            grid[row][col].setColor(NOTE_COLOR);
            if ((start + col) % 12 == 0)
                grid[row][col].setColor(ROOT_COLOR);
            else if (!major[(start + col) % 12])
                grid[row][col].setColor(OFF_COLOR);
        }
        start += 5;
    }
}

juce::MidiMessage LaunchpadLayout::getFunction(const int index) const
{
    return functions[index].getMidi();
}

juce::MidiMessage LaunchpadLayout::getScene(const int index) const
{
    return scenes[index].getMidi();
}

juce::MidiMessage LaunchpadLayout::getLogo() const
{
    return logo.getMidi();
}

juce::MidiMessage LaunchpadLayout::getGridItem(const int row, const int col) const
{
    return grid[row][col].getMidi();
}

const bool LaunchpadLayout::major[12]{true, false, true, false, true, true, false, true, false, true, false, true};
const uint8 LaunchpadLayout::NOTE_COLOR = 0x02;
const uint8 LaunchpadLayout::OFF_COLOR = 0x00;
const uint8 LaunchpadLayout::ROOT_COLOR = 0x36;
const uint8 LaunchpadLayout::NOTIFY_COLOR = 0x4C;
const uint8 LaunchpadLayout::UP_COLOR = 0x5A;
const uint8 LaunchpadLayout::DOWN_COLOR = 0x5A;
const uint8 LaunchpadLayout::LEFT_COLOR = 0x35;
const uint8 LaunchpadLayout::RIGHT_COLOR = 0x35;
const uint8 LaunchpadLayout::SESSION_COLOR = 0x21;
const uint8 LaunchpadLayout::DRUMES_COLOR = 0x26;
const uint8 LaunchpadLayout::KEYS_COLOR = 0x4E;
const uint8 LaunchpadLayout::USER_COLOR = 0x6B;
const uint8 LaunchpadLayout::LOGO_COLOR = 0x6F;
const uint8 LaunchpadLayout::SCENE_COLOR_0 = 0x50;
const uint8 LaunchpadLayout::SCENE_COLOR_1 = 0x58;
const uint8 LaunchpadLayout::SCENE_COLOR_2 = 0x60;
const uint8 LaunchpadLayout::SCENE_COLOR_3 = 0x68;
const uint8 LaunchpadLayout::SCENE_COLOR_4 = 0x43;
const uint8 LaunchpadLayout::SCENE_COLOR_5 = 0x30;
const uint8 LaunchpadLayout::SCENE_COLOR_6 = 0x5F;
const uint8 LaunchpadLayout::SCENE_COLOR_7 = 0x27;