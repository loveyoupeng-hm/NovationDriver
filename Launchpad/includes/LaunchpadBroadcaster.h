#pragma once

#include <atomic>
#include <juce_events/juce_events.h>

using namespace juce;

class LaunchpadBroadcaster final : private AsyncUpdater
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void buttonPressed(uint8 x, uint8 y)
        {
            juce::ignoreUnused(x);
            juce::ignoreUnused(y);
        }
        virtual void upPressed() {}
        virtual void downPressed() {}
        virtual void leftPressed() {}
        virtual void rightPressed() {}
        virtual void sessionPressed() {}
        virtual void drumsPressed() {}
        virtual void keysPressed() {}
        virtual void userPressed() {}
        virtual void scenePressed(uint8 scene) { juce::ignoreUnused(scene); }
    };

    void addListener(Listener &callback);
    void buttonPressed(uint8 x, uint8 y);
    void upperPressed(uint8 index);
    void rightPressed(uint8 scene);
    void remove(Listener *callback);
    void notify();

private:
    void handleAsyncUpdate() override;
    juce::ListenerList<LaunchpadBroadcaster::Listener>
        listeners;
    std::atomic<uint16_t> button{static_cast<uint16_t>(2048)};
    std::atomic<uint32_t> flags{1 << 20};
};
