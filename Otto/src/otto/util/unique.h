#pragma once

namespace otto
{
	template<typename T>
	class Unique
	{
	public:
		Unique()
			: mPointer(nullptr), mIsOwner(true)
		{
		}

		Unique(T* pointer)
			: mPointer(pointer), mIsOwner(true)
		{
		}

		Unique(const Unique& other)
		{
			*this = other;
		}

		~Unique()
		{
			if (!isValid())
				return;

			if (mIsOwner)
				delete mPointer;
		}

		Unique& operator=(const Unique& other)
		{
			if (this == &other)
				return;

			this->~Unique();

			mPointer = other.mPointer;
			mIsOwner = false;

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

		bool isOwner() const
		{
			return mIsOwner;
		}

	private:
		T* mPointer;
		bool mIsOwner;
	};

} // namespace otto
