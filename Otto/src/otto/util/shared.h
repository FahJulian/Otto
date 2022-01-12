#pragma once

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
            : mPointer(pointer), mNCopies(pointer != nullptr ? new size_t(0) : nullptr)
        {
        }

        Shared(const Shared& other)
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
                return;

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

        operator bool() const
        {
            return isValid();
        }

        bool isValid() const
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

        size_t getCopyCount() const
        {
            return *mNCopies;
        }

    private:
        T* mPointer;
        size_t* mNCopies;
    };

} // namespace otto