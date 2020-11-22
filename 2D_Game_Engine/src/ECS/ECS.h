#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

//Component id
using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() 
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept 
{
	//Generate new unique id
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

//Maximum ammount of components an entity can hold
constexpr std::size_t maxComponents = 32;

//To check what components entity has
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	//Components owner
	Entity* entity;

	virtual void init(){}
	virtual void update(){}
	virtual void draw(){}

	virtual ~Component(){}
};

class Entity {
private: 
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void update() 
	{
		//Update all components
		for (auto& c : components) c->update();
	}
	void draw() 
	{
		//Draw all components
		for (auto& c : components) c->draw();
	}
	bool isActive() { return active; }
	//Remove entity
	void destroy() { active = false; }

	//Check if entity has a component attached
	template <typename T> bool hasComponent() const 
	{
		return componentBitSet[getComponentTypeID<T>];
	}

	template <typename T,typename... TArgs>
	T& addComponent(TArgs&&... mArgs) 
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const 
	{
		//Set pointer to position in pointer array
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*> (ptr);
	}
};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
public:
	void update() 
	{
		//Update all entities
		for (auto& e : entities) e->update();
	}

	void draw()
	{
		//Update all entities
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		//Remove unactive entities
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));		
	}

	Entity& addEntity()
	{
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};