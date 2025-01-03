#include "LaunchpadComponent.h"

LaunchpadComponent::LaunchpadComponent()
{
    broadcaster = new LaunchpadBroadcaster();
    auto area = getLocalBounds();
    setSize(area.getWidth(), area.getWidth());
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            clicked[row][col] = false;
        }
        upclicked[row] = false;
        rightclicked[row] = false;
    }
    this->setRepaintsOnMouseActivity(true);
}

LaunchpadComponent::~LaunchpadComponent()
{
    if (broadcaster != nullptr)
        delete broadcaster;
    broadcaster = nullptr;
}

void LaunchpadComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::black);

    for (int i = 0; i < 8; i++)
    {
        if (upclicked[i])
            g.setColour(juce::Colours::goldenrod);
        else
            g.setColour(juce::Colours::grey);

        int start_i = 10 + i * 60 + 5;
        int end_i = 10 + 5;
        float start = static_cast<float>(start_i);
        float end = static_cast<float>(end_i);
        g.drawRect(start_i, end_i, 60, 60, 2);
        g.setColour(juce::Colours::black);
        Path path;
        if (upclicked[i])
            g.setColour(juce::Colours::goldenrod);
        else
            g.setColour(juce::Colours::grey);
        switch (i)
        {
        case 0:
            path.addTriangle(start + 25, end + 35, start + 35, end + 35, start + 30, end + 25);
            g.fillPath(path);
            break;
        case 1:
            path.addTriangle(start + 25, end + 25, start + 30, end + 35, start + 35, end + 25);
            g.fillPath(path);
            break;
        case 2:
            path.addTriangle(start + 25, end + 30, start + 35, end + 35, start + 35, end + 25);
            g.fillPath(path);
            break;
        case 3:
            path.addTriangle(start + 25, end + 35, start + 35, end + 30, start + 25, end + 25);
            g.fillPath(path);
            break;
        default:
            break;
        }

        switch (i)
        {
        case 4:
            g.drawText("Session", start_i + 2, end_i + 2, 60 - 4, 60 - 4, juce::Justification::centred, true);
            break;
        case 5:
            g.drawText("Drums", start_i + 2, end_i + 2, 60 - 4, 60 - 4, juce::Justification::centred, true);
            break;
        case 6:
            g.drawText("Keys", start_i + 2, end_i + 2, 60 - 4, 60 - 4, juce::Justification::centred, true);
            break;
        case 7:
            g.drawText("User", start_i + 2, end_i + 2, 60 - 4, 60 - 4, juce::Justification::centred, true);
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        if (rightclicked[i])
            g.setColour(juce::Colours::lightcoral);
        else
            g.setColour(juce::Colours::grey);

        int start_i = 10 + 7 * 60 + 5 + 60 + 10;
        int end_i = 80 + (7 - i) * 60 + 5;
        float start = static_cast<float>(start_i);
        float end = static_cast<float>(end_i);
        g.drawRect(start_i, end_i, 60, 60, 2);
        g.setColour(juce::Colours::black);
        if (rightclicked[i])
            g.setColour(juce::Colours::lightcoral);
        else
            g.setColour(juce::Colours::grey);
        if ((7 - i) < 7)
        {
            g.drawLine(start + 25, end + 25, start + 35, end + 30, 2);
            g.drawLine(start + 25, end + 35, start + 35, end + 30, 2);
        }
        else
        {
            g.drawText("Stop", start_i + 2, end_i + 12, 60 - 4, 60 - 4, juce::Justification::centredTop, true);
            g.drawText("Solo", start_i + 2, end_i + 2, 60 - 4, 60 - 4, juce::Justification::centred, true);
            g.drawText("Mute", start_i + 2, end_i + 2, 60 - 4, 50 - 4, juce::Justification::centredBottom, true);
        }
    }

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            g.setColour(juce::Colours::grey);
            int start = 10 + col * 60 + 5;
            int end = 80 + (7 - row) * 60 + 5;
            g.drawRect(start, end, 60, 60, 2);
            if (clicked[row][col])
                g.setColour(juce::Colours::darkcyan);
            else
                g.setColour(juce::Colours::lightsteelblue);

            g.fillRect(start + 2, end + 2, 60 - 4, 60 - 4);
        }
    }
}

void LaunchpadComponent::resized() {}

void LaunchpadComponent::mouseDown(const MouseEvent &event)
{
    int x = event.getPosition().getX();
    int y = event.getPosition().getY();
    if (x > 10 + 7 * 60 + 5 + 60 + 65)
        return;
    if (y > 80 + 7 * 60 + 5 + 60)
        return;

    if (y <= 15 + 60)
    {
        uint8 col = static_cast<uint8>((x - 15) / 60);
        upclicked[col] = !upclicked[col];
        broadcaster->upperPressed(col);
    }
    else if (x >= 10 + 7 * 60 + 5 + 60 + 10)
    {
        uint8 row = static_cast<uint8>(7 - (y - 85) / 60);
        rightclicked[row] = !rightclicked[row];
        broadcaster->rightPressed(row);
    }
    else
    {
        uint8 col = static_cast<uint8>((x - 15) / 60);
        uint8 row = static_cast<uint8>(7 - (y - 85) / 60);
        clicked[row][col] = !clicked[row][col];
        broadcaster->buttonPressed(row, col);
    }
}