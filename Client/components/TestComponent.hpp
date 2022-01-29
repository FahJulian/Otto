#include <otto.h>

using namespace otto;

struct TestComponent 
{
    float32 x;
    float32 y;
};

template<>
TestComponent otto::deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
{
    Log::debug("Deserializing Component TestComponent...");
    return NONE;
}
