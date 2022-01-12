#include <iostream>
#include <ctime>
#include <chrono>

#include "otto/core/platform/time.h"
#include "otto/debug/log/log.h"
#include "otto/base.h"
#include "otto/math/vec2.h"
#include "otto/math/mat2x2.h"
#include "otto/util/string.h"
#include "otto/math/math.h"
#include "otto/debug/profile/profiler.h"

template<int i>
otto::String s = "";

template<>
otto::String s<1> = "1";

template<int i>
otto::String getS()
{

}

int main()
{
    otto::Time::init();
    otto::Log::init("C:/dev/Otto/Sandbox/.log/main.log", std::cout, otto::Log::ALL, otto::Log::ALL);

    otto::Log::debug("Testing logger...");

    //otto::uint32 i = 3;
    //std::cout << (i += 3) << std::endl;

    //otto::Vec2f32 pos = { 1.f, .2f };
    //std::cout << pos.x << pos.y << std::endl;

    //otto::Mat2x2f32 mat = {
    //    1, 1,
    //    1, 1
    //};

    //otto::Vec2f32 result = mat * pos;
    //std::cout << result.x << ", " << result.y << std::endl;

    //std::cout << otto::Mat2x2f32(1, 2, 3, 4) << std::endl;

    //std::cout << s<1> << std::endl;

    //std::cout << sizeof(double) << std::endl;

    //std::cout << otto::sqrt(static_cast<otto::float32>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::float64>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::int8>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::int16>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::int32>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::int64>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::uint8>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::uint16>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::uint32>(2)) << std::endl;
    //std::cout << otto::sqrt(static_cast<otto::uint64>(2)) << std::endl;

    struct T
    {
        otto::float32 sqrt()
        {
            return 2.f;
        }
    } a;

    int i = 3;
    const int& j = i;
    
    otto::String testString = "123456789";
    std::cout << otto::String::replace(testString, 2, 3, 'a') << std::endl;
    std::cout << otto::String::replace(testString, 2, 5, 'a') << std::endl;
    std::cout << otto::String::replace(testString, 2, 2, "Replacement") << std::endl;
    std::cout << otto::String::replace(testString, 1, 3, "Replacement") << std::endl;
    std::cout << otto::String::replace(testString, 2, 3, otto::String("Replacement")) << std::endl;
    std::cout << otto::String::replace(testString, 1, 3, otto::String("Replacement")) << std::endl;

    otto::float32 f = otto::PI_32;
    otto::float64 d = otto::PI_64;

    for (int i = 0; i < 100; i++)
        otto::sqrt(3.0f);

    std::cout << otto::Time::getTime32() << std::endl;

    otto::Profiler::logResults();
}
