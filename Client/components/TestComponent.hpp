#include <otto.h>

using namespace otto;

struct TestComponent 
{
    String tag;
};

template<>
TestComponent otto::deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
{
    TestComponent c;
    if (args.contains("tag"))
        c.tag = args.get<String>("tag");

    return c;
}
