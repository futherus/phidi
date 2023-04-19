#pragma once

#include "core/plugin_registry.hh"
#include "core/root.hh"
#include "widgets/column.hh"
#include "widgets/padding.hh"

namespace xui
{

class InitPlugin final
    : public IPlugin
{
public:
    static const int ID;

    InitPlugin()
        : IPlugin{}
        , manager_{ 10, LayoutPolicy{ MainAxisAlignment::Center, CrossAxisAlignment::Center}}
    {
        manager_.setPadding( 20);
        gRootWidget->push_back( manager_);
    }

    ~InitPlugin() = default;

    void deserialize( const json&) override {}

    void serialize( json&) override {}

    void add( LayoutDelegate&& widget) { manager_.getChild().push_back( std::move( widget)); }

private:
    Padding<Column<LayoutDelegate>> manager_;
};

}
