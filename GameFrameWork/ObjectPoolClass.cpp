#include "ObjectPoolClass.h"
#include "Actor.h"

ObjectPoolClass::ObjectPoolClass(const LPDIRECT3DDEVICE9 Device) : m_Device(Device) {

}

ObjectPoolClass::~ObjectPoolClass() {
	for (auto Object : m_Objects) {
		size_t ObjectSize = Object.second.size();
		for (size_t i = 0; i < ObjectSize; i++) {
			if (Object.second.top()) {
				delete Object.second.top();
				Object.second.pop();
			}
		}
	}
}

void ObjectPoolClass::Release(std::string ObjectName, Actor* Object) {
	auto It = std::find_if(m_Objects.begin(), m_Objects.end(), [&ObjectName](const std::pair<std::string, std::stack<Actor*>>& Iterator) -> bool { if (Iterator.first.compare(ObjectName) == 0) { return true; } return false; });
	if (It != m_Objects.cend()) {
		It->second.push(Object);
	}
}