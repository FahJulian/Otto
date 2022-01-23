#pragma once

#include "otto/base.h"
#include "otto/util/dynamic_array.h"
#include "otto/event/event_listener.h"

namespace otto
{
    template<typename E>
    class EventDispatcher
    {
    public:
        void addListener(const EventListener<E>& listener)
        {
            mListeners.add(listener);
        }

        void removeListener(const EventListener<E>& listener)
        {
            mListeners.remove(listener);
        }

        void dispatch(const E& e)
        {
            for (auto& listener : mListeners)
                listener(e);
        }

    private:
        DynamicArray<EventListener<E>> mListeners;
    };

} // namespace otto
