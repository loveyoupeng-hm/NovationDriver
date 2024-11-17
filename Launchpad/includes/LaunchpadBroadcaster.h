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
        virtual void buttonPressed(uint8 x, uint8 y) {juce::ignoreUnused(x);juce::ignoreUnused(y);};
        virtual void upPressed() {};
        virtual void downPressed() {};
        virtual void leftPressed() {};
        virtual void rightPressed() {};
        virtual void sessionPressed() {};
        virtual void drumsPressed() {};
        virtual void keysPressed() {};
        virtual void userPressed() {};
        virtual void scenePressed(uint8 scene) {juce::ignoreUnused(scene);};
    };

    void
    addListener(Listener& callback)
    {
        JUCE_ASSERT_MESSAGE_THREAD
        listeners.add(&callback);
    }

    void buttonPressed(uint8 x, uint8 y)
    {
        const uint16_t value = x << 8 | y;
        button.store(value);
        notify();
    }

    void remove(Listener *callback)
    {
        JUCE_ASSERT_MESSAGE_THREAD
        listeners.remove(callback);
    }

    void notify()
    {
        if (MessageManager::getInstance()->isThisTheMessageThread())
        {
            const uint16_t value = button.load();
            button = static_cast<uint16_t>(666);
            if (value == 666)
                return;

            const uint8 x = value >> 8;
            const uint8 y = value & 7;

            cancelPendingUpdate();
            listeners.call([x, y](Listener& l)
                           { l.buttonPressed(x, y); });
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

private:
    juce::ListenerList<LaunchpadBroadcaster::Listener>
        listeners;

    std::atomic<uint16_t> button;

    void handleAsyncUpdate() override
    {
        notify();
    }
};
