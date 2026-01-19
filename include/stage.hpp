#pragma once

#include "actor.hpp"
#include "asset.hpp"
#include "config.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "utils/cache.hpp"
#include "utils/form.hpp"

#include <cstddef>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define DM_STAGE_TYPE_ID 0xA0000000
#define DM_STAGE_TYPE_NAME "stage"

namespace dm {
    class Stage : public Asset {
    
        // ========================================================================================
        // | TYPES |
        // =========
        
        public:
            using Link = std::pair<
                Asset::Contact, Tile::Position
            >;
        
        private:
            using _LinkMapping = std::pair<
                unsigned long, Stage::Link
            >;
    
        // ========================================================================================
        // | CONSTRUCTORS & DESTRUCTORS |
        // ==============================
        
        public:
            Stage(void);

            Stage(
                unsigned long primeId, 
                const std::string& filePath,
                std::size_t rowCount,
                std::size_t columnCount
            );

            Stage(
                unsigned long primeId, 
                const std::string& filePath,
                const std::string& name, 
                std::size_t rowCount,
                std::size_t columnCount, 
                const std::string& markers,
                const std::string& modifiers
            );
            
            ~Stage(void) = default;
        
        // ========================================================================================
        // | ACCESSORS |
        // =============
        
        public:
            unsigned long getTypeId(void) const override;
            std::string getTypeName(void) const override;
            
            static bool checkType(const Asset* asset);
            static bool checkType(const Asset::Contact& contact);
            
            static const Stage* cast(const Asset* asset);
            static Stage* cast(Asset* asset);
        
            std::size_t getSize(void) const;
            std::size_t getRowCount(void) const;
            std::size_t getColumnCount(void) const;
            
            bool isInBounds(Tile::Position position) const;
            bool isInBounds(std::size_t row, std::size_t column) const;
            
            const Tile* getTile(const Tile::Position& positon) const;
            const Tile* getTile(std::size_t row, std::size_t column) const;
            Tile* getTile(const Tile::Position& positon);
            Tile* getTile(std::size_t row, std::size_t column);

            std::vector<const Actor*> getActors(void) const;
            std::vector<Actor*> getActors(void);

            std::set<Asset::Contact> getContacts(void) const;
            
            const Tile* getLinkedTile(const Asset::Contact& contact) const;
            Tile* getLinkedTile(const Asset::Contact& contact);
            
            bool isLinked(const Unit* unit) const;
            bool isLinked(const Asset::Contact& contact) const;
        
        // ========================================================================================
        // | MODIFIERS |
        // =============
        
        public:
            void link(const Unit* unit);
            void link(
                const Asset::Contact& contact,
                const Tile::Position& position
            );
            
            void unlink(const Unit* unit);
            void unlink(const Asset::Contact& contact);
            
            void unlinkAll(void);
        
        // ========================================================================================
        // | CONVERTERS |
        // ==============
        
        public:
            std::string toString(void) const override;
            Form toForm(void) const override;
        
        // ========================================================================================
        // | LOGISTICS |
        // =============
        
        public:
            static Stage* fetch(unsigned long primeId);
            static bool isLoaded(unsigned long primeId);

            static Stage* load(const std::string& filePath);
            static Stage* select(unsigned long primeId);
            static bool unload(unsigned long primeId);
        
        // ========================================================================================
        // | UTILITIES |
        // =============
        
        private:
            std::size_t _linearIndex(
                std::size_t row, 
                std::size_t column
            ) const;
            
            bool _compareContacts(
                const Actor::Contact& contactA,
                const Actor::Contact& contactB
            );
        
        // ========================================================================================
        // | MEMBERS |
        // ===========
        
        private:
            static Cache< // stage storage
                Stage, DM_STAGE_CACHE_CAP
            > _cache;

            std::size_t _rowCount; // grid #of rows
            std::size_t _columnCount; // grid #of columns

            std::vector<Tile> _tiles; // tile storage

            std::map< // unit linkage
                unsigned long, Stage::Link
            > _links;
    };
}

