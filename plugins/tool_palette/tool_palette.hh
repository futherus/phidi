#pragma once

#include "plugins/tools/tools.hh"
#include "plugins/init/init.hh"
#include "widgets/sized_box.hh"

namespace xui
{

class ToolPalette final
{
public:
    ToolPalette( ToolManager* tool_manager,
                 float padding)
        : tool_manager_{ tool_manager}
        , palette_{ padding}
    {
        palette_.bind(
            [&]( int indx){ tool_manager_->setActive( tool_ids_.find( indx)->second); }
        );

        tool_manager->addView( *this);
    }

    ToolPalette( const ToolPalette&) = delete;
    ToolPalette& operator=( const ToolPalette&) = delete;
    ToolPalette( ToolPalette&&) = delete;
    ToolPalette& operator=( ToolPalette&&) = delete;
    ~ToolPalette() = default;

public:
    void update();

    void add( std::string tool_id, PushButton& button);

public:
    const PushPalette& getPalette() const { return palette_; }
          PushPalette& getPalette()       { return palette_; }

private:
    ToolManager* tool_manager_;
    PushPalette palette_;

    std::vector<std::unique_ptr<Padding<PushButton&>>> sized_boxes_;

    // FIXME: BiMap.
    std::map<int, std::string> tool_ids_;
};

class ToolPalettePlugin final
    : public IPlugin
{
public:
    static const int ID;

    ToolPalettePlugin()
        : IPlugin{}
        , tool_palette_{ std::make_unique<ToolPalette>(
              PluginRegistry::instance()->getPlugin<ToolsPlugin>()->getToolManager(),
              10
          )}
    {$FUNC
        // PluginRegistry::instance()->getPlugin<InitPlugin>()->add( tool_palette_->getPalette(), 0, 1.0);
    }

    ToolPalettePlugin( const ToolPalettePlugin&) = delete;
    ToolPalettePlugin& operator=( const ToolPalettePlugin&) = delete;
    ToolPalettePlugin( ToolPalettePlugin&&) = delete;
    ToolPalettePlugin& operator=( ToolPalettePlugin&&) = delete;
    ~ToolPalettePlugin() override = default;

public:
    void
    deserialize( const json&) override
    {
        PluginRegistry::instance()->getPlugin<InitPlugin>()->add( tool_palette_->getPalette(), 0, 1.0);
    }

    void serialize( json&) override {}

public:
    ToolPalette* getToolPalette() { return tool_palette_.get(); }

private:
    std::unique_ptr<ToolPalette> tool_palette_;
};

}
