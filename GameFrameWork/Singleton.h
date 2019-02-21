#pragma once

template<class T>
class Singleton {
public:
	static T* m_Application;
public:
	static inline T* GetInst() { if (!m_Application) { m_Application = new T; } return m_Application; }
	static inline void DestroySingleton() { 
		if (m_Application) {
			delete m_Application;
			m_Application = nullptr;
		}
	}
};