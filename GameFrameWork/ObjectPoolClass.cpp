#include "ObjectPoolClass.h"
#include "Actor.h"

ObjectPoolClass::ObjectPoolClass() {

}

ObjectPoolClass::~ObjectPoolClass() {
	for (auto Object : m_Objects) {
		for (int i = 0; i < Object.second.size(); i++) {
			delete Object.second.top();
			Object.second.pop();
		}
	}
}

Actor* ObjectPoolClass::GetPoolObject(std::string ObjectName) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	if (It != m_Objects.cend()) {
		Actor* Object = It->second.top();
		It->second.pop();
		return (Object ? Object : nullptr);
	}
	return nullptr;
}

void ObjectPoolClass::Release(std::string ObjectName, Actor* Object) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	if (It != m_Objects.cend()) {

		It->second.push(Object);
	}
}