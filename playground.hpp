//
// Created by toozi on 9/16/2019.
//




//
// Created by toozig on 9/15/19.
//

#include <algorithm>
#include <vector>
#include <set>
#include <cmath>
#include <stdexcept>
#include <cmath>
#include <iostream>

#ifndef UNTITLED3_PLAYGROUND_HPP
#define UNTITLED3_PLAYGROUND_HPP

#endif //HASHMAP_HASHMAP2_HPP
static const int emptyMap = -1;

static const int DEFAULT_SIZE = 4;
static const int SIZE_BASE = 2;
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
    explicit arrayWrapper(long size = 0): _size(size), _data(new T[size])
    {

    }

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


//template < unsigned long,  unsigned int>
class HashMap
{
    typedef __gnu_cxx::__normal_iterator<std::pair<unsigned long, unsigned int> *, std::vector<std::pair<unsigned long, unsigned int>>> iterator;
    typedef std::vector<std::pair<unsigned long, unsigned int>> bucket;

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
            _size(DEFAULT_SIZE),
            _counter(0),
            _map(new bucket[mapSize(DEFAULT_SIZE)])
    {
        if(_lowerBound < 0)
        {
            throw std::invalid_argument("Invalid load factor") ;
        }
    }

    explicit HashMap(const std::vector<unsigned long> & keys, const std::vector<unsigned int> & values):
            _lowerBound(DEFAULT_LOW_FACTOR),
            _upperBound(DEFAULT_UP_FACOTR),
            _size(DEFAULT_SIZE),
            _counter(0)
    {
        size_t keyNums = keys.size();
        if(std::set<unsigned long>(keys.begin(), keys.end()).size() != keyNums
           || keys.size() != values.size())
        {
            throw std::invalid_argument("Vector not in the same size") ;
        }
        while((_counter / mapSize( _size)) > _upperBound * mapSize( _size))
        {
            ++_size;
        }
        _map = new bucket[mapSize(DEFAULT_SIZE)];
        long hash;
        for (size_t i = 0; i < keyNums; ++i)
        {
            hash = hashKey(keys[i]);
            _map[hash].push_back(std::make_pair(keys[i], values[i]));
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
            _map(new bucket[mapSize(other._size)])
    {
        std::copy(other._map, other._map + _size, _map);
    }

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

    class const_iterator
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
        typedef const_iterator self_type;
        typedef std::pair<unsigned long, unsigned int> value_type;
        typedef std::pair<unsigned long, unsigned int>& reference;
        typedef std::pair<unsigned long, unsigned int>* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;
        explicit const_iterator(arrayWrapper<bucket> *arr, long outIdx = 0, long inIdx = 0)
                : _arr(arr), _outIdx(outIdx), _inIdx(inIdx)
        {
            forward();
        };

        /**
         * prefix ++ operator
         */
        self_type operator++()
        {
            if(_outIdx < _arr->getSize()) {++_inIdx; }
            forward();
            return *this;
        }

        self_type operator++(int)  {
            self_type i = *this;
            if(_outIdx < _arr->getSize()) {++_inIdx; }
            forward();
            return i; }

        const value_type &operator*()
        {


            return (*_arr)[_outIdx].at(_inIdx);
        }

        const value_type *operator->()
        {
            return &(*_arr)[_outIdx].at(_inIdx);
        }

        bool operator==(const const_iterator& rhs)
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

    const_iterator begin() const
    {
        return const_iterator(_map);
    }

    const_iterator cbegin() const
    {
        return const_iterator(_map);
    }

    const_iterator end() const
    {
        long outIdx = mapSize(_size);
        long inIDx = _map[outIdx - 1].size();
        return const_iterator(_map, outIdx, inIDx);
    }

    const_iterator cend() const
    {
        return end();
    }

    bool operator==(const HashMap & other) const
    {
        if(_counter != other._counter){ return false;}
        for(auto &pair : other)
        {
            long hash = hashKey(pair.first);
            long idx = getIdx(pair, hash);
            if(idx == _map[hash].size() || idx == emptyMap) { return  false;}
        }
        return true;
    }
    bool  operator!=(const HashMap &other) const
    {
        return  !(*this == other);
    }


    /**
     * returns the amount of keys inside the map
     */
    int size(){ return _counter; }

    /**
     * returns the current capacity of themap
     * @return
     */
    int capacity(){ return mapSize( _size); }

    /**
     *
     * @return the lower load factor
     */
    double getLoadFactor(){ return _counter / mapSize(_size); }

    /**
     * returns true if the map is empty
     */
    bool empty() { return !_counter; }

    /**
     * Insert a key and a value to the map.
     * if the key exist, it replace it's value
     * @return true if insert correctly
     */
    bool insert(const unsigned long &key, const unsigned int &value)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        //checks if the key already exist
        if(0 <= idx < _map[hash].size()){ return false;}
        ++_counter;
        _map[hash].push_back( std::make_pair(key, value));

        if( getLoadFactor() > _upperBound){ resize(enlarg); }
        return true;
    }

    /**
     * Checks if the map contains a key
     */
    bool containsKey(const unsigned long &key) const
    {
        long hash = hashKey(key);
        long size = _map[hash].size();
        long idx =  getIdx(key, hash);
        return 0 <= idx && idx < size;
    }

    /**
     *
     * @param key the key which the buckets belongs to
     * @return the bucket size of a given key
     */
    int bucketSize(const unsigned long &key) const
    {

        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        return idx == emptyMap || idx == _map[hash].size() ? 0 :(int) _map[hash].size();
    }

    /**
     * clear the map
     */
    void clear()
    {
        *this = HashMap();
    }

    unsigned int& operator[](const unsigned long &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < _map[hash].size())
        {
            return _map[hash][idx].second;
        }
        std::pair<unsigned long, unsigned int> pair = std::make_pair(key, (unsigned int) int()); //todo change it
        _map[hash].push_back(pair);
        ++_counter;
        if(getLoadFactor() > _upperBound){ resize(enlarg); }
        return (*--_map[hash].end()).second;
    }

