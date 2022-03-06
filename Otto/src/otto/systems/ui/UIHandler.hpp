#pragma once

#include "otto/base.h"
#include "otto/scene/view.h"
#include "otto/scene/system.h"

#include "otto/events/UpdateEvent.hpp"
#include "otto/components/ui/UIComponent.hpp"
#include "otto/events/ui/UIRendererRebufferEvent.hpp"

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
