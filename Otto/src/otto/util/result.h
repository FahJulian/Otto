#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename R, typename E>
    struct Result
    {
        Result(R result)
            : _hasError(false), result(result)
        {
        }

        Result(E error)
            : _hasError(true), error(error)
        {
        }

        ~Result()
        {
            if (_hasError)
                error.~E();
            else
                result.~R();
        }

        bool8 hasError() const { return _hasError; }
        const E& getError() const { return error; }
        R& getResult() { return result; }
        const R& getResult() const { return result; }

    private:
        bool8 _hasError;

        union
        {
            R result;
            E error;
        };
    };

} // namespace otto
