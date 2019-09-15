//
// Created by toozig on 9/15/19.
//

#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#ifndef HASHMAP_HASHMAP2_HPP
#define HASHMAP_HASHMAP2_HPP

#endif //HASHMAP_HASHMAP2_HPP
#define mapSize(size) pow(2, size)
static const int emptyMap = -1;

static const int defaultSize = 4;
enum reSizeFactors{shrink, enlarg};

template <typename T>
class arrayWrapper
{

public:

    typedef long size_type;
//    class const_iterator
//    {
//    public:
//
//        /**
//         * Typedefs for the array wrapper iterator
//         */
//        typedef const_iterator self_type;
//        typedef T value_type;
//        typedef T& reference;
//        typedef T* pointer;
//        typedef int difference_type;
//        typedef std::forward_iterator_tag iterator_category;
//        explicit const_iterator(pointer ptr) : _ptr(ptr) { }
//        self_type operator++()
//        {
//            _ptr++;
//            return *this ;
//        }
////        self_type operator++(int junk) { _ptr++; return *this; }
//        const value_type &operator*() { return *_ptr; }
//        const value_type *operator->() const { return _ptr; }
//        bool operator==(const self_type& rhs) { return _ptr == rhs._ptr; }
//        bool operator!=(const self_type& rhs) { return _ptr != rhs._ptr; }
//    private:
//        pointer _ptr;
//
//    };

    /**
     * Default Ctor
     * @param size
     */
    explicit arrayWrapper(long size = 0): _size(size), _data(new T[size]){}

    /**
     * Copy Ctor
     */
    arrayWrapper(const arrayWrapper  & other): _size(other._size), _data(new T[other._size])
    {
        std::copy(other._data, other._data + _size, _data);
    }

    bool operator==(const arrayWrapper& other)
    {
        if(_size != other._size) {return false;}
        bool answer = true;
        size_t idx = 0;
        while (answer)
        {
            answer = other._data[idx] == _data[idx];
            ++idx;
        }
        return answer;
    }

    bool operator!=(const arrayWrapper& other)
    {
        return !( *this == other );
    }

    /**
     * Assigment operator
     */
    arrayWrapper& operator=(const arrayWrapper& other)
    {
        if(this != other)
        {
            delete[] _data;
        }
        _size = other._size;
        std::copy(other._data, other._data + _size, _data);
        return *this;
    }

    /**
     * Assigment operator
     */
    arrayWrapper& operator=(arrayWrapper other)
    {

        swap(*this, other);
        return *this;
    }

    /**
     * Move Ctor
     */
    arrayWrapper( arrayWrapper&& other) noexcept : _size(other._size)
    {
        if(*this != other)
        {
            delete[] _data;
        }
        _data = other._data;
        other._data = nullptr;
        other._size = 0;
    }
    /**
     * Move assigment
     */
    arrayWrapper& operator=(arrayWrapper&& other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    long getSize(){ return _size; }

    T& operator[](size_type index) noexcept
    {
        return _data[index];
    }

    const T& operator[](size_type index) const
    {
        return _data[index];
    }

//    const_iterator begin()
//    {
//        return const_iterator(_data);
//    }
//
//    const_iterator end()
//    {
//        const_iterator a  = const_iterator(_data);
////        const_iterator b  = const_iterator(&_data[_size]);
////        ++b;
//        for (int  i = 1 ; i <= _size; i++)
//        {
//            ++a;
//        }
////        bool y = a ==b;
//        return a;
//    }

    ~arrayWrapper()
    {
        delete[] _data;
        _data = nullptr;
    }

    friend void swap (arrayWrapper &a, arrayWrapper & b)
    {
        using std::swap;
        swap(a._data,b._data);
        swap(a._size, b._size);
    }
private:
    T* _data;
    long _size;
};


template < typename keyT, typename valueT>
class HashMap
{
    typedef __gnu_cxx::__normal_iterator<std::pair<keyT, valueT> *, std::vector<std::pair<keyT, valueT>>> iterator;
    typedef std::vector<std::pair<keyT, valueT>> bucket;

    /**
 * The default value for lower bound
 */
    constexpr static  double DEFAULT_LOW_FACTOR = 0.25;

    /**
     * The default value for upper bound
     */
    constexpr static double DEFAULT_UP_FACOTR = 0.75;
public:

    /**
     * Default Ctor
     * @param lowerBound
     */
    explicit HashMap(double lowerBound = DEFAULT_LOW_FACTOR, double upBound = DEFAULT_UP_FACOTR):
            _lowerBound(lowerBound),
            _upperBound(upBound),
            _size(defaultSize),
            _counter(0),
    _map(new arrayWrapper<bucket>(mapSize(defaultSize)))
    {
        if(_lowerBound < 0)
        {
            //todo throw exception
        }
    }

