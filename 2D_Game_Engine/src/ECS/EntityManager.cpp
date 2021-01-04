#include "EntityManager.h"

//Update all entities
void EntityManager::update(float dt) {
	for (auto& e : entities) e->update(dt);
}

//Draw all entities
void EntityManager::draw() {
	for (auto& e : entities) e->draw();
}

//Remove unactive entities
void EntityManager::refresh() {
	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
				return !mEntity->isActive();
		}),
		std::end(entities));
}

//Add new entity to manager
void EntityManager::addEntity(Entity* e) {
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));
}
