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
#include "otto/serialization/otto_file_loader.h"
#include "otto/scene/scene.h"
#include "otto/math/vec4.h"

struct Test
{
    otto::int32 test;

    friend otto::float32 operator+(const Test& test, otto::float32 f)
    {
        return f;
    }
};

int main()
{
    otto::Time::init();
    otto::Log::init("C:/dev/Otto/Sandbox/.log/main.log", std::cout, otto::Log::ALL, otto::Log::ALL);

    auto file = otto::OttoFileLoader::load("C:/dev/Otto/Client/scenes/scene_concept.otto");

    if (!file.hasError())
        otto::Log::trace(otto::String::toString(file.getResult().serialized));

    otto::Vec4f32 vec = { 0, 1, 2, 3 };
    otto::Vec4f32 vec2 = { 0, 1, 2, 3 };

    otto::Log::debug(vec + vec2);
}
