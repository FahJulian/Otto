#pragma once

#include "otto.h"

#include "packages/otto/core.h"

#include "packages/otto/ui/components/UIComponent.hpp"
#include "packages/otto/ui/events/UIRendererRebufferEvent.hpp"

namespace otto
{
    class UIHandler : public System
    {
    public:
        UIHandler(View<UIComponent>* uiComponentView)
        {
            mUIComponentView = uiComponentView;
        }

        void onEvent(const UpdateEvent& updateEvent)
        {
            bool8 dirty = false;

            for (auto [entity, component] : *mUIComponentView)
            {
                if (component.dirty)
                {
                    dirty = true;
                    break;
                }
            }

            if (dirty)
                mScene->dispatchEvent(UIRendererRebufferEvent());
        }

    private:
        View<UIComponent>* mUIComponentView = nullptr;
    };

} // namespace otto
