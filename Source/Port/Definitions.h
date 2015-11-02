//
// Created by LE, Duc Anh on 11/2/15.
//

#ifndef CLDEPLUS_CLDE_DEFAULT_H
#define CLDEPLUS_CLDE_DEFAULT_H

/*
 * The idea of of using clde_default is to encapsulate uses of STL components.
 * These components usually include string, vector, memory, exception, unordered_map, list, set.
 * All STL uses are declared & defined here so that they can be easily cross-platform maintained.
 *
 */

#include <memory>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <list>
#include <iterator>
#include <functional>

namespace CLDEPlus {

    using exception = std::exception;
    using string = std::string;

    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    template<typename T>
    using unique_ptr = std::unique_ptr<T>;

    template<typename T>
    using vector = std::vector<T>;

    template<typename T>
    using set = std::set<T>;

    template<typename T>
    using list = std::list<T>;

    template<typename TKey, typename TElem>
    using unordered_map = std::unordered_map<TKey, TElem>;

    template<typename TKey, typename TElem>
    using pair = std::pair<TKey, TElem>;

    template<typename T, typename... Args>
    auto cldeplus_make_shared(Args &&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...)) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    unique_ptr<T> cldeplus_make_unique(Args &&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}


#endif //CLDEPLUS_CLDE_DEFAULT_H