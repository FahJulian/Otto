#pragma once

#include <iterator>

#include "otto/base.h"
#include "otto/util/pair.h"
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
            using value_type = Pair<Entity, C&>;
            using reference = Pair<Entity, C&>&;
            using pointer = Pair<Entity, C&>*;

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

            value_type operator*()
            {
                return {
                    mView->mPool->mComponents.getData()[mIndex].first,
                    mView->mPool->mComponents.getData()[mIndex].second
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

        Pair<Entity, C&> operator[](uint64 index)
        {
            return get(index);
        }

        Pair<Entity, C&> get(uint64 index) 
        {
            return {
                mPool->mComponents.getData()[index].first,
                mPool->mComponents.getData()[index].second,
            };
        }

        Iterator begin()
        {
            return Iterator(this, 0);
        }

        Iterator end()
        {
            return Iterator(this, mPool->mComponents.getSize());
        }

        uint64 getSize() const
        {
            return mPool->mComponents.getSize();
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
