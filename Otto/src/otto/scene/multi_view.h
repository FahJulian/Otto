#pragma once

#include <iterator>

#include "otto/base.h"
#include "otto/util/triple.h"
#include "otto/scene/component_pool.h"

namespace otto
{
    template<typename C1, typename C2>
    class MultiView
    {
    public:
        struct Iterator
        {
        private:
            using iterator_category = std::forward_iterator_tag;
            using value_type = Triple<Entity, C1&, C2&>;
            using reference = Triple<Entity, C1&, C2&>&;
            using pointer = Triple<Entity, C1&, C2&>*;

            uint64 mIndex;
            MultiView<C1, C2>* mView;

            Iterator(MultiView<C1, C2>* view, uint64 index)
                : mView(view), mIndex(index)
            {
                mView->mActiveIterators.add(&mIndex);
            }

        public:
            Iterator(const Iterator& other)
                : mView(other.mView), mIndex(other.mIndex)
            {
                mView->mActiveIterators.add(&mIndex);
            }

            value_type operator*() 
            { 
                return {
                    mView->mEntities[mIndex],
                    mView->mPool1->mComponents[mView->mEntities[mIndex]],
                    mView->mPool2->mComponents[mView->mEntities[mIndex]]
                };
            }

            Iterator& operator++() { mIndex++; return *this; }
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

            friend bool operator==(const Iterator& a, const Iterator& b) { return a.mIndex == b.mIndex; }
            friend bool operator!=(const Iterator& a, const Iterator& b) { return a.mIndex != b.mIndex; }
            friend bool operator <(const Iterator& a, const Iterator& b) { return a.mIndex < b.mIndex; }
            friend bool operator >(const Iterator& a, const Iterator& b) { return a.mIndex > b.mIndex; }
            friend bool operator <=(const Iterator& a, const Iterator& b) { return a.mIndex <= b.mIndex; }
            friend bool operator >=(const Iterator& a, const Iterator& b) { return a.mIndex >= b.mIndex; }

            ~Iterator()
            {
                mView->mActiveIterators.remove(mView->mActiveIterators.indexOf(&mIndex));
            }

            template<typename C1, typename C2>
            friend class MultiView;
        };

    private:
        MultiView(ComponentPool<C1>* pool1, ComponentPool<C2>* pool2)
            : mPool1(pool1), mPool2(pool2)
        {
            for (uint64 i = 0; i < mPool1->mComponents.getSize(); i++)
            {
                Entity entity = mPool1->mComponents.getData()[i].first;

                if (mPool2->mComponents.containsKey(entity))
                    mEntities.add(entity);
            }
        }

    public:
        MultiView(const MultiView& other) = delete;
        MultiView& operator=(const MultiView& other) = delete;

        Iterator begin()
        {
            return Iterator(this, 0);
        }

        Iterator end()
        {
            return Iterator(this, getSize());
        }

    private:
        void onComponent1Added(Entity entity)
        {
            if (mPool2->mComponents.containsKey(entity))
                mEntities.add(entity);
        }

        void onComponent2Added(Entity entity)
        {
            if (mPool1->mComponents.containsKey(entity))
                mEntities.add(entity);
        }

        void onComponent1Removed(Entity entity)
        {
            if (mEntities.contains(entity))
                mEntities.remove(mEntities.indexOf(entity));
        }

        void onComponent2Removed(Entity entity)
        {
            if (mEntities.contains(entity))
                mEntities.remove(mEntities.indexOf(entity));
        }

        void onComponentRemoved(Entity entity)
        {
            uint64 index = mEntities.indexOf(entity);

            if (index == mEntities.getSize())
                return;

            for (uint64* iteratorIndex : mActiveIterators)
            {
                if (*iteratorIndex >= index)
                    (*iteratorIndex)--;
            }
        }   

        uint64 getSize() const
        {
            return mEntities.getSize();
        }

        DynamicArray<uint64*> mActiveIterators;
        DynamicArray<Entity> mEntities;
        ComponentPool<C1>* mPool1;
        ComponentPool<C2>* mPool2;

        friend class Scene;
        friend struct SceneData;
    };

} // namespace otto
