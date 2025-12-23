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
        
        static_assert(
            N > 0,
            "N must be greater than 0"
        );
        
        public:
        // ============================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
            Cache(void) {}
            ~Cache(void) {}
        
        public:
        // ============================================================================================
        // | ACCESSORS |
        // =============
        
            std::size_t getCapacity(void) const {
                return this->_storage.size();
            }
            
            std::size_t getLength(void) const {
                // accumulate #of valid entries
                std::size_t size = 0;
                for (const T& asset : this->_storage) {
                    if (asset.isActive()) {
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
                // find the active minimum-priority asset
                const T* front = nullptr;
                for (const T& asset : this->_storage) {
                    if (
                        front == nullptr
                    || (
                        asset.isActive()
                        &&
                        asset.getPriority() < front.getPriority()
                    )) {
                        front = &asset;
                    }
                }
                
                return front;
            }
                    
            T* getFront(void) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->getFront()
                );
            }
            
            const T* getBack(void) const {
                // find the active maximum-priority asset
                const T* back = nullptr;
                std::size_t maxPriority = 0;
                for (const T& asset : this->_storage) {
                    if (
                        back == nullptr
                    || (
                        asset.isActive()
                        &&
                        asset.getPriority() > back.getPriority()
                    )) {
                        back = &asset;
                    }
                }
                
                return back;
            }
            
            T* getBack(void) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->getBack()
                );
            }
            
            const T* find(unsigned long id) const {
                // find an active asset associated with the given id
                for (const T& asset : this->_storage) {
                    if (asset.isActive() && asset.getId() == id) {
                        return &asset;
                    }
                }
                
                // no matching id was found
                return nullptr;
            }
             
            T* find(unsigned long id) {
                return const_cast<T*>(
                    static_cast<const Cache*>(this)->find(id)
                );
            }
            
            bool contains(unsigned long id) const {
                return this->find(id) != nullptr;
            }
        
        public:
        // ============================================================================================
        // | MODIFIERS |
        // =============
        
            template <typename... Args>
            T* store(Args&&... args) {            
                // find an asset to be overwritten
                T* target = nullptr;
                for (T& asset : this->_storage) {
                    if (
                        target == nullptr
                    ||
                        asset.getPriority() < target->getPriority()
                    ) {
                        target = &asset;
                    }
                }
                
                // try to clean up the target asset (if it is active)
                if (target->isActive()) {
                    if (!target->save()) {
                        return nullptr;
                    }
                    target->deactivate();
                    target->~T();
                }
                
                // shift the queue
                for (T& asset : this->_storage) {
                    if (asset.isActive()) {
                        asset.shiftPriority(-1);
                    }
                }
                
                // insert the new asset
                new (target) T(std::forward<Args>(args)...);
                target->setPriority(this->getCapacity());
                
                return target;
            }
            
            T* select(unsigned long id) {
                // find an active asset associated with the given id
                T* target = this->find(id);
                if (target == nullptr) {
                    return nullptr;
                }
                
                // shift the queue
                std::size_t targetPriority = target->getPriority();
                for (T& asset : this->_storage) {
                    if (
                        asset.isActive() 
                    &&
                        asset.getPriority() > targetPriority
                    ) {
                        asset.shiftPriority(-1);
                    }
                }
                
                // renew the target's priority
                target->setPriority(this->getCapacity());
                
                return target;
            }
            
            bool remove(unsigned long id) {
                // find an active asset associated with the given id
                T* target = this->find(id);
                if (target == nullptr) {
                    return false;
                }
                
                // remember the target's priority
                std::size_t targetPriority = target->getPriority();
                
                // try to clean up the target asset
                if (!target->save()) {
                    return false;
                }
                target->deactivate();
                target->~T();
                
                // shift the queue
                for (T& asset : this->_storage) {
                    if (
                        asset.isActive() 
                    &&
                        asset.getPriority() > targetPriority
                    ) {
                        asset.shiftPriority(-1);
                    }
                }
                
                return true;
            }
    
        private:
        // ============================================================================================
        // | MEMBERS |
        // ===========
        
            std::array<T, N> _storage;  // asset storage
    };
}

