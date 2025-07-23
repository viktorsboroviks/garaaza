#pragma once
#include <algorithm>
#include <cassert>
#include <deque>
#include <list>
#include <set>
#include <vector>

#include "rododendrs.hpp"

namespace garaaza {

template <typename Tcontainer, typename Tvalue>
bool contains(const<Tcontainer<Tvalue>>& v, const T& x)
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
            // cppcheck-suppress useStlAlgorithm
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
            size_t i    = *_free_i.begin();
            _storage[i] = x;
            _free_i.erase(i);
            return i;
        }

        // if no free storage available, extend storage
        _storage.push_back(x);
        return _storage.size() - 1;
    }
};

template <typename T, auto comp_fn>
class SortedStorage {
private:
    std::vector<T> _data;  // allocate once, do not resize
    std::vector<size_t> _idx_sorted;
    std::deque<size_t> _idx_unsorted;
    std::deque<size_t> _idx_free;

    size_t _add_to_sorted(size_t i)
    {
        const auto it =
                std::lower_bound(_idx_sorted.begin(),
                                 _idx_sorted.end(),
                                 i,
                                 [this](size_t lhs, size_t rhs) {
                                     return comp_fn(_data[lhs], _data[rhs]);
                                 });
        _idx_sorted.insert(it, i);
        // no sense to empty the removed data cell itself

        assert(_idx_sorted.front() <= _idx_sorted.back());
        assert(_idx_sorted.size() + _idx_free.size() + _idx_unsorted.size() ==
               _data.size());
    }

public:
    explicit SortedStorage(size_t n)
    {
        _data.resize(n);
        _idx_sorted.reserve(n);
        for (size_t i = 0; i < n; i++) {
            _idx_free.push_back(i);
        }
    }

    void reset()
    {
        _idx_sorted.clear();
        _idx_free.clear();
        _idx_unsorted.clear();
        for (size_t i = 0; i < _data.capacity(); i++) {
            _idx_free.push_back(i);
        }
    }

    size_t capacity() const
    {
        assert(_data.capacity() == _idx_sorted.capacity());
        return _data.capacity();
    }

    size_t size() const
    {
        return _idx_sorted.size();
    }

    bool empty() const
    {
        return _idx_sorted.empty();
    }

    bool full() const
    {
        return _idx_free.empty();
    }

    size_t free_i() const
    {
        assert(!full());
        return _idx_free.front();
    }

    T& at(size_t i)
    {
        assert(i < _data.size());
        return _data[i];
    }

    T& sorted_at(size_t i)
    {
        assert(i < _data.size());
        return _data[_idx_sorted[i]];
    }

    // add a new element by value without processing it
    T& add(const T& val)
    {
        assert(!_idx_free.empty());
        const size_t i = _idx_free.front();
        _idx_free.pop_front();
        _data[i] = val;
        _idx_unsorted.push_back(i);
        return _data[i];
    }

    // add a new element by index without processing it
    T& add(size_t i)
    {
        assert(!full());
        const auto it = std::find(_idx_free.begin(), _idx_free.end(), i);
        assert(it != _idx_free.end());
        _idx_free.erase(it);
        _idx_unsorted.push_back(i);
        return _data[i];
    }

    void sort()
    {
        while (!_idx_unsorted.empty()) {
            const size_t i = _idx_unsorted.front();
            _idx_unsorted.pop_front();
            _add_to_sorted(i);
        }
    }

    void rm_sorted_at(size_t i)
    {
        assert(i < _data.size());
        const size_t rm_i = _idx_sorted[i];
        _idx_sorted.erase(_idx_sorted.begin() + i);
        _idx_free.push_back(rm_i);

        assert(_idx_sorted.front() <= _idx_sorted.back());
        assert(_idx_sorted.size() + _idx_free.size() + _idx_unsorted.size() ==
               _data.size());
    }
};

}  // namespace garaaza
