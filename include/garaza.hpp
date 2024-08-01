#pragma once
#include <algorithm>
#include <cassert>
#include <list>
#include <set>
#include <vector>

#include "rododendrs.hpp"

namespace garaza {

template <typename T>
bool vector_contains(const std::vector<T>& v, const T& x)
{
    return std::find(v.begin(), v.end(), x) != v.end();
}

template <typename T>
class Storage {
public:
    size_t add(T x)
    {
        return _add_at_first_available_i(x);
    }

    void replace(T x, size_t i)
    {
        assert(contains_i(i));
        _storage[i] = x;
    }

    void remove(size_t i)
    {
        assert(contains_i(i));
        _free_i.insert(i);
        assert(_free_i.size() <= _storage.size());
    }

    T* at(size_t i)
    {
        assert(i < _storage.size());
        if (_free_i.contains(i)) {
            assert(!contains_i(i));
            return nullptr;
        }

        return &_storage[i];
    }

    const std::vector<size_t> all_i()
    {
        std::vector<size_t> ret;
        for (size_t i = 0; i < _storage.size(); i++) {
            if (!_free_i.contains(i)) {
                ret.push_back(i);
            }
        }

        return ret;
    }

    const std::list<T> list()
    {
        std::list<T> ret;
        for (size_t i : all_i()) {
            ret.push_back(_storage[i]);
        }

        return ret;
    }

    size_t max_i()
    {
        const std::vector<size_t> v = all_i();
        return *std::max_element(v.begin(), v.end());
    }

    size_t rnd_i()
    {
        const auto all = all_i();
        assert(!all.empty());
        const size_t i_i = rododendrs::rnd01() * all.size();
        return all[i_i];
    }

    size_t size()
    {
        return all_i().size();
    }

    bool empty()
    {
        return all_i().size() == 0;
    }

    bool contains(T x)
    {
        for (size_t i = 0; i < _storage.size(); i++) {
            if (!_free_i.contains(i)) {
                if (_storage[i] == x) {
                    return true;
                }
            }
        }
        return false;
    }

    bool contains_i(size_t i)
    {
        return at(i) != nullptr;
    }

private:
    std::vector<T> _storage;
    std::set<size_t> _free_i;

    size_t _add_at_first_available_i(T x)
    {
        assert(_free_i.size() <= _storage.size());

        // first try to occupy free storage
        if (!_free_i.empty()) {
            size_t i = *_free_i.begin();
            _storage[i] = x;
            _free_i.erase(i);
            return i;
        }

        // if no free storage available, extend storage
        _storage.push_back(x);
        return _storage.size() - 1;
    }
};

}  // namespace garaza
