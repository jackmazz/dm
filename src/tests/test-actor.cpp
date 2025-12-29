#include "actor.hpp"
#include "tile.hpp"
#include "utils/schema.hpp"
#include "utils/testing.hpp"

#include <iostream>
#include <string>
    
// ================================================================================================
// | CONSTRUCTOR & DESTRUCTOR TESTS |
// ==================================

static void _testConstructors(void) {
    unsigned long id = 1;
    std::string filePath = "file-path-1";
    std::string name = "name-1";
    char marker = '1';
    
    // call each constructor
    dm::Actor actor;
    actor = dm::Actor(id, filePath);
    actor = dm::Actor(id, filePath, name, marker);
    
    std::cout << "test-constructors passed\n";
}

static void _testDestructors(void) {
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
    
    std::cout << "test-destructors passed\n";
}

// ================================================================================================
// | ACCESSOR TESTS |
// ==================

static void _testAccessors(void) {
    unsigned long id = 3;
    std::string filePath = "file-path-3";
    std::string name = "name-3";
    char marker = '3';

    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    // test getId()
    dm::assertEquals(
        "test-accessors-1",
        "getId() did not return the expected value",
        id, actor.getId()
    );
    
    // test getFilePath()
    dm::assertEquals(
        "test-accessors-2",
        "getFilePath() did not return the expected value",
        filePath, actor.getFilePath()
    );
    
    // test getName()
    dm::assertEquals(
        "test-accessors-3",
        "getName() did not return the expected value",
        name, actor.getName()
    );
    
    // test getMarker()
    dm::assertEquals(
        "test-accessors-4",
        "getMarker() did not return the expected value",
        marker, actor.getMarker()
    );
    
    // test getTile()
    dm::assertEquals<void*>(
        "test-accessors-5",
        "getTile() did not return the null pointer",
        nullptr, actor.getTile()
    );
    
    std::cout << "test-accessors passed\n";
}

// ================================================================================================
// | MODIFIER TESTS |
// ==================

static void _testModifiers(void) {
    unsigned long id = 4;
    std::string filePath = "file-path-4";
    std::string name = "name-4";
    char marker = '4';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    char newMarker = '@';
    dm::Tile newTile;
    
    // test setMarker()
    actor.setMarker(newMarker);
    dm::assertEquals(
        "test-modifiers-1",
        "setMarker() did not update the marker",
        newMarker, actor.getMarker()
    );
    actor.setMarker(marker);
    dm::assertEquals(
        "test-modifiers-2",
        "setMarker() did not update the marker",
        marker, actor.getMarker()
    );
    
    // test setTile()
    actor.setTile(&newTile);
    dm::assertEquals(
        "test-modifiers-3",
        "setTile() did not update the tile",
        &newTile, actor.getTile()
    );
    dm::assertEquals(
        "test-modifiers-4",
        "the actor's tile does point to the actor",
        &actor, actor.getTile()->getActor()
    );
    actor.setTile(nullptr);
    dm::assertEquals<void*>(
        "test-modifiers-5",
        "setTile() did not update the tile",
        nullptr, actor.getTile()
    );
    
    std::cout << "test-modifiers passed\n";
}

// ================================================================================================
// | CONVERTER TESTS |
// ===================

static void _testToString(void) {
    unsigned long id = 5;
    std::string filePath = "file-path-5";
    std::string name = "name-5";
    char marker = '5';
    
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
    unsigned long id = 6;
    std::string filePath = "file-path-6";
    std::string name = "name-6";
    char marker = '6';
    
    dm::Actor actor = dm::Actor(id, filePath, name, marker);
    
    dm::Schema schema = dm::Schema::decode(
        std::string() +
        "[PROPERTIES]\n" +
        "id = " + std::to_string(id) + "\n" +
        "name = " + name + "\n" +
        "marker = " + std::string(1, marker) + "\n"
    );
    
    // test toSchema
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
        _testConstructors();
        _testDestructors();
        _testAccessors();
        _testModifiers();
        _testToString();
        _testToSchema();
        _testLogistics();
    }
    
    else if (testName == "constructors") {
        _testConstructors();
    } 
    
    else if (testName == "destructors") {
        _testDestructors();
    } 
    
    else if (testName == "accessors") {
        _testAccessors();
    } 
    
    else if (testName == "modifiers") {
        _testModifiers();
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