    explicit HashMap(const std::vector<keyT> & keys, const std::vector<valueT> & values):
    _lowerBound(DEFAULT_LOW_FACTOR),
            _upperBound(DEFAULT_UP_FACOTR),
            _size(defaultSize),
            _counter(0)
    {
        size_t keyNums = keys.size();
        if(std::set<keyT>(keys.begin(), keys.end()).size() != keyNums
        || keys.size() != values.size())
        {
            //todo throw
        }
        while( (_counter / mapSize(_size)) > _upperBound * mapSize(_size))
        {
         ++_size;
        }
        _map = new arrayWrapper<bucket>(mapSize(_size));
        long hash;
        for (int i = 0; i < keyNums; ++i)
        {
            hash = hashKey(keys[i]);
            (*_map)[hash]->push_back(std::make_pair(keys[i], values[i]));
        }
    }

    /**
     * Copy Ctor
     */
    HashMap(const HashMap& other):
            _lowerBound(other._lowerBound),
            _upperBound(other._upperBound),
            _size(other._size),
            _counter(other._counter),
            _map(new arrayWrapper<bucket>(*other._map)) {}

    /**
     * Assigment Ctor
     */
    HashMap& operator=( HashMap other)
    {
        swap(*this, other);
        return *this;
    }


    /**
     * move Ctor
     */
    HashMap( HashMap&& other) noexcept
            :_lowerBound(other._lowerBound),
             _upperBound(other._upperBound),
             _size(other._size),
             _counter(other._counter),
             _map(other._map)
    {
        other._map = nullptr;
    }

    ~HashMap()
    {
        delete(_map);
        _map = nullptr;
    }

    class mapIterator
    {
    public:

//        /**
//         * end iterator
//         */
//        explicit mapIterator(HashMap map, long size)
//                :_outIdx(0), _arr(bucket()), _arrPtr(nullptr), _idx(-1),_end(pPair){};

        /**
       * Typedefs for the array wrapper iterator
       */
        typedef mapIterator self_type;
        typedef std::pair<keyT, valueT> value_type;
        typedef std::pair<keyT, valueT>& reference;
        typedef std::pair<keyT, valueT>* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;
        explicit mapIterator(arrayWrapper<bucket> *arr, long outIdx = 0, long inIdx = 0)
                : _arr(arr), _outIdx(outIdx), _inIdx(inIdx)
        {
            forward();
        };


        self_type operator++()
        {
            if(_outIdx < _arr->getSize()) {++_inIdx; }
            forward();
            return *this;
        }

        const value_type &operator*()
        {


            return (*_arr)[_outIdx].at(_inIdx);
        }

        const value_type *operator->()
        {
            return &(*_arr)[_outIdx].at(_inIdx);
        }

        bool operator==(const mapIterator& rhs)
        {

            return _arr == rhs._arr  && _outIdx == rhs._outIdx && _inIdx == rhs._inIdx;
        }

        bool operator!=(const self_type& rhs) {
            return !(*this == rhs);
        }
    private:

        void forward()
        {
            if(_outIdx == _arr->getSize()) { return;} //todo double check if needed
            if(_inIdx == (*_arr)[_outIdx].size())
            {
                _inIdx = 0;
                bool empty_bucket =  true;
                while (_outIdx < _arr->getSize() && empty_bucket)
                {
                    ++_outIdx;
                    if(_outIdx < _arr->getSize()) {empty_bucket = (*_arr)[_outIdx].empty(); }

                }
            }
        }

        arrayWrapper<bucket> * _arr;
        long _outIdx;
        long _inIdx;
    };

    mapIterator begin() const
    {
        return mapIterator(_map);
    }
    mapIterator end() const
    {
        long outIdx = _map->getSize();
        long inIDx = (*_map)[outIdx - 1].size();
        return mapIterator(_map, outIdx, inIDx);
    }
    bool operator==(const HashMap & other) const
    {
        if(_counter != other._counter){ return false;}
        for(auto &pair : other)
        {
            long hash = hashKey(pair.first);
            long idx = getIdx(pair, hash);
            if(idx == (*_map)[hash].size() || idx == emptyMap) { return  false;}
        }
        return true;
    }
    bool  operator!=(const HashMap &other) const
    {
        return  !(*this == other);
    }

