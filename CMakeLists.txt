cmake_minimum_required(VERSION 3.14)

project(PocketBase LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Network Core Quick REQUIRED)

qt6_add_library(PocketBase STATIC)
qt6_add_qml_module(PocketBase
    URI "PocketBase"
    VERSION 1.0
    QML_FILES Collection.qml
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/PocketBase
    SOURCES src/PocketBase.hpp
    SOURCES src/PocketBaseCollection.h src/PocketBaseCollection.cpp
    SOURCES src/PocketBaseCollectionPromise.h src/PocketBaseCollectionPromise.cpp
    SOURCES src/PocketBaseCollectionSchema.h src/PocketBaseCollectionSchema.cpp
    SOURCES src/PocketBaseCollectionField.h src/PocketBaseCollectionField.cpp
    SOURCES src/CollectionFieldOptions.h src/CollectionFieldOptions.cpp
    SOURCES src/CollectionTextFieldOptions.h src/CollectionTextFieldOptions.cpp
    SOURCES src/CollectionFileFieldOptions.h src/CollectionFileFieldOptions.cpp
    SOURCES src/PocketBaseClient.h src/PocketBaseClient.cpp
    SOURCES src/PocketRequest.h src/PocketRequest.cpp
    SOURCES src/PocketUtility.h src/PocketUtility.cpp
    SOURCES src/CollectionSubscriber.h src/CollectionSubscriber.cpp
    SOURCES src/PocketBaseServer.h src/PocketBaseServer.cpp
    SOURCES src/PocketBaseSettings.h src/PocketBaseSettings.cpp
    SOURCES src/pocketnetworkmanager.h src/pocketnetworkmanager.cpp
)

target_link_libraries(PocketBase
    PRIVATE Qt::Core Qt::Quick Qt::Network)

set_target_properties(PocketBase PROPERTIES
    PUBLIC_HEADER "src/PocketBase.hpp"
)

target_include_directories(PocketBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
