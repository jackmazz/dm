#pragma once

#include "utils/asset.hpp"

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace dm {
    template <typename T, std::size_t N>
    class Cache {
        static_assert(
            std::is_base_of<Asset, T>::value, 
            "T must derive from Asset"
        );
    
        public:

        // ============================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================

            Cache(void) {}
            ~Cache(void) {}

        // ============================================================================================
        // | ACCESSORS |
        // =============
            
            std::size_t getCapacity(void) const {
                return this->_storage.size();
            }
            
            std::size_t getLength(void) const {
                // accumulate #of valid entries
                std::size_t size = 0;
                for (const T& entry : this->_storage) {
                    if (entry.getPriority() > 0) {
                        size++;
                    }
                }
                
                return size;
            }
            
            bool isEmpty(void) const {
                return this->getLength() == 0;
            }
            
            bool isFull(void) const {
                return this->getLength() == this->getCapacity();
            }
            
            const T* getFront(void) const {
                // find the index of the minimum-priority entry
                std::size_t index;
                std::size_t minPriority = this->getCapacity();
                for (std::size_t i = 0; i < this->getCapacity(); i++) {
                    std::size_t priority = this->_storage[i].getPriority();
                    if (priority < minPriority) {
                        minPriority = priority;
                        index = i;
                    }
                }
                
                // return the minimum-priority entry
                return &this->_storage[index];
            }
                    
            T* getFront(void) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->getFront()
                );
            }
            
            const T* getBack(void) const {
                // find the index of the maximum-priority entry
                std::size_t index;
                std::size_t maxPriority = 0;
                for (std::size_t i = 0; i < this->getCapacity(); i++) {
                    std::size_t priority = this->_storage[i].getPriority();
                    if (priority > maxPriority) {
                        maxPriority = priority;
                        index = i;
                    }
                }
                
                // return the maximum-priority entry
                return &this->_storage[index];
            }
            
            T* getBack(void) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->getBack()
                );
            }
            
            const T* select(unsigned long id) const {
                // find an element that was assigned the input id
                for (const T& entry : this->_storage) {
                    if (entry.getId() == id) {
                        return &entry;
                    }
                }
                
                // no matching id was found
                return nullptr;
            }
             
            T* select(unsigned long id) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->select(id)
                );
            }
            
            bool contains(unsigned long id) const {
                return this->select(id) != nullptr;
            }

        // ============================================================================================
        // | MODIFIERS |
        // =============

            template <typename... Args>
            T* store(Args&&... args) {
                // find the minimum-priority entry
                T* front = this->getFront();
                
                // decrement the priorities of all valid entries
                for (T& entry : this->_storage) {
                    std::size_t priority = entry.getPriority();
                    if (priority > 0) {
                        entry.setPriority(priority-1);
                    }
                }
                
                // destroy the minimum-priority entry
                front->~T();
                
                // insert the new element
                new (front) T(std::forward<Args>(args)...);
                front->setPriority(this->getCapacity());
                
                return front;
            }
                    
        private:
    
        // ============================================================================================
        // | MEMBERS |
        // ===========
        
            std::array<T, N> _storage;  // cache element storage
    };
}

