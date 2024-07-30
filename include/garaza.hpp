#include <algorithm>
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

static const size_t I_FIRST_AVAILABLE = SIZE_MAX;
static const size_t I_RANDOM = I_FIRST_AVAILABLE - 1;

template <typename T>
class Storage {
public:
    size_t add(T x, size_t i = I_FIRST_AVAILABLE)
    {
        if (i == I_FIRST_AVAILABLE) {
            return _add_at_first_available_i(x);
        }

        if (i == I_RANDOM) {
            return _add_at_random_i(x);
        }

        // expand storage if needed
        if (i >= _storage.size()) {
            const size_t new_storage_size = i + 1;
            _storage.resize(new_storage_size);
            for (size_t fi = _storage.size(); fi < new_storage_size; fi++) {
                _free_i.insert(fi);
            }
        }

        _storage[i] = x;
        _free_i.erase(i);
        return i;
    }

    size_t remove(size_t i)
    {
        if (i == I_FIRST_AVAILABLE) {
            return remove(all_i()[0]);
        }

        if (i == I_RANDOM) {
            return remove(rnd_i());
        }

        assert(i < _storage.size());
        assert(!_free_i.contains(i));
        _free_i.insert(i);
        assert(_free_i.size() <= _storage.size());
        return i;
    }

    T* at(size_t i)
    {
        if (i == I_FIRST_AVAILABLE || i == I_RANDOM || i >= _storage.size() ||
            _free_i.contains(i)) {
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

    size_t max_size()
    {
        return I_RANDOM - 1;
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

    size_t _add_at_random_i(T x)
    {
        assert(_free_i.size() <= _storage.size());

        // first try to occupy free storage
        if (!_free_i.empty()) {
            const std::vector<size_t> v_free_i(_free_i.begin(), _free_i.end());
            const size_t i_i = rododendrs::rnd01() * v_free_i.size();
            const size_t i = v_free_i[i_i];
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
