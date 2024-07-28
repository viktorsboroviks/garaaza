#include <algorithm>
#include <set>
#include <vector>

namespace garaza {

template <typename T>
bool vector_contains(const std::vector<T>& v, const T& x)
{
    return std::find(v.begin(), v.end(), x) != v.end();
}

static const size_t FIRST_AVAILABLE_I = SIZE_MAX;

template <typename T>
class Storage {
public:
    size_t add(T x)
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

    size_t add_at(size_t i, T x)
    {
        if (i == FIRST_AVAILABLE_I) {
            return add(x);
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

    void remove(size_t i)
    {
        assert(i < _storage.size());
        assert(!_free_i.contains(i));
        _free_i.insert(i);
        assert(_free_i.size() <= _storage.size());
    }

    T* at(size_t i)
    {
        if (i == FIRST_AVAILABLE_I || i >= _storage.size() ||
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

    size_t max_i()
    {
        const std::vector<size_t> v = all_i();
        return *std::max_element(v.begin(), v.end());
    }

    size_t size()
    {
        return all_i().size();
    }

    size_t max_size()
    {
        return FIRST_AVAILABLE_I - 1;
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
};

}  // namespace garaza