    /**
//     * Ctor that makes map from 2 vectors of keysand values
//     * @param keys vector of keys
//     * @param values vector of values
//     */
    HashMap(const std::vector<unsigned  long> & keys, const std::vector<valueT> & values)
    {
        // make sure same _size and all the keys are different
        if(keys.size() != values.size() && std::set<keyT>(keys.begin(), keys.end()).size() != keys.size())

        {
            //todo  throw exception
        }
        //todo (need insert and all of that shit)

    };//todo this

    /**
     * returns the amount of keys inside the map
     */
    int size(){ return _size; }

    /**
     * returns the current capacity of themap
     * @return
     */
    int capacity(){ return mapSize(_size); }

    /**
     *
     * @return the lower load factor
     */
    double getLoadFactor(){ return _lowerBound; }

    /**
     * returns true if the map is empty
     */
    bool empty() { return !_counter; }

    /**
     * Insert a key and a value to the map.
     * if the key exist, it replace it's value
     * @return true if insert correctly
     */
    bool insert(const keyT &key, const valueT &value)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        //checks if the key already exist
        if(0 <= idx < (*_map)[hash].size()){ return false;}
        ++_counter;
        std::pair<keyT, valueT> a = std::make_pair(key, value);
        (*_map)[hash].push_back(a);
        if((_counter / mapSize(_size)) > _upperBound * mapSize(_size)){ resize(enlarg); }
        return true;
    }

    /**
     * Checks if the map contains a key
     */
    bool containKey(const keyT &key) const
    {
        long hash = hashKey(key);
        long size = (*_map)[hash].size();
        long idx =  getIdx(key, hash);
        return 0 <= idx && idx < size;
    }

    /**
     *
     * @param key the key which the buckets belongs to
     * @return the bucket size of a given key
     */
    int bucketSize(const keyT &key) const
    {

        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        return idx == emptyMap || idx == (*_map)[hash].size() ? 0 :(int) (*_map)[hash].size();
    }

    /**
     * clear the map
     */
    void clear()
    {
        *this = HashMap();
    }

    valueT& operator[](const keyT &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < (*_map)[hash].size())
        {
            return (*_map)[hash][idx].second;
        }
        std::pair<keyT, valueT> pair = std::make_pair(key, valueT());
        (*_map)[hash].push_back(pair);
        ++_counter;
        if((_counter / mapSize(_size)) > _upperBound){ resize(enlarg); }
        return (*--(*_map)[hash].end()).second;
    }

    bool erase(const keyT &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(idx == (*_map)[hash].size() || !_counter) { return false; } //key is not in the map
        (*_map)[hash].erase((*_map)[hash].begin() + idx);
        --_counter;
        if((_counter / mapSize(_size)) < _lowerBound){ resize(shrink); }
        return true;
    }

    /**
     * returns the value of a given key
     */
    valueT& at(const keyT &key) const
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < (*_map)[hash].size())
        {
            return (*_map)[hash][idx].second;
        }
        else throw ;

    }

private:

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    long getIdx(const std::pair<keyT, valueT> &pair, const long hash) const
    {
        if(!_counter){ return emptyMap;}
        bucket list =  (*_map)[hash];
        iterator item = (std::find(list.begin(), list.end(), pair));
        return std::distance(list.begin(), (std::find(list.begin(), list.end(), pair)));
    }

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    long getIdx(const keyT &key, const long hash) const
    {
        if(!_counter){ return emptyMap;}
        bucket list =  (*_map)[hash];
        std::pair<keyT, valueT> result;
        for(std::pair<keyT, valueT> pair : list)
        {
            if(pair.first == key)
            {
                result = pair;
                break;
            }
        }
        return std::distance(list.begin(), (std::find(list.begin(), list.end(), result)));
    }

    /**
     * Resize the hashmap
     */
    void resize(reSizeFactors factor)
    {
        if(!_size){ return; }
        _size = factor == enlarg ? ++_size : --_size;
        auto * tmp = new arrayWrapper<bucket>(mapSize(_size));
        for (std::pair<keyT, valueT> pair: *this)
        {
            bucket list = (*tmp)[hashKey(pair.first)];
            list.emplace_back(pair);
        }
        delete _map;
        _map = tmp;
    }

    /**
     * Swap method for the Hashmap
     */
    friend void swap(HashMap& first, HashMap & second)
    {
        using std::swap;
        swap(first._map, second._map);
        swap(first._counter, second._counter);
        swap(first._size, second._size);
        swap(first._lowerBound, second._lowerBound);
    }

    /**
    * returns the hash value of a given key
    */
    long hashKey(const keyT &key) const {
        long hash;
        hash = std::hash<keyT>{}(key);
        return  hash &  ((long ) mapSize(_size) - 1);}

    size_t _size;
    size_t _counter;
    double _lowerBound;
    double _upperBound;
    arrayWrapper<bucket> *_map;
};





