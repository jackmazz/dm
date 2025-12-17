#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace dm {
    // forward declarations
    template <typename T, std::size_t N> class Cache;
    
    // cache data type
    class CacheEntry {
        public:
            /* constructors & destructors */
                CacheEntry(unsigned long id);
                ~CacheEntry(void);
            
             /* accessors */
                unsigned long id(void);
                std::size_t priority(void);
        
        private:
            template <typename T, std::size_t N> 
            friend class Cache;
            
            unsigned long _id;      // unique id
            std::size_t _priority;  // #of stores until this entry is removed
    };
    
    template <typename T, std::size_t N>
    class Cache {
        public:

        // ============================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================

            Cache(void) {}
            ~Cache(void) {}

        // ============================================================================================
        // | ACCESSORS |
        // =============
            
            std::size_t capacity(void)  {
                return this->_storage.size();
            }
            
            std::size_t size(void)  {
                // accumulate #of valid entries
                std::size_t size = 0;
                for (T& entry : this->_storage) {
                    if (entry.priority() > 0) {
                        size++;
                    }
                }
                
                return size;
            }
            
            bool isEmpty(void) {
                return this->size() == 0;
            }
            
            bool isFull(void) {
                return this->size() == this->capacity();
            }
                    
            T* front(void) {
                // find the index of the minimum-priority entry
                std::size_t index;
                std::size_t minPriority = this->capacity();
                for (std::size_t i = 0; i < this->capacity(); i++) {
                    T& entry = this->_storage[i];
                    if (entry.priority() < minPriority) {
                        minPriority = entry.priority();
                        index = i;
                    }
                }
                
                // return the minimum-priority entry
                return &this->_storage[index];
            }
            
            T* back(void) {
                // find the index of the maximum-priority entry
                std::size_t index;
                std::size_t maxPriority = 0;
                for (std::size_t i = 0; i < this->capacity(); i++) {
                    T& entry = this->_storage[i];
                    if (entry.priority() > maxPriority) {
                        maxPriority = entry.priority();
                        index = i;
                    }
                }
                
                // return the maximum-priority entry
                return &this->_storage[index];
            }
                    
            T* select(unsigned long id) {
                // find an element that was assigned the input id
                for (T& entry : this->_storage) {
                    if (entry.id() == id) {
                        return &entry;
                    }
                }
                
                // no matching id was found
                return nullptr;
            }
            
            bool contains(unsigned long id) {
                return this->select(id) != nullptr;
            }

        // ============================================================================================
        // | MODIFIERS |
        // =============

            template <typename... Args>
            T* store(Args&&... args) {
                // find the minimum-priority entry
                T* front = this->front();
                
                // decrement the priorities of all valid entries
                for (T& entry : this->_storage) {
                    if (entry.priority() > 0) {
                        entry._priority--;
                    }
                }
                
                // destroy the minimum-priority entry
                front->~T();
                
                // insert the new element
                new (front) T(std::forward<Args>(args)...);
                front->_priority = this->capacity();
                
                return front;
            }
                    
        private:
    
        // ============================================================================================
        // | MEMBERS |
        // ===========
        
            std::array<T, N> _storage;  // cache element storage
    };
}

