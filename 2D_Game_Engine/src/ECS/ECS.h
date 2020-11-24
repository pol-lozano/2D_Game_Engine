#pragma once
#include <iostream>
#include <bitset>
#include <array>

class Component;
class Entity;
class EntityManager;

//Component id
using ComponentTypeID = std::size_t;

inline ComponentTypeID getUniqueComponentTypeID(){
	static ComponentTypeID lastID = 0u;
	return lastID++;
}

template <typename T> 
inline ComponentTypeID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value, "Type not base on component!");
	//Generate new unique id
	static ComponentTypeID typeID = getUniqueComponentTypeID();
	return typeID;
}

//Maximum ammount of components an entity can hold
constexpr std::size_t MAX_ENTITIES = 1000;
constexpr std::size_t MAX_COMPONENTS = 32;

//To check what components entity has
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentArray = std::array<Component*, MAX_COMPONENTS>;