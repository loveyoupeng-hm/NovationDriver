#include "LaunchpadComponent.h"

LaunchpadComponent::LaunchpadComponent()
{
}
void LaunchpadComponent::paint(juce::Graphics &g) {
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::white);

    g.drawLine(0,0, 100,100);
}
void LaunchpadComponent::resized() {}