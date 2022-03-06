#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename T>
    class Shared
    {
    public:
        Shared()
            : mPointer(nullptr), mNCopies(nullptr)
        {
        }

        Shared(T* pointer)
            : mPointer(pointer), mNCopies(pointer != nullptr ? new uint64(1) : nullptr)
        {
        }

        Shared(const Shared& other)
            : mPointer(nullptr), mNCopies(nullptr)
        {
            *this = other;
        }

        ~Shared()
        {
            if (!isValid())
                return;

            (*mNCopies)--;

            if (*mNCopies == 0)
            {
                delete mNCopies;
                delete mPointer;
            }
        }

        Shared& operator=(const Shared& other)
        {
            if (this == &other)
                return *this;

            this->~Shared();

            mPointer = other.mPointer;
            mNCopies = other.mNCopies;

            if (isValid())
                (*mNCopies)++;

            return *this;
        }

        T& operator*()
        {
            return *mPointer;
        }

        T* operator->()
        {
            return mPointer;
        }

        const T& operator*() const
        {
            return *mPointer;
        }

        const T* operator->() const
        {
            return mPointer;
        }

        operator bool8() const
        {
            return isValid();
        }

        bool8 isValid() const
        {
            return mPointer != nullptr;
        }

        T* get()
        {
            return mPointer;
        }

        const T* get() const
        {
            return mPointer;
        }

        uint64 getCopyCount() const
        {
            return *mNCopies;
        }

    private:
        T* mPointer;
        uint64* mNCopies;
    };

} // namespace otto
