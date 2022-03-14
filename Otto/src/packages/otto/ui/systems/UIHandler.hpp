#pragma once

#include "otto.h"

#include "packages/otto/core.h"
#include "packages/otto/window.h"

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

        void onEvent(const InitEvent& e)
        {
            for (auto [entity, component] : *mUIComponentView)
            {
                component.hovered = component.x.absoluteValue <= Mouse::getX() && Mouse::getX() < component.x.absoluteValue + component.width.absoluteValue &&
                    component.y.absoluteValue <= Mouse::getY() && Mouse::getY() < component.y.absoluteValue + component.height.absoluteValue;
            }
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

        void onEvent(const MouseMovedEvent& e)
        {
            bool8 dirty = false;

            for (auto [entity, component] : *mUIComponentView)
            {
                bool8 hovered = component.x.absoluteValue <= e.x && e.x < component.x.absoluteValue + component.width.absoluteValue &&
                    component.y.absoluteValue <= e.y && e.y < component.y.absoluteValue + component.height.absoluteValue;

                if (component.hovered != hovered)
                {
                    component.hovered = hovered;
                    dirty = true;
                }
            }

            if (dirty)
                mScene->dispatchEvent(UIRendererRebufferEvent());
        }

        void onEvent(const ComponentAddedEvent<UIComponent>& e)
        {
            Log::debug(mScene);
            auto& component = mScene->getComponent<UIComponent>(e.entity);

            component.hovered = component.x.absoluteValue <= Mouse::getX() && Mouse::getX() < component.x.absoluteValue + component.width.absoluteValue &&
                component.y.absoluteValue <= Mouse::getY() && Mouse::getY() < component.y.absoluteValue + component.height.absoluteValue;
        }

    private:
        View<UIComponent>* mUIComponentView = nullptr;
    };

} // namespace otto
