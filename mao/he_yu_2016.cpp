#include "mao.hpp"
#include "he_yu_2016.hpp"

Z countMAOUsingHeYu2016(const GraphData& graphData) {
    Z ret;
    graphData.accessGraph([&](auto graph) {
        assert(isValidEssentialGraph(graph));
        ret = he_yu_2016::computeSize(graph);
    });
    return ret;
}
