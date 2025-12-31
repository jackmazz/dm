#include "actor.hpp"
#include "stage.hpp"
#include "tile.hpp"
#include "utils/schema.hpp"
#include "utils/testing.hpp"

#include <iostream>
#include <string>
    
// ================================================================================================
// | CONSTRUCTOR & DESTRUCTOR TESTS |
// ==================================

static void _testBasicConstructors(void) {
    unsigned long id = 1;
    std::string filePath = "file-path-1";
    std::string name = "name-1";
    char marker = '1';
    
    // call each constructor
    dm::Actor actor;
    actor = dm::Actor(id, filePath);
    actor = dm::Actor(id, filePath, name, marker);
    
    std::cout << "test-basic-constructors passed\n";
}

static void _testBasicDestructors(void) {
    unsigned long id = 2;
    std::string filePath = "file-path-2";
    std::string name = "name-2";
    char marker = '2';
    
    // destroy a local object
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    actor.~Actor();
    
    // destroy a dynamically-allocated object
    dm::Actor* heapActor = new dm::Actor(id, filePath, name, marker);
    delete heapActor;
    
    std::cout << "test-basic-destructors passed\n";
}

// ================================================================================================
// | ACCESSOR TESTS |
// ==================

static void _testBasicAccessors(void) {
    unsigned long id = 3;
    std::string filePath = "file-path-3";
    std::string name = "name-3";
    char marker = '3';

    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    // test getId()
    dm::assertEquals(
        "test-basic-accessors-1",
        "getId() did not return the expected value",
        id, actor.getId()
    );
    
    // test getFilePath()
    dm::assertEquals(
        "test-basic-accessors-2",
        "getFilePath() did not return the expected value",
        filePath, actor.getFilePath()
    );
    
    // test getName()
    dm::assertEquals(
        "test-basic-accessors-3",
        "getName() did not return the expected value",
        name, actor.getName()
    );
    
    // test getMarker()
    dm::assertEquals(
        "test-basic-accessors-4",
        "getMarker() did not return the expected value",
        marker, actor.getMarker()
    );
    
    // test getTile()
    dm::assertEquals<void*>(
        "test-basic-accessors-5",
        "getTile() did not return the null pointer",
        nullptr, actor.getTile()
    );
    
    std::cout << "test-basic-accessors passed\n";
}

// ================================================================================================
// | MODIFIER TESTS |
// ==================

static void _testBasicModifiers(void) {
    unsigned long id = 4;
    std::string filePath = "file-path-4";
    std::string name = "name-4";
    char marker = '4';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    char newMarker = '@';
    
    // test setMarker()
    actor.setMarker(newMarker);
    dm::assertEquals(
        "test-basic-modifiers-1",
        "setMarker() did not update the marker",
        newMarker, actor.getMarker()
    );
    
    std::cout << "test-basic-modifiers passed\n";
}

static void _testSetTile() {
    unsigned long id = 5;
    std::string filePath = "file-path-5";
    std::string name = "name-5";
    char marker = '5';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);

    dm::Stage stageA;
    dm::Stage stageB;
    dm::Tile tileA(&stageA, 0, 0);
    dm::Tile tileB(&stageB, 0, 0);
    dm::Tile tileC(&stageA, 0, 0);
    
    auto subtest = [&actor](
        const std::string& testName, 
        dm::Tile* tile
    ) {
        dm::Tile* prevTile = actor.getTile();
    
        // test setTile()
        actor.setTile(tile);
        dm::assertEquals(
            testName+"A",
            "setTile() did not update the tile",
            tile, actor.getTile()
        );
        
        if (prevTile != nullptr) {
            if (prevTile != tile) {
                // test that the actor's previous tile isn't occupied 
                dm::assertTrue(
                    testName+"B",
                    "the actor's previous tile is still occupied",
                    !prevTile->isOccupied()
                );
            }
            
            if (tile == nullptr || prevTile->getParent() != tile->getParent()) {
                // test that the actor's previous stage doesn't have the actor's contact
                dm::assertTrue(
                    testName+"C",
                    "the actor's previous stage still has the actor's contact",
                    !prevTile->getParent()->hasContact(
                        actor.getContact()
                    )
                );
            }
        }
        
        if (tile != nullptr) {        
            // test that the tile points to the actor
            dm::assertEquals(
                testName+"D",
                "the actor's tile does point to the actor",
                &actor, actor.getTile()->getActor()
            );
            
            // test that the actor's stage has the actor's contact
            dm::assertTrue(
                testName+"E",
                "the actor's stage doesn't have the actor's contact",
                actor.getTile()->getParent()->hasContact(
                    actor.getContact()
                )
            );
        }
    };
    
    dm::setDebugModeEnabled(false);
    subtest("test-set-tile-1", nullptr); // from nothing to nothing
    subtest("test-set-tile-2", &tileA); // from nothing to a tile
    subtest("test-set-tile-3", &tileA); // move to the same tile
    dm::setDebugModeEnabled(true);
    subtest("test-set-tile-4", &tileB); // move to a tile on the same stage
    subtest("test-set-tile-5", &tileC); // move to a tile on a different stage
    subtest("test-set-tile-6", nullptr); // move from a tile to nothing
    
    std::cout << "test-set-tile passed\n";
}

// ================================================================================================
// | CONVERTER TESTS |
// ===================

static void _testToString(void) {
    unsigned long id = 6;
    std::string filePath = "file-path-6";
    std::string name = "name-6";
    char marker = '6';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    // test toString()
    dm::assertEquals(
        "test-to-string-1",
        "toString() did not return the expected string",
        std::string(1, marker), actor.toString()
    );
    
    std::cout << "test-to-string passed\n";
}

static void _testToSchema(void) {
    unsigned long id = 7;
    std::string filePath = "file-path-7";
    std::string name = "name-7";
    char marker = '7';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    dm::Schema schema = dm::Schema::decode(
        std::string() +
        "[PROPERTIES]\n" +
        "id = " + std::to_string(id) + "\n" +
        "name = " + name + "\n" +
        "marker = " + std::string(1, marker) + "\n"
    );
    
    // test toSchema()
    dm::assertEquals(
        "test-to-schema-1",
        "toSchema() did not return the expected schema",
        schema, actor.toSchema()
    );

    std::cout << "test-to-schema passed\n";
}

// ================================================================================================
// | LOGISTICS TESTS |
// ===================

static void _testLogistics(void) {
    std::cout << "test-logistics passed\n";
}

// ================================================================================================
// | MAIN |
// ========

int main(int argCount, char** args) {
    std::cout << "\nTesting actor.cpp\n";

    std::string testName = "all";
    if (argCount > 1) {
        testName = args[1];
    }

    if (testName == "all") {
        _testBasicConstructors();
        _testBasicDestructors();
        _testBasicAccessors();
        _testBasicModifiers();
        _testSetTile();
        _testToString();
        _testToSchema();
        _testLogistics();
    }
    
    else if (testName == "basic-constructors") {
        _testBasicConstructors();
    } 
    
    else if (testName == "basic-destructors") {
        _testBasicDestructors();
    } 
    
    else if (testName == "basic-accessors") {
        _testBasicAccessors();
    } 
    
    else if (testName == "basic-modifiers") {
        _testBasicModifiers();
    }
    
    else if (testName == "set-tile") {
        _testSetTile();
    }
    
    else if (testName == "to-string") {
        _testToString();
    }
    
    else if (testName == "to-schema") {
        _testToSchema();
    }
    
    else if (testName == "logistics") {
        _testLogistics();
    }
    
    std::cout << "All tests passed\n";
    return 0;
}

