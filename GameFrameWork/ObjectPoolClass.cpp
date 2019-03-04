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
	m_ObjectNames.clear();
}