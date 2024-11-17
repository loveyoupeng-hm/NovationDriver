#include "LaunchpadBroadcaster.h"

void LaunchpadBroadcaster::addListener(Listener &callback)
{
    JUCE_ASSERT_MESSAGE_THREAD
    listeners.add(&callback);
}

void LaunchpadBroadcaster::buttonPressed(uint8 x, uint8 y)
{
    const uint16_t value = x << 8 | y;
    button.store(value);
    notify();
}

void LaunchpadBroadcaster::upperPressed(uint8 index)
{
    flags = 1 << index;
    notify();
}

void LaunchpadBroadcaster::rightPressed(uint8 scene)
{
    flags = scene << 8;
    notify();
}

void LaunchpadBroadcaster::remove(Listener *callback)
{
    JUCE_ASSERT_MESSAGE_THREAD
    listeners.remove(callback);
}

void LaunchpadBroadcaster::notify()
{
    if (MessageManager::getInstance()->isThisTheMessageThread())
    {
        const uint16_t value = button.load();
        button = static_cast<uint16_t>(2048);
        if (value != 2048)
        {

            const uint8 x = value >> 8;
            const uint8 y = value & 7;

            cancelPendingUpdate();
            listeners.call([x, y](Listener &l)
                           { l.buttonPressed(x, y); });
        }

        const uint32_t flag = flags.load();
        flags = 1 << 20;
        if (flag != 1 << 20)
        {
            cancelPendingUpdate();
            switch (flag)
            {
            case 1:
                listeners.call([](Listener &l)
                               { l.upPressed(); });
                break;
            case 1 << 1:
                listeners.call([](Listener &l)
                               { l.downPressed(); });
                break;
            case 1 << 2:
                listeners.call([](Listener &l)
                               { l.leftPressed(); });
                break;
            case 1 << 3:
                listeners.call([](Listener &l)
                               { l.rightPressed(); });
                break;
            case 1 << 4:
                listeners.call([](Listener &l)
                               { l.sessionPressed(); });
                break;
            case 1 << 5:
                listeners.call([](Listener &l)
                               { l.drumsPressed(); });
                break;
            case 1 << 6:
                listeners.call([](Listener &l)
                               { l.keysPressed(); });
                break;
            case 1 << 7:
                listeners.call([](Listener &l)
                               { l.userPressed(); });
                break;
            default:
                uint8 scene = flag >> 8;
                listeners.call([scene](Listener &l)
                               { l.scenePressed(scene); });
                break;
            }
        }
    }
    else
    {
        triggerAsyncUpdate();
    }
}

void LaunchpadBroadcaster::handleAsyncUpdate()
{
    notify();
}
