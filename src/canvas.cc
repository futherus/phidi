#include "tools.hh"

namespace xui
{

// void Canvas::drawCircle( sf::Vector2f pos, float radius, sf::Color color);
// void Canvas::drawLine( sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color);

// void Canvas::draw( sf::RenderTarget& target) const override;

// void Canvas::onMousePressed ( const sf::Event& event) override;
// void Canvas::onMouseReleased( const sf::Event& event) override;
// void Canvas::onMouseMoved   ( const sf::Event& event) override;


void Canvas::clear()
{
    is_pressed_ = false;

    sf::RectangleShape rect(bounds().size());
    rect.setFillColor(base_color_);

    pixels_.draw(rect);

    pixels_.display();
}

void Canvas::drawCircle(sf::Vector2f pos, float radius, sf::Color color)
{
    sf::CircleShape circ(radius);
    circ.setPosition(pos);
    circ.setOrigin({radius, radius});
    circ.setFillColor(color);
    pixels_.draw(circ);

    pixels_.display();
}

void Canvas::drawLine(sf::Vector2f pos1, sf::Vector2f pos2, float width, sf::Color color)
{
    sf::Vertex line[] = { sf::Vertex(pos1), sf::Vertex(pos2) };
    line[0].color = color;
    line[1].color = color;
    pixels_.draw(line, 2, sf::Lines);

    pixels_.display();
}

void Canvas::draw(sf::RenderTarget& target) const
{
    this->IWidget::draw(target);

    // printf("drawing canvas %lgx%lg at (%lg, %lg)\n", bounds().width(), bounds().height(), bounds().left(), bounds().top());
    // printf("texture: %ux%u\n", texture_.getSize().x, texture_.getSize().y);
    // printf("sprite: (%lg, %lg)\n", sprite_.getPosition().x, sprite_.getPosition().y);
    // std::cout << std::hex << pixels_.at(11) << std::endl;
    target.draw(sprite_);
}

void Canvas::onMousePressed(const sf::Event& event)
{
    // printf("mouse pressed\n");
    // is_pressed_ = true;
    sf::Vector2f pos = {event.mouseButton.x, event.mouseButton.y};
    tool_manager_->getActiveTool()->onMousePressed(this, pos - bounds().tl());

}

void Canvas::onMouseReleased(const sf::Event& event)
{
    // printf("mouse released\n");
    // is_pressed_ = false;
    sf::Vector2f pos = {event.mouseButton.x, event.mouseButton.y};
    tool_manager_->getActiveTool()->onMouseReleased(this, pos - bounds().tl());

}

void Canvas::onMouseMoved(const sf::Event& event)
{
    sf::Vector2f pos = {event.mouseMove.x, event.mouseMove.y};
    tool_manager_->getActiveTool()->onMouseMoved(this, pos - bounds().tl());
}

} // namespace xui