    unsigned int& operator[](const unsigned long &key) const
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < _map[hash].size())
        {
            return _map[hash][idx].second;
        }
        throw std::exception(); //todo throw exception
    }

    bool erase(const unsigned long &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(idx == _map[hash].size() || !_counter) { return false; } //key is not in the map
        _map[hash].erase(_map[hash].begin() + idx);
        --_counter;
        if(getLoadFactor() < _lowerBound){ resize(shrink); }
        return true;
    }

    /**
     * returns the value of a given key
     */
    unsigned int& at(const unsigned long &key) const
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < _map[hash].size())
        {
            return _map[hash][idx].second;
        }
        else throw std::invalid_argument("The key does not exist") ;

    }

private:

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    long getIdx(const std::pair<unsigned long, unsigned int> &pair, const long hash) const
    {
        if(!_counter){ return emptyMap;}
        bucket list =  _map[hash];
        iterator item = (std::find(list.begin(), list.end(), pair));
        return std::distance(list.begin(), (std::find(list.begin(), list.end(), pair)));
    }

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    long getIdx(const unsigned long &key, const long hash) const
    {
        if(!_counter){ return emptyMap;}
        bucket list =  _map[hash];
        std::pair<unsigned long, unsigned int> result;
        for(std::pair<unsigned long, unsigned int> pair : list)
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
        auto* tmp = new bucket[mapSize(_size)];
        for (std::pair<unsigned long, unsigned int> pair: *this)
        {
            bucket list = tmp[hashKey(pair.first)];
            list.push_back(pair);
        }
        delete[] _map;
        _map = tmp;
        auto p =tmp->operator[](0);
        for (std::pair<unsigned long, unsigned int> pair: *this)
        {
            int a =4;
        }

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
    long hashKey(const unsigned long &key) const {
        long hash ;
        hash = std::hash<unsigned long>{}(key) ;
        return  hash &  ((long ) mapSize( _size) - 1) ;
    }

    /**
     * returns the current map size
     */
    int mapSize(const int size) const
    {
        double result = pow(SIZE_BASE,size);
        return result;
    }

    int _size;
    size_t _counter;
    double _lowerBound;
    double _upperBound;
    bucket*_map;
};




