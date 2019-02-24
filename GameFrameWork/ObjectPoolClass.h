#pragma once
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <algorithm>

class ObjectPoolClass {
private:
	std::vector<std::pair<std::string, std::stack<class Actor*>>> m_Objects;

public:
	ObjectPoolClass();
	~ObjectPoolClass();
	
	template<typename T>
	void CreateObject(std::string ObjectName, size_t ObjectCount);

	class Actor* GetPoolObject(std::string ObjectName);
	void Release(std::string ObjectName, class Actor* Object);
};

template<typename T>
inline void ObjectPoolClass::CreateObject(std::string ObjectName, size_t ObjectCount) {
	std::stack<class Actor*> Objects;
	for (int i = 0; i < ObjectCount; i++) {
		class Actor* Object = new T;
		
		Objects.push(Object);
	}
	m_Objects.push_back(std::make_pair(ObjectName, Objects));
}