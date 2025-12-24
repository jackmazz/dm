#pragma once

#include "actor.hpp"
#include "config.hpp"
#include "tile.hpp"
#include "utils/asset.hpp"
#include "utils/cache.hpp"
#include "utils/schema.hpp"

#include <cstddef>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace dm {
    class Stage : public Asset {
    
        public:
        // ============================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
            Stage(void);

            Stage(
                unsigned long id, 
                const std::string& filePath, 
                std::size_t rowCount,
                std::size_t columnCount
            );

            Stage(
                unsigned long id, 
                const std::string& filePath,
                const std::string& name, 
                std::size_t rowCount,
                std::size_t columnCount, 
                const std::string& markers,
                const std::string& modifiers
            );

            ~Stage(void) = default;

        public:
        // ============================================================================================
        // | ACCESSORS |
        // =============
        
            std::size_t getSize(void) const;
            std::size_t getRowCount(void) const;
            std::size_t getColumnCount(void) const;
            bool isInBounds(std::size_t row, std::size_t column) const;

            const Tile* getTile(std::size_t row, std::size_t column) const;
            Tile* getTile(std::size_t row, std::size_t column);

            std::vector<const Actor*> getActors(void) const;
            std::vector<Actor*> getActors(void);

            std::vector<Actor::Contact> getContacts(void) const;

        public:
        // ============================================================================================
        // | MODIFIERS |
        // =============
        
            void addContact(const Actor::Contact& contact);
            void addContact(unsigned long id, const std::string& filePath);

            void removeContact(const Actor::Contact& contact);
            void removeContact(unsigned long id, const std::string& filePath);

        public:
        // ============================================================================================
        // | CONVERTERS |
        // ==============
        
            std::string toString(void) const override;
            Schema toSchema(void) const override;

        public:
        // ============================================================================================
        // | LOGISTICS |
        // =============
        
            static Stage* get(unsigned long id);
            static bool contains(unsigned long id);

            static Stage* load(const std::string& filePath);
            static Stage* select(unsigned long id);
            static bool unload(unsigned long id);
        
        private:
        // ============================================================================================
        // | UTILITIES |
        // =============
        
            std::size_t _linearIndex(
                std::size_t row, 
                std::size_t column
            ) const;
        
        private:
        // ============================================================================================
        // | MEMBERS |
        // ===========
        
            static Cache<Stage, DM_STAGE_CACHE_CAP> _cache; // stage storage

            std::size_t _rowCount; // grid #of rows
            std::size_t _columnCount; // grid #of columns

            std::vector<Tile> _tiles; // tile storage

            std::set<Actor::Contact> _contacts; // actor information storage
    };
}

