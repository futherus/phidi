#pragma once
#include <graphviz/gvc.h>
#include <string>

namespace gv {

using Edge = Agedge_t*;
using Node = Agnode_t*;

enum class ObjectTag
{
    Node = AGNODE,
    Edge = AGEDGE,
    Graph = AGRAPH,
};

class Graph
{
public:

    Graph( const std::string& name)
        : graph_p{ agopen( const_cast<char*>( name.c_str()), Agdirected, nullptr)}
    {}

    ~Graph()
    {
        agclose( graph_p);
    }

    struct Attribute
    {
        std::string name;
        std::string value;
    };

    using Attributes = std::initializer_list<Attribute>;

    void attrs( ObjectTag tag, Attributes attrs)
    {
        for ( const Attribute& attr : attrs )
        {
            agattr( graph_p, static_cast<int>( tag),
                    const_cast<char*>( attr.name.c_str()), attr.value.c_str());
        }
    }

    void attrs( Node node, Attributes attrs)
    {
        for ( const Attribute& attr : attrs )
        {
            agset( node, const_cast<char*>( attr.name.c_str()), attr.value.c_str());
        }
    }

    void attrs( Edge edge, Attributes attrs)
    {
        for ( const Attribute& attr : attrs )
        {
            agset( edge, const_cast<char*>( attr.name.c_str()), attr.value.c_str());
        }
    }

    Node node( const std::string& name, Attributes attributes = {})
    {
        Node node = agnode( graph_p, const_cast<char*>( name.c_str()), kCreateIfNotExist);
        attrs( node, attributes);
        return node;
    }

    Edge edge( Node from, Node to, const std::string& name = "", Attributes attributes = {})
    {
        Edge edge = agedge( graph_p, from, to, const_cast<char*>( name.c_str()), kCreateIfNotExist);
        attrs( edge, attributes);
        return edge;
    }

    void render( FILE* file, const std::string& format)
    {
        GVC_t* gvc_p = gvContext();

        gvLayout( gvc_p, graph_p, "dot");
        gvRender( gvc_p, graph_p, format.c_str(), file);

        gvFreeLayout( gvc_p, graph_p);
        gvFreeContext( gvc_p);
    }

    void dump( FILE* file)
    {
        agwrite( graph_p, file);
    }

private:
    static const int kCreateIfNotExist = 1;
    static const int kDontCreateIfNotExist = 0;

    Agraph_t* graph_p;
};

};
