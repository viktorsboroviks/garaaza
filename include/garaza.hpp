#include <set>
#include <vector>

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

    void remove(size_t i)
    {
        assert(i < _storage.size());
        assert(!_free_i.contains(i));
        _free_i.push(i);
        assert(_free_i.size() <= _storage.size());
    }

    T* at(size_t i)
    {
        if (i >= _storage.size() || _free_i.contains(i)) {
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

private:
    std::vector<T> _storage;
    std::set<size_t> _free_i;
};

}  // namespace garaza
