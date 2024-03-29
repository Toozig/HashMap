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



static const int NOT_FOUND = -1;

static const int DEFAULT_SIZE = 4;
static const int SIZE_BASE = 2;
static const int DEFAULT = -1;
enum reSizeFactors{shrink, enlarg};


//template <typename unsigned long, typename  char>
class HashMap
{
    typedef __gnu_cxx::__normal_iterator<std::pair<unsigned long, char> *, std::vector<std::pair<unsigned long, char>>> iterator;
    typedef std::vector<std::pair<unsigned long, char>> bucket;

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
            _map(new bucket*[ (int) mapSize(DEFAULT_SIZE)])
    {
        if(_lowerBound > _upperBound || _upperBound < 0 || _upperBound > 1 || _lowerBound > 1 || _lowerBound < 0 )
        {
            throw std::invalid_argument("Invalid parameters") ;
        }
        for (int i = 0; i < capacity(); ++i)
        {
            _map[i] = nullptr;
        }
    }

    explicit HashMap(const std::vector<unsigned long> & keys, const std::vector<char> & values): //todo change to pointer of poitner arr
            _lowerBound(DEFAULT_LOW_FACTOR),
            _upperBound(DEFAULT_UP_FACOTR),
            _size(DEFAULT_SIZE),
            _counter(0)
    {
        size_t keyNums = keys.size();
        if(keys.size() != values.size())
        {
            throw std::invalid_argument("Vector not in the same size") ;
        }

        while((keyNums / mapSize( _size)) > _upperBound)
        {
            ++_size;
        }
        _map = new bucket*[(int) mapSize(_size)];
        for (int j = 0; j < mapSize(_size); ++j)
        {
            _map[j] = nullptr;
        }
        long hash;
        for (size_t i = 0; i < keyNums; ++i)
        {
            hash = hashKey(keys[i]);
            if(_map[hash] == nullptr)
            {
                _map[hash] = new bucket() ;
                _map[hash]->push_back(std::make_pair(keys[i], values[i]));
                ++_counter;
            } else{
                int idx = getIdx(keys[i], hash);
                if(idx == NOT_FOUND)
                {
                    _map[hash]->push_back(std::make_pair(keys[i], values[i]));
                    ++_counter;
                }
                else
                {
                    this[keys[i]] = values[i];
                }
            }
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
            _map(nullptr)
    {
        *this = other;
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
        for (int i = 0; i < capacity() ; ++i)
        {
            delete _map[i];
            _map[i] = nullptr;
        }
        delete[] _map;
        _map = nullptr;
    }

    class const_iterator
    {
    public:
        /**
       * Typedefs for the array wrapper iterator
       */
        typedef const_iterator self_type;
        typedef std::pair<unsigned long, char> value_type;
        typedef std::pair<unsigned long, char>& reference;
        typedef std::pair<unsigned long, char>* pointer;
        typedef int difference_type;
        typedef std::forward_iterator_tag iterator_category;
        explicit const_iterator(const HashMap &map, long outIdx = 0, long inIdx = 0)
                : _arr(map._map), _arrSize(pow(SIZE_BASE, map._size)), _outIdx(outIdx), _inIdx(inIdx)
        {
            forward();
        };

        /**
         * prefix ++ operator
         */
        self_type operator++()
        {
            if(_outIdx < _arrSize) {++_inIdx; }
            forward();
            return *this;
        }

        self_type operator++(int)  {
            self_type i = *this;
            if(_outIdx < _arrSize) {++_inIdx; }
            forward();
            return i; }

        const value_type &operator*()
        {
            return _arr[_outIdx]->at(_inIdx);
        }

        const value_type *operator->()
        {
            return &_arr[_outIdx]->at(_inIdx);
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
            if(_outIdx == _arrSize) { return;} //todo double check if needed
            if(_arr[_outIdx] == nullptr || _inIdx == _arr[_outIdx]->size())
            {
                _inIdx = 0;
                bool empty_bucket =  true;
                while (_outIdx < _arrSize && empty_bucket)
                {
                    ++_outIdx;
                    if(_outIdx < _arrSize)
                    {empty_bucket = _arr[_outIdx] == nullptr; }
                }
            }
        }

        size_t _arrSize;
        bucket ** _arr;
        long _outIdx;
        long _inIdx;
    };

    const_iterator begin() const
    {
        return const_iterator(*this);
    }

    const_iterator cbegin() const
    {
        return const_iterator(*this);
    }

    const_iterator end() const
    {
        long outIdx = mapSize(_size);
        return const_iterator(*this, outIdx);
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
            if(idx == _map[hash]->size() || idx == NOT_FOUND) { return  false;}
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
    double getLoadFactor(){return _counter / mapSize(_size);}

    /**
     * returns true if the map is empty
     */
    bool empty() { return !_counter; }

    /**
     * Insert a key and a value to the map.
     * if the key exist, it replace it's value
     * @return true if insert correctly
     */
    bool insert(const unsigned long &key, const char &value)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        //checks if the key already exist
        if(0 <= idx ){ return false;}
        ++_counter;
        if( _map[hash] == nullptr) { _map[hash] = new bucket; }
        _map[hash]->push_back( std::make_pair(key, value));
        if( getLoadFactor() > _upperBound){ resize(enlarg); }
        return true;
    }

    /**
     * Checks if the map contains a key
     */
    bool containsKey(const unsigned long &key) const
    {
        long hash = hashKey(key);
        long idx =  getIdx(key, hash);
        return idx != NOT_FOUND;
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
        return idx == NOT_FOUND ? 0 : (int) _map[hash]->size();
    }

    /**
     * clear the map
     */
    void clear()
    {
        *this = HashMap();
    }

    char& operator[](const unsigned long &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < _map[hash]->size())
        {
            return _map[hash]->at(idx).second;
        }
        std::pair<unsigned long, char> pair = std::make_pair(key, char()); //todo change it
        _map[hash]->push_back(pair);
        ++_counter;
        if(getLoadFactor() > _upperBound){ resize(enlarg); }
        return (*--_map[hash]->end()).second;
    }

    char& operator[](const unsigned long &key) const
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash) < _map[hash]->size())
        {
            return _map[hash]->at(idx).second;
        }
        throw std::invalid_argument("key does not exist"); //todo throw exception
    }

    bool erase(const unsigned long &key)
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(idx == NOT_FOUND) { return false; } //key is not in the map
        _map[hash]->erase(_map[hash]->begin() + idx);
        --_counter;
        if(!_map[hash]->size())
        {
            delete _map[hash];
            _map[hash] = nullptr;
        }
        if(getLoadFactor() < _lowerBound){ resize(shrink); }
        return true;
    }

    /**
     * returns the value of a given key
     */
    char& at(const unsigned long &key) const
    {
        long hash = hashKey(key);
        long idx = getIdx(key, hash);
        if(0 <= getIdx(key, hash))
        {
            return _map[hash]->at(idx).second;
        }
        else throw std::invalid_argument("The key does not exist") ;

    }

