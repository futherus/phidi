#pragma once

#include <utility>
#include <list>
#include <map>
#include <string>

//
// LRU cache discards the least recently used items first.
//
// Implementation details:
//
// List handles both value and key in the following format:
// most recently used [key, value] -> [key, value] -> ... -> [key, value] least recently used
//
// Map handles iterators to list elements to provide O(lg N) search.
//
template <typename Key, typename Value>
class LRUCache final
{
public:
    using mapped_type    = Value;
    using key_type       = Key;
    using value_type     = std::pair<const key_type, mapped_type>;
    using list_type      = std::list<value_type>;
    using map_type       = std::map<const key_type, typename list_type::iterator>;
    using iterator       = typename list_type::iterator;
    using const_iterator = typename list_type::const_iterator;

    iterator            begin()       { return list_.begin(); }
    const_iterator      begin() const { return list_.begin(); }
    iterator              end()       { return list_.end();   }
    const_iterator        end() const { return list_.end();   }

    LRUCache( size_t capacity)
        : capacity_{ capacity}
    {}

    ~LRUCache() = default;

    size_t size()     const noexcept { return list_.size(); }
    size_t capacity() const noexcept { return capacity_; }
    bool   empty()    const noexcept { return list_.empty(); }

    bool contains( const key_type &key)
    {
        return map_.find( key) != map_.end();
    }

    void insert( const key_type &key,
                 mapped_type value)
    {
        if ( !contains( key) )
        {
            if ( size() >= capacity() )
            {
                typename list_type::iterator it = --list_.end();
                map_.erase( it->first);
                list_.erase( it);
            }

            list_.push_front( std::make_pair( key, std::move( value)));
            map_.insert( std::make_pair( key, list_.begin()));
        }
    }

    iterator find( const key_type &key)
    {
        typename map_type::iterator it = map_.find( key);
        if ( it == map_.end() )
        {
            return list_.end();
        }

        typename list_type::iterator lru = it->second;
        if ( lru != list_.begin() )
        {
            list_.splice( list_.begin(), list_, lru );
            map_[key] = list_.begin();
        }

        return list_.begin();
    }

private:
    list_type list_;
    map_type map_;
    size_t capacity_;
};

