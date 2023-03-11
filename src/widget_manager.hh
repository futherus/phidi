#ifndef WIDGET_MANAGER_HH
#define WIDGET_MANAGER_HH

#include <vector>
#include <memory>
#include "widget.hh"

namespace xui
{

template <typename T>
class WidgetManager
    : public IWidget
{
protected:
    std::vector<T*> widgets_;

public:
    WidgetManager( const Rectangle& bounds)
        : IWidget{ bounds}
        , widgets_{}
    {}

    virtual void add(T* widget)
    {
        widgets_.push_back(widget);
    }

    size_t size() const { return widgets_.size(); }

    void draw(sf::RenderTarget& target) const override
    {
        this->IWidget::draw(target);

        for (size_t i = 0; i < widgets_.size(); i++)
            widgets_.at(i)->draw(target);
    }

    void onMousePressed(const sf::Event& event) override
    {
        sf::Vector2f point(event.mouseButton.x, event.mouseButton.y);
        for (size_t i = 0; i < widgets_.size(); i++)
            if (widgets_.at(i)->contains(point))
                widgets_.at(i)->onMousePressed(event);
    }

    void onMouseReleased(const sf::Event& event) override
    {
        sf::Vector2f point(event.mouseButton.x, event.mouseButton.y);
        for (size_t i = 0; i < widgets_.size(); i++)
            if (widgets_.at(i)->contains(point))
                widgets_.at(i)->onMouseReleased(event);
    }

    void onMouseMoved(const sf::Event& event) override
    {
        for (size_t i = 0; i < widgets_.size(); i++)
            widgets_.at(i)->onMouseMoved(event);
    }

    void onKeyPressed(const sf::Event& event) override
    {
        for (size_t i = 0; i < widgets_.size(); i++)
            widgets_.at(i)->onKeyPressed(event);
    }

    void onKeyReleased(const sf::Event& event) override
    {
        for (size_t i = 0; i < widgets_.size(); i++)
            widgets_.at(i)->onKeyReleased(event);
    }

    void onTextEntered(const sf::Event& event) override
    {
        for (size_t i = 0; i < widgets_.size(); i++)
            widgets_.at(i)->onTextEntered(event);
    }
};

} // namespace xui

#endif // WIDGET_MANAGER_HH
