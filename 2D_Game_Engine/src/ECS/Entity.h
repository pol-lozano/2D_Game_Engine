#pragma once
#include "ECS.h"
#include <vector>
#include <memory>
#include "Component.h"
#include "Components/Transform.h"

class Entity {
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;

public:
	Entity(){
		//All entities have a transform just like in unity!
		this->addComponent<Transform>(0, 0);
	}

	virtual ~Entity() = default;

	template <typename T, typename... TArgs>
	inline T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component>uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitset[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> 
	inline T& getComponent() const {
		//Set pointer to position in pointer array
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*> (ptr);
	}

	//Check if entity has a component attached
	template <typename T>
	inline bool hasComponent() const {
		return componentBitset[getComponentTypeID<T>()];
	}

	inline bool isActive() { return active; }

	//Remove entity
	inline void destroy() { active = false; }

	inline void update(float dt) {
		//Update all components
		for (auto& c : components) c->update(dt);
	}

	inline void draw() {
		//Draw all components that need to be drawn
		for (auto& c : components) c->draw();
	}
};
