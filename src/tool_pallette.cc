#include "tools.hh"

namespace xui
{

void ToolPallette::update()
{
    std::string active = tool_manager_->getActive();

    for (auto& p : tool_ids_)
    {
        if (p.second == active)
        {
            pallette_.update(p.first);
            return;
        }
    }
}

void ToolPallette::add( std::string tool_id, PushButton* button)
{
    // FIXME: button ordering is fixed
    std::fprintf( stderr, "before inserting index pair\n");
    tool_ids_.insert( std::pair<int, std::string>{ pallette_.size(), std::move( tool_id)});

    pallette_.add( button);
}

} // namespace xui
