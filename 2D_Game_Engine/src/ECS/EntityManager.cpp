#include "EntityManager.h"

	void EntityManager::update(float dt){
		//Update all entities
		for (auto& e : entities) e->update(dt);
	}

void EntityManager::draw(){
	//Update all entities
	for (auto& e : entities) e->draw();
}

void EntityManager::refresh(){
	//Remove unactive entities
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
				return !mEntity->isActive();
		}),
		std::end(entities));
}

void EntityManager::addEntity(Entity* e){
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));
}
