#pragma once

template<typename T>
class Queue {
private:
	const int Size;
	T* m_List;

private:
	short Read, Write, Counting;

public:
	Queue(const size_t);
	~Queue();

public:
	void Push(T&&);
	T Pop();
	bool IsEmpty();

};

template<typename T>
Queue<T>::Queue(const size_t MaxSize) : Size(MaxSize), Read(-1), Write(0), Counting(0) {
	m_List = new T[MaxSize];
}

template<typename T>
Queue<T>::~Queue() {
	if (m_List) {
		delete[] m_List;
		m_List = nullptr;
	}
}

template<typename T>
inline void Queue<T>::Push(T&& Data) {
	Counting++;
	m_List[Write < Size ? Write++ : Write = 0] = Data;
}

template<typename T>
inline T Queue<T>::Pop() {
	Counting--;
	return m_List[Read < Size ? ++Read : Read = -1];
}

template<typename T>
inline bool Queue<T>::IsEmpty() {
	return Counting <= 0 ? true : false;
}