#include <otto.h>

using namespace otto;

struct TestComponent2
{
    float32 x;
    float32 y;
};

template<>
TestComponent2 otto::deserializeComponent(const Serialized& serialized, const Map<String, Entity>& entities)
{
    Log::debug("Deserializing Component TestComponent2...");
    return NONE;
}
