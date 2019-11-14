#include "mao.hpp"

namespace {

template <int N>
using BP = BitSet<bitSetParam(N)>;

template <int N>
using Mem = unordered_map<pair<int, BP<N>>, Z, hash<pair<int, BP<N>>>, equal_to<pair<int, BP<N>>>, PoolAllocator<pair<const pair<int, BP<N>>, Z>>>;

template <int N>
Z countInChordal(
    Mem<N>& mem,
    const Graph<N>& graph,
    BP<N> mask,
    BP<N> dom
) {
    typedef Graph<N> G;
    typedef typename G::B B;
    
    assert(graph.isUndirected());
    assert(graph.isChordal());
    
    int n = graph.size();
    if(n <= 1) {
        return 1;
    }
    
    B all = B::range(n);
    int e = dom.count() * (n - 1);
    setDifference(all, dom).iterate([&](int v) {
        int neighCount = graph.edgesOut(v).count();
        e += neighCount;
        if(neighCount == n - 1) {
            dom.add(v);
        }
    });
    e /= 2;
    
    if(e == n - 1) {
        return n;
    }
    if(e == n) {
        return 2 * n;
    }
    int a = n * (n - 1) / 2;
    if(e == a - 2) {
        return (n * (n - 1) - 4) * FactorialTable<N>::get(n - 3);
    }
    if(e == a - 1) {
        return FactorialTable<N>::get(n - 2) * (2 * n - 3);
    }
    if(e == a) {
        return FactorialTable<N>::get(n);
    }
    
    Z& ret = mem[make_pair(dom.count(), setDifference(mask, B::unpack(dom, mask)))];
    if(ret != 0) {
        return ret;
    }
    
    auto considerRoot = [&](int v) {
        G newGraph = graph;
        
        B prev(v);
        B cur = graph.neighbors(v);
        
        while(cur.isNonEmpty()) {
            cur.iterate([&](int y) {
                setIntersection(newGraph.bidirectionalNeighbors(y), prev)
                    .iterate([&](int x) {
                        newGraph.delD(y, x);
                    });
            });
            
            while(true) {
                bool changed = false;
                
                cur.iterate([&](int y) {
                    setIntersection(newGraph.bidirectionalNeighbors(y), cur)
                        .iterate([&](int z) {
                            if(setDifference(newGraph.directedEdgesIn(y), newGraph.neighbors(z)).isNonEmpty()) {
                                newGraph.delD(z, y);
                                changed = true;
                            }
                        });
                });
                
                if(!changed) {
                    break;
                }
            }
            
            B next;
            cur.iterate([&](int x) {
                next = setUnion(next, graph.neighbors(x));
            });
            next = setDifference(next, setUnion(prev, cur));
            
            prev = cur;
            cur = next;
        }
        
        Z val = 1;
        newGraph.iterateBidirectionalComponents([&](const auto& comp) {
            val *= countInChordal(mem, newGraph.inducedSubgraph(comp), B::unpack(comp, mask), B::pack(dom, comp));
        });
        return val;
    };
    
    setDifference(all, dom).iterate([&](int v) {
        ret += considerRoot(v);
    });
    if(dom.isNonEmpty()) {
        ret += dom.count() * considerRoot(dom.min());
    }
    
    return ret;
}

template <int N>
Z count(const Graph<N>& graph) {
    assert(isValidEssentialGraph(graph));
    Z ret = 1;
    graph.iterateBidirectionalComponents([&](const auto& comp) {
        Mem<N> mem;
        ret *= countInChordal(mem, graph.inducedSubgraph(comp), comp, BP<N>());
    });
    return ret;
}

}

Z countMAOUsingDynamicProgrammingWithDominatingVertexElimination(const GraphData& graphData) {
    Z ret;
    graphData.accessGraph([&](auto graph) {
        ret = count(graph);
    });
    return ret;
}
