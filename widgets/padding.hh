#pragma once

#include "core/layout_tree.hh"
#include "widgets/widget.hh"

namespace xui
{

template <typename T>
class Padding final
{
public:
    /**
     * Implements:
     * 1. In-place constructor of Wrapper<AnyBar>
     * 2. Constructor of Wrapper<NonmovableBar&> from existing NonmovableBar
     * 3. Constructor of Wrapper<MovableFoo> from existing MovableBar passed as rvalue
     *
     * Supports nesting.
     */
    template <typename... ArgsT,
  			  typename = std::enable_if_t<(!std::is_same_v<std::decay<ArgsT>, Padding> && ...)>>
    explicit Padding( ArgsT&&... args)
  		: child_{ std::forward<ArgsT>( args)...}
        , left_{ kNotInitialized}
        , right_{ kNotInitialized}
        , top_{ kNotInitialized}
        , bottom_{ kNotInitialized}
    {}

  	Padding( const Padding& other) = delete;
  	Padding& operator=( const Padding& other) = delete;

  	Padding( Padding&& other) = delete;
  	Padding& operator=( Padding&& other) = delete;

    ~Padding() = default;

public:
    void setPadding( float left,
                     float right,
                     float top,
                     float bottom)
    {
        left_ = left;
        right_ = right;
        top_ = top;
        bottom_ = bottom;
    }

    void setPadding( float val) { setPadding( val, val, val, val); }

    float getLeft()   const { verify(); return left_;   }
    float getRight()  const { verify(); return right_;  }
    float getTop()    const { verify(); return top_;    }
    float getBottom() const { verify(); return bottom_; }

    const T& getChild() const { return child_; }
          T& getChild()       { return child_; }

    void
    verify() const
    {
        assert( left_ != kNotInitialized
                && right_ != kNotInitialized
                && top_ != kNotInitialized
                && bottom_ != kNotInitialized
                && "Padding wasn't set");
    }

private:
    T child_;

    static constexpr float kNotInitialized = -1;

    float left_;
    float right_;
    float top_;
    float bottom_;
};

template <typename T>
inline void
Render( const Padding<T>& padding,
        const Geometry&,
        sf::RenderTarget&)
{
    padding.verify();
}

template <typename T>
inline LayoutObject
Layout( const Padding<T>& padding,
        const Constraints& cons)
{
    padding.verify();

    const sf::Vector2f diff{ padding.getLeft() + padding.getRight(), padding.getTop() + padding.getBottom()};

    LayoutObject child = Layout( padding.getChild(), Constraints{ cons - diff});
    child.setPosition( { padding.getLeft(), padding.getTop()});

    Geometry geom{ {0, 0}, child.getSize() + diff};
    LayoutObject self{ padding, geom, 1};
    self.push_back( std::move( child));

    return self;
}

} // namespace xui
