#include <iostream>

#include "tools.hh"

namespace xui
{

const int ToolsPlugin::ID = 0;
static RegisterPlugin<xui::ToolsPlugin> X{ "ToolsPlugin"};

void
ToolManager::updateViews()
{
    for ( auto& v : views_ )
        v.update();
}

void
ToolManager::setActive( const std::string& id)
{
    // FIXME: is available?
    active_tool_ = id;

    $M( "Active tool: %s\n", id.data());
    updateViews();
}

const std::string&
ToolManager::getActive() const
{
    return active_tool_;
}

void
ToolManager::addView( ToolManagerViewDelegate&& view)
{
    views_.push_back( std::move( view));
    updateViews();
}

void
ToolManager::addTool( ITool* tool)
{
    // FIXME: update views or leave it on them?
    tools_.push_back( tool);
}

ITool*
ToolManager::getActiveTool() const
{
    // FIXME: Store pointer to active tool instead string.
    // FIXME: Stub tool to handle case if we have no tools.
    $D( "active_tool_ = %s\n", active_tool_.data());

    for ( auto t : tools_ )
        if ( t->getId() == active_tool_ )
            return t;

    assert( 0 && "Tool was not found");
}

// void Canvas::drawCircle( sf::Vector2f pos, float radius, sf::Color color);
// void Canvas::drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color);

// void Canvas::draw( sf::RenderTarget& target) const override;

void
Canvas::clear()
{
    is_pressed_ = false;

    sf::RectangleShape rect{ size_};
    rect.setFillColor( base_color_);

    pixels_.draw( rect);

    pixels_.display();
}

void
Canvas::drawCircle( sf::Vector2f pos, float radius, sf::Color color)
{
    sf::CircleShape circ{ radius};
    circ.setPosition( pos);
    circ.setOrigin( {radius, radius});
    circ.setFillColor( color);
    pixels_.draw( circ);

    pixels_.display();
}

void
Canvas::drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color)
{
    sf::Vertex line[] = { sf::Vertex(pos1), sf::Vertex(pos2) };
    line[0].color = color;
    line[1].color = color;
    pixels_.draw( line, 2, sf::Lines);

    pixels_.display();
}

// void Canvas::draw( sf::RenderTarget& target) const
// {
//     this->IWidget::draw( target);

//     // printf("drawing canvas %lgx%lg at (%lg, %lg)\n", bounds().width(), bounds().height(), bounds().left(), bounds().top());
//     // printf("texture: %ux%u\n", texture_.getSize().x, texture_.getSize().y);
//     // printf("sprite: (%lg, %lg)\n", sprite_.getPosition().x, sprite_.getPosition().y);
//     // std::cout << std::hex << pixels_.at(11) << std::endl;
//     target.draw( sprite_);
// }

void
Render( const Canvas& canvas,
        const Geometry& geometry, 
        sf::RenderTarget& target)
{
    sf::Sprite sprite;
    sprite.setTexture( canvas.getRenderTexture()->getTexture(), true);
    sprite.setPosition( geometry.tl());

    target.draw( sprite);
}

LayoutObject
Layout( const Canvas& canvas,
        const Constraints& cons)
{$FUNC
    assert( cons >= canvas.getSize());
    $D( "Canvas size in layout: (%f, %f)\n", canvas.getSize().x, canvas.getSize().y);

    LayoutObject object = LayoutObject{ canvas, {{}, canvas.getSize()}};
    $M( "returning Canvas (%f, %f) (%f, %f)\n", object.getPosition().x, object.getPosition().y, object.getSize().x, object.getSize().y);
    return object;
}

void
Canvas::onMousePressed( sf::Vector2f rel_pos)
{
    // printf("mouse pressed\n");
    //impl(). is_pressed_ = true;
    tool_manager_->getActiveTool()->onMousePressed( this, rel_pos);
}

void
Canvas::onMouseReleased( sf::Vector2f rel_pos)
{
    // printf("mouse released\n");
    // is_pressed_ = false;
    tool_manager_->getActiveTool()->onMouseReleased( this, rel_pos);
}

void
Canvas::onMouseMoved( sf::Vector2f rel_pos)
{
    if (rel_pos <= Constraints{ size_})
        tool_manager_->getActiveTool()->onMouseMoved( this, rel_pos);
}

} // namespace xui