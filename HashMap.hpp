////
//// Created by toozi on 9/11/2019.
////
//
//#include <cmath>
//#include <utility>
//#include <vector>
//#include <list>
//#include <algorithm>
//#include <set>
//#include <string>
//#include <cmath>
//#ifndef UNTITLED3_HASHMAP_HPP
//#define UNTITLED3_HASHMAP_HPP
//#define mapSize(_size) pow(2, _size + 4)
//
//static const int notFound = -1;
//enum sizeFactors {enlarge = 1, shrink = -1};
////    typedef std::vector<std::vector<std::pair<unsigned long, std::string>>> outerVec;
////{
//
//
//
//
//class MapIterator;
//
////template<typename keyT, typename valueT>
//class HashMap
//{
//    typedef std::pair<unsigned long ,unsigned int> pairsT;
//    typedef std::vector<pairsT> bucket;
//    typedef std::vector<bucket> dictionary;
//    typedef std::vector<std::pair<unsigned long, unsigned int>> innerVec;
//    typedef __gnu_cxx::__normal_iterator<const std::pair<unsigned long, unsigned int> *,
//            innerVec> innerIter;
//    typedef __gnu_cxx::__normal_iterator<std::pair<unsigned long, unsigned int> *, innerVec> iterator;
//    typedef std::vector<innerVec> outerVec;
//    typedef __gnu_cxx::__normal_iterator<const innerVec*, outerVec> outerIter;
////    typedef typename std::vector<std::pair<unsigned long, unsigned int>>::iterator mapIterator;
//
//    /**
//     * The default value for lower bound
//     */
//    static const long lowestBound = (1 / 4);
//
//    /**
//     * The default value for upper bound
//     */
//    static const long upperBound = (3 / 4);
//
//public:
//    /**
//     * default Ctor
//     * @param lowerBoundFactor adjusting new
//     */
//    explicit HashMap(double lowerBoundFactor = lowestBound)
//            : _size(0), _lowerBound(lowerBoundFactor), _list(dictionary()),_vCounter(0)
//            {
//        if(lowerBoundFactor < 0)
//        {
//            //todo throw exception
//        }
//            };
//    /**
//     * Ctor that makes map from 2 vectors of keysand values
//     * @param keys vector of keys
//     * @param values vector of values
//     */
//    HashMap(const std::vector<unsigned  long> & keys, const std::vector<unsigned int> & values)
//    {
//        // make sure same _size and all the keys are different
//        if(keys._size() != values._size() && std::set<unsigned long>(keys.begin(), keys.end())._size() != keys._size())
//
//        {
//            //todo  throw exception
//        }
//        //todo (need insert and all of that shit)
//
//    };
//
//    //rule of 5
//    /**
//     * Move Ctor
//     */
//    HashMap(HashMap &&map)
//    : _size(map._size), _lowerBound(map._lowerBound),_list(map._list),_vCounter(map._vCounter){};
//
//    /**
//     * copy Ctor
//     */
//    HashMap(const HashMap &map)
//    :_list(map._list), _lowerBound(map._lowerBound), _size(map._size), _vCounter(map._vCounter){};
//
//    /**
//     * assigment Ctor
//     * @return
//     */
//    HashMap &operator=(const HashMap &other)
//    {
//        _list =  dictionary(other._list.begin(), other._list.end());
//        _size = other._size;
//        _vCounter = other._vCounter;
//        _lowerBound = other._lowerBound;
//        return *this;
//    };
//
//    /**
//     * Move assigment Ctor
//     */
//    HashMap &operator=(HashMap &&other) noexcept
//    {
//        _list = other._list;
//        _size = other._size;
//        _lowerBound = other._lowerBound;
//        _vCounter = other._vCounter;
//        return *this;
//    };
//
//    /**
//     * Dtor
//     */
//    ~HashMap() = default;
//    /**
//     * @return the amount of values inside the map
//     */
//    int _size(){return _vCounter;}
//
//    /**
//     * @return the current capacity of the hashmap
//     */
//     int capacity(){ return mapSize(_size);}
//
//     /**
//      * @return  true if the hashMap is empty
//      */
//      bool empty(){ return  _vCounter ==0;}
//
//      /**
//       * @return true of the map contains the given key
//       */
//       bool containsKey(const unsigned long key) const
//      {
//           long hash = hashKey(key);
//           long idx = getIdx(key, hash);
//          return  idx != notFound;
//      }
//
//      /**
//       * returns the value of a given key
//       */
//      unsigned int at(const unsigned long &key) const
//      {
//          long hash = hashKey(key);
//          long idx = getIdx(key, hash);
//            if( idx == notFound)
//            {
//                // todo throw exception
//            }
//          return _list[hash][idx].second;
//      }
//
//      bool erase(unsigned long key)
//      {
//          long hash = hashKey(key);
//          long idx = getIdx(key, hash);
//          if(idx == notFound)
//          {
//              return false;
//          }
//          _list.at(hash).erase(_list.at(hash).begin() + idx);
//          return true;
//      };
//
//      /**
//       *
//       * @param key key that will get a bucket from the map
//       * @return the _size of the bucket that the key is / would belong to
//       */
//      int bucketSize(const unsigned long key) const
//      {
//          return _vCounter ? (int) _list.at(hashKey(key))._size() : 0;}
//
//      /**
//       * reset the hash map
//       */
//      void clear()
//      {
//          _list = dictionary();
//          _vCounter = 0;
//          _size = 0;
//      }
//
//      /**
//       * Insert a key and a value into the map
//       * @param key
//       * @param value
//       * @return  true if insreted
//       */
//      bool insert(const unsigned long &key, const unsigned int &value)
//      {
//          long hash =  hashKey(key);
//          long idx = getIdx(key,hash);
//          // replace value
//          if(idx != notFound)
//          {
//              _list.at(idx);
//              return true;
//          }
//          _vCounter++;
//          int check = sizeCheck();
//          //insert
//          std::pair<unsigned long, unsigned int> pair = {key, value};
//          bool  allGood = true;
//          switch (check)
//          {
//              case enlarge:
//                  allGood = reSize(enlarge);
//                  break;
//              case shrink:
//                  allGood = reSize(shrink);
//              default:
//                  break;
//          }
//          if(!allGood) { return false;}
//          _list.at(hash).push_back({key, value});
//          return true;
//      }
//
//    HashMap& operator=(const unsigned int &value)
//    {
//        for(std::pair<unsigned long, unsigned int> pair : *this)
//        {
//            pair.second = value;
//        }
//        return *this;
//    }
//
//    unsigned int operator[](const unsigned long &key)
//      {
//          long hash = hashKey(key);
//          long idx = getIdx(key, hash);
//          if( idx == notFound)
//          {
//              insert(key, 0);
//              //todo change the 0 to valueT
//              idx = getIdx(key, hash);
//
//          }
//          return _list[hash][idx].second;
//      }
//
//      bool operator==(const HashMap& other)
//      {
//          for(std::pair<unsigned long, unsigned int> pair : other)
//          {
//              long hash = hashKey(pair.first);
//              long idx = getIdx(pair.first, hash);
//              if(idx == notFound)
//              {
//                  return false;
//              }
//          }
//          return true;
//      }
//
//      bool operator!=(const HashMap &other){ return !(*this == other);}
//
//    class MapIterator
//    {
//
//
//        typedef std::forward_iterator_tag category;
//        //__gnu_cxx::__normal_iterator<std::pair<unsigned long, unsigned int> *, std::vector<std::pair<unsigned long, unsigned int>>>
//
//        outerIter _curOut;
//        const outerIter _outEnd;
//        innerIter _curIn;
//        innerIter _bucketEnd;
//
//
//    public:
//        /**
//         * Default Ctor
//         */
//        MapIterator() : _curOut(outerVec::iterator()),
//                        _outEnd(_curOut),
//                        _curIn(innerVec::iterator()),
//                        _bucketEnd(_curIn)
//        {} ;
//
//        explicit MapIterator(const HashMap& map): _curOut(map._list.begin()),
//                                                                        _outEnd(map._list.end()),
//                                                                        _curIn(_curOut.base()->begin()),
//                                                                        _bucketEnd(_curOut.base()->end())
//        {} ;
//
//
//        bool operator==(const MapIterator other)
//        { return other._curIn == _curIn; }
//
//        bool operator!=(const MapIterator other)
//        { return !(*this == other); }
//
//        const std::pair<unsigned long, unsigned int> &operator*()
//        { return *_curIn.base(); }
//
//        const MapIterator &operator++()
//        {
//            if (++_curIn == _bucketEnd)
//            {
//                _curIn = ++_curOut == _outEnd ? _curIn : _curOut.base()->begin();
//                _bucketEnd = _curOut.base()->end();
//            }
//            return *this;
//        }
//        const innerIter* operator->(){ return &_curIn;}
//
//    };
//
//      MapIterator end() const
//      {
//
//          return {};
//      }
//
//    MapIterator begin() const
//    {
//        return MapIterator(*this);
//
//    }
//
//private:
//    /**
//     * The _size factor of the list (capacity)
//     */
//    size_t _size;
//
//    /**
//     * The lower bound of the function capacity
//     */
//    double _lowerBound;
//
//    /**
//     * Counts how many object the list have
//     */
//    size_t _vCounter;
//
//    /**
//     * Array which cointains the buckets
//     */
//    dictionary _list;
//
//    /**
//     * returns the hash value of a given key
//     */
//    long hashKey(unsigned long key) const {
//        long hash;
//        hash = std::hash<unsigned long>{}(key);
//        return  hash &  ((long )mapSize(_size) - 1);}
//
//    /**
//     * checks if the map need to be resized
//     */
//    int sizeCheck()
//    {
//
//        double curCap = _vCounter / mapSize(_size);
//        if(curCap > upperBound) {return enlarge; }
//        if(curCap < lowestBound) { return shrink;}
//        return 0;
//    }
//
//    /**
//     * resize the hash map
//     * return true if went well.
//     */
//     bool reSize(sizeFactors factor)
//    {
//         std::vector<innerVec>  tmp;
//         _size = factor == enlarge ? ++_size : --_size;
//         for(std::pair<unsigned long, unsigned int> i : *this)
//         {
//             tmp.at(hashKey(i.first)).push_back(i);
//         }
//         _list.clear();
//         _list = tmp;
//        return true;
//    }
//
//    /**
//     * returns the index of a given key
//     * @param key the key
//     * @param hash it's hashed value
//     * @return the idx of the item, -1 if wasnt found
//     */
//    long getIdx(const unsigned long key, const long hash) const
//    {
//        if(!_vCounter){ return notFound;}
//        innerVec list = _list.at(hash);
//        iterator item = std::find(list.begin(), list.end(), key);
//        if (item == list.end())
//        {
//            return notFound;
//        }
//        return std::distance(list.begin(), item);
//    }
//
//
//};
//
//
////class MapIterator
////    typedef std::vector<std::pair<unsigned long, std::string>> innerVec;
////    typedef __gnu_cxx::__normal_iterator<std::vector<std::pair<unsigned long, std::basic_string<char>>>
////    *, std::vector<std::vector<std::pair<unsigned long, std::basic_string<char>>>>> outerIter;
////    typedef __gnu_cxx::__normal_iterator<std::pair<unsigned long, std::string> *,
////    std::vector<std::pair<unsigned long, std::string>>> innerIter;
////
////     outerIter _outItr;
////     const outerIter _outEnd;
////     innerIter _bucketItr;
////     innerIter _bucketEnd;
////
////public:
////    /**
////     * Default Ctor
////     */
////    MapIterator(): _outItr(outerVec::iterator()), _outEnd(_outItr), _bucketItr(innerVec::iterator()),
////                  _bucketEnd(_bucketItr){} ;
////
////    MapIterator(HashMap<keyT, map)
////
////    bool operator==(const MapIterator other) {return other._bucketItr == _bucketItr;}
////    bool operator!=(const MapIterator other) { return !(*this == other);}
////    innerIter*operator*(){ return &_bucketItr;}
////
////    MapIterator&operator++()
////    {
////        if(++_bucketItr == _bucketEnd)
////        {
////            _bucketItr = ++_outItr == _outEnd ? _bucketItr : _outItr.base()->begin();
////            _bucketEnd = _outItr.base()->end();
////        }
////        return *this;
////    }
//
//
//
//
//
//
//////        typedef MapIterator HashMap;
////
////    /**
////     * The HashMap of the iterator
////     */
////    const HashMap *_map;
//////
////    /**
////     * The current object the iterator points to
////     */
////    std::list<std::pair<unsigned long, std::string>>* _cur;
////
////    /**
////     * to which cell of the list of the map the pointer now belongs
////     */
////    size_t _counter;
////public:
////    /**
////     * Ctor of the iterator
////     * @param map the map which the iterator iterate on
////     */
////    explicit MapIterator(const HashMap *map) : _map(map),_cur(_map->_list[0].begin()), _counter(0){};
////
////    /**
////     * Default Ctor
////     */
////    explicit MapIterator(std::pair<unsigned long, std::string>* cur = nullptr)
////            : _map(nullptr), _cur(cur), _counter(0){};
////
////    /**
////    *  the  * operator
////    * @return pair of the key and value
////    */
////    std::pair<unsigned long, std::string> operator*() const
////    { return *_cur; }
////
////    /**
////     * The ++ operator of the iterator
////     */
////    MapIterator &operator++()
////    {
////        _cur++;
////        if (_cur == _map->end())
////        {
////            _cur = mapSize(_counter) < mapSize(_map._size) ? _map._list[++_counter] : _cur;
////        }
////        return *this;
////    }
//////        const_iterator operator++ (int) {const_iterator tmp = *this; ++*this; return tmp; }
////
////    /**
////  * The ++ operator of the iterator
////  */
////    bool operator==(const MapIterator &other) const
////    {
////        return _cur == other._cur;
////    }
////
////    /**
////  * The != operator of the iterator
////  */
////    bool operator!=(const MapIterator &other) const
////    {
////        return _cur != other._cur;
////    }
////
////    /**
////    * The == operator of the iterator
////    */
////    friend bool operator==(MapIterator a, MapIterator b)
////    {
////        return a._cur == b._cur;
////    }
////
////    /**
////     *  == operator for keys ,to use find function
////     */
////    friend bool operator==(unsigned long a, MapIterator b)
////    {
////        return a == b._cur.first;
////    }
////
////    /**
////     *  == operator for keys ,to use find function
////     */
////    friend bool operator==(MapIterator a, unsigned long b)
////    {
////        return a._cur.first == b;
////    }
////
////    /**
////    * The == operator of the iterator
////    */
////    friend bool operator!=(MapIterator a, MapIterator b)
////    {
////        return a._cur != b._cur;
////    }
////
////    /**
////    * return the object that from him it is possible to iterate the whole map
////    */
////    MapIterator begin() const { return {&_map->_list[0][0]}; }
////
////    MapIterator end() const {
////        long idx = mapSize(_map->_size);
////        return &_map->_list[idx]->end(); }
////
//////        const_iterator begin() const
//////        { return const_iterator(this); }
//////
//////        const_iterator end() const
//////        { return const_iterator(); }
////};
//
//
////    friend
////    bool operator== (const_iterator a, iterator b) {
////        return a._cur == b._cur;
////    }
////    friend
////    bool operator!= (const_iterator a, iterator b) {
////        return a._cur != b._cur;
////    }
//
//
//#endif //UNTITLED3_HASHMAP_HPP