private:

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if bucket is empty
     */
    long getIdx(const std::pair<unsigned long, char> &pair, const long hash) const
    {
        if(!_counter || _map[hash] == nullptr ){ return NOT_FOUND;}
        bucket *list =  _map[hash];
        iterator item = (std::find(list->begin(), list->end(), pair));
        int idx =  std::distance(list->begin(), (std::find(list->begin(), list->end(), pair)));
        return idx == _map[hash]->size() ? NOT_FOUND : idx;
    }

    /**
     * gets the index of a key inside the bucket.
     * @param pair
     * @return  the idx, -1 if wasnt found
     */
    long getIdx(const unsigned long &key, const long hash) const
    {
        if(!_counter|| _map[hash] == nullptr){ return NOT_FOUND;}
        bucket *list =  _map[hash];
        std::pair<unsigned long, char> result;
        for(std::pair<unsigned long, char> pair : *list)
        {
            if(pair.first == key)
            {
                result = pair;
                break;
            }
        }
        int idx =  std::distance(list->begin(), (std::find(list->begin(), list->end(), result)));
        return idx == _map[hash]->size() ? NOT_FOUND : idx;
    }

    /**
     * Resize the hashmap
     */
    void resize(reSizeFactors factor)
    {
        if(!_size){ return; }
        int addition = factor == enlarg ? 1 : -1;
        auto** tmp = new bucket*[ (int) mapSize(_size + factor)] ;
        for (int j = 0; j < mapSize(_size + factor); ++j)
        {
            tmp[j] = nullptr;
        }
        for (std::pair<unsigned long, char> pair: *this)
        {
            int hash = hashKey(pair.first, _size + factor);
            if(tmp[hash] == nullptr) { tmp[hash] = new bucket;}
            std::cout << pair.first << ' ';
            tmp[hash]->push_back(pair);
        }
        for (int i = 0; i <mapSize(_size - factor) ; ++i)
        {
            delete _map[i];
            _map[i] = nullptr;
        }
        delete[] _map;
        _size += addition;
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
    long hashKey(const unsigned long &key, int size = DEFAULT) const {
        size = size == DEFAULT? _size : size;
        long hash ;
        hash = std::hash<unsigned long>{}(key) ;
        return  hash &  ((long ) mapSize( size) - 1) ;
    }

    /**
     * returns the current map size
     */
    double mapSize(const int size) const
    {
        return pow(SIZE_BASE,size);
    }

    double _size;
    double _counter;
    double _lowerBound;
    double _upperBound;
    bucket**_map;
};




