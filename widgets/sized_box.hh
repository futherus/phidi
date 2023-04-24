#pragma once

#include "widgets/widget.hh"
#include "core/layout_tree.hh"

namespace xui
{

template <typename T>
class SizedBox final
{
public:
    /**
     * Implements:
     * 1. In-place constructor of SizedBox<AnyBar>
     * 2. Constructor of SizedBox<NonmovableBar&> from existing NonmovableBar
     * 3. Constructor of SizedBox<MovableFoo> from existing MovableBar passed as rvalue
     *
     * Supports nesting.
     */
    template <typename... ArgsT,
  			  typename = std::enable_if_t<(!std::is_same_v<std::decay<ArgsT>, SizedBox> && ...)>>
    explicit SizedBox( ArgsT&&... args)
  		: child_{ std::forward<ArgsT>( args)...}
    {}

  	SizedBox( const SizedBox& other) = delete;
  	SizedBox& operator=( const SizedBox& other) = delete;

  	SizedBox( SizedBox&& other) = delete;
  	SizedBox& operator=( SizedBox&& other) = delete;

    ~SizedBox() = default;

public:
    void setSize( float width,
                  float height)
    {
        width_ = width;
        height_ = height;
    }

    float        getWidth()  const { verify(); return width_;  }
    float        getHeight() const { verify(); return height_; }
    sf::Vector2f getSize()   const { verify(); return {width_, height_}; }

    const T& getChild() const { return child_; }
          T& getChild()       { return child_; }

    void
    verify() const
    {
        assert( width_ != kNotInitialized
                && height_ != kNotInitialized
                && "Size wasn't initialized");
    }

private:
    T child_;

    static constexpr float kNotInitialized = -1;

    float width_;
    float height_;
};

template <typename T>
inline void
Render( const SizedBox<T>& sized_box,
        const Geometry&,
        sf::RenderTarget&)
{
    sized_box.verify();
}

template <typename T>
inline LayoutObject
Layout( const SizedBox<T>& sized_box,
        const Constraints& cons)
{
    sized_box.verify();

    assert( cons >= sized_box.getSize());

    LayoutObject child = Layout( sized_box.getChild(), Constraints{ sized_box.getSize()});
    child.setPosition( {0, 0});

    Geometry geom{ {0, 0}, sized_box.getSize()};
    LayoutObject self{ sized_box, geom, 1};
    self.push_back( std::move( child));

    return self;
}

} // namespace xui