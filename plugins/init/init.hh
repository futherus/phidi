#pragma once

#include "core/plugin_registry.hh"
#include "core/root.hh"
#include "widgets/column.hh"
#include "widgets/row.hh"
#include "widgets/padding.hh"
#include "widgets/placeholder.hh"

namespace xui
{

class InitPlugin final
    : public IPlugin
{
public:
    static const int ID;

    InitPlugin()
        : IPlugin{}
        , row_{ LayoutPolicy{ MainAxisAlignment::Center, CrossAxisAlignment::Center}}
        , cols_{}
    {
        cols_.push_back( std::make_unique<Column<LayoutDelegate>>( LayoutPolicy{ MainAxisAlignment::Center, CrossAxisAlignment::Center}));
        cols_.push_back( std::make_unique<Column<LayoutDelegate>>( LayoutPolicy{ MainAxisAlignment::Center, CrossAxisAlignment::Center}));
        row_.push_back( *cols_.at( 0), 1.0f);
        row_.push_back( *cols_.at( 1), 1.0f);

        gRootWidget->push_back( row_);
    }

    ~InitPlugin() = default;

    void deserialize( const json&) override {}

    void serialize( json&) override {}

    void
    add( LayoutDelegate&& widget,
         size_t index,
         float flex)
    {
        cols_.at( index)->push_back( std::move( widget), flex);
    }

private:
    Row<LayoutDelegate> row_;
    std::vector<std::unique_ptr<Column<LayoutDelegate>>> cols_;
};

}
