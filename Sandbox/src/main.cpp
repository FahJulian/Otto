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
#include "otto/serialization/serializer.h"
#include "otto/scene/scene.h"
#include "otto/math/vec4.h"
#include "otto/components/transform_component.hpp"
#include "otto/core/application.h"

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

    otto::Application::init("C:/dev/Otto/Client/app_settings_concept.otto");

    auto file = otto::Serializer::deserialize("C:/dev/Otto/Client/scenes/scene_concept.otto");

    if (!file.hasError())
        otto::Log::trace(otto::String::toString(file.getResult()));

    otto::TransformComponent c = file.getResult().get("Entity1").get<otto::TransformComponent>("TransformComponent");
    otto::Log::trace(otto::serialize(c));

    otto::Vec4f32 vec = { 0, 1, 2, 3 };
    otto::Vec4f32 vec2 = { 0, 1, 2, 3 };

    otto::Log::debug(vec + vec2);
}
