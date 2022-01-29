#pragma once

#include <iterator>

#include "otto/base.h"
#include "otto/scene/component_pool.h"

namespace otto
{
    template<typename C>
    class View
    {
    public:
        struct Iterator
        {
        private:
            using iterator_category = std::forward_iterator_tag;
            using value_type = C;
            using reference = C&;
            using pointer = C*;

            uint64 mIndex;
            View<C>* mView;

            Iterator(View<C>* view, uint64 index)
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

            pointer operator*() { return &mView->mPool->mComponents[mIndex]; }

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

            template<typename C>
            friend class View;
        };
            
    private:
        View(ComponentPool<C>* pool)
            : mPool(pool)
        {
        }

    public:
        View(const View& other) = delete;
        View& operator=(const View& other) = delete;

        Iterator begin()
        {
            return Iterator(this, 0);
        }

        Iterator end()
        {
            return Iterator(this, mPool->mComponents.getSize());
        }

    private:
        void onComponentRemoved(uint64 index)
        {
            for (uint64* iteratorIndex : mActiveIterators)
            {
                if (*iteratorIndex >= index)
                    (*iteratorIndex)--;
            }
        }

        DynamicArray<uint64*> mActiveIterators;
        ComponentPool<C>* mPool;

        friend struct SceneData;
    };

} // namespace otto
