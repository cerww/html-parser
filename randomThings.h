#pragma once
#include <vector>
#include <deque>
#include <stack>
#include <list>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <utility>
#include <string>
#include <type_traits>
#include <fstream>
#include <array>
#include <forward_list>
#include <queue>
#include <thread>
#include <ppl.h>
#include <ppltasks.h>
#include <ppltaskscheduler.h>
//that might be usefull and not in the std already
using lock_t = std::unique_lock<std::mutex>;
template<typename T>
decltype(auto) derefCompletely(T& thing) {
	if constexpr(std::is_pointer_v<T>)
		return derefCompletely(*thing);
	else
		return thing;
}

template<typename fn>
struct Not{
	Not(fn t_f) : m_function(std::move(t_f)) {}
	template<typename ... Args>
	decltype(auto) operator()(Args... args) {
		return !m_function(std::forward<Args>(args)...);
	}
private:
	fn m_function;
};//callable that is returns !fn(args...) :D

template<typename fn>
inline auto not(fn func) {
	return Not<fn>(std::forward<fn>(func));
}

inline std::vector<int> Range(const int n) {
	std::vector<int> retVal(n);
	std::generate(retVal.begin(), retVal.end(), [awesome = 0]()mutable {return awesome++; });
	return retVal;
}

inline std::vector<int> Range(const int n1,const int n2) {
	std::vector<int> retVal(n2 - n1);
	std::generate(retVal.begin(), retVal.end(), [awesome = n1]()mutable {return awesome++; });
	return retVal;
}

template<int size>
inline constexpr std::array<int, size> RangeArray(const int start) {
	std::array<int, size> retVal = {};
	std::generate(retVal.begin(), retVal.end(), [abc = start]()mutable {return abc++; });
	return retVal;
}

//I found this online
inline std::string getFileContents(const std::string& filePath) {
	std::string fileContents;
	std::ifstream file(filePath, std::ios::in);
	file.seekg(0, std::ios::end);
	int filesize = (int)file.tellg();
	file.seekg(0, std::ios::beg);
	filesize -= (int)file.tellg();
	fileContents.resize(filesize);
	file.read(&fileContents[0], filesize);
	file.close();
	return std::move(fileContents);
}

template<typename T>
struct removePtrt {
	using val = T;
};

template<typename T>
struct removePtrt<T*> {
	using val = T;
};

inline constexpr int roundToNearest(const int num, const int multiply) {
	return (num / multiply)*multiply + multiply * !!(num%multiply);
}

template<int multiply>
inline constexpr int roundToNearestT(const int num) {
	return (num / multiply)*multiply + multiply * !!(num%multiply);
}

template<typename T>
struct getDimensions {
	static constexpr size_t value = 0;
};
template<typename T,size_t n>
struct getDimensions<std::array<T, n>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};
template<typename T>
struct getDimensions<std::vector<T>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};
template<typename T>
struct getDimensions<std::stack<T>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};
template<typename T,typename U>
struct getDimensions<std::unordered_map<T,U>> {
	static constexpr size_t value = getDimensions<U>::value + 1;
};
template<typename T,typename U>
struct getDimensions<std::map<T,U>> {
	static constexpr size_t value = getDimensions<U>::value + 1;
};
template<typename T>
struct getDimensions<std::list<T>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};
template<typename T>
struct getDimensions<std::forward_list<T>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};
template<typename T>
struct getDimensions<std::deque<T>> {
	static constexpr size_t value = getDimensions<T>::value + 1;
};

template<typename T1, typename T2, size_t size>
inline std::array<T1,size> array_cast(std::array<T2,size> oldArray){
	std::array<T1, size> retVal = {};
	for(size_t i = 0;i<size;++i){
		retVal[i] = static_cast<T1>(oldArray[i]);
	}return retVal;
}

template<typename T1, typename T2, size_t size>
constexpr inline std::array<T1, size> array_cast2(std::array<T2, size> oldArray) {
	return array_cast_impl<T1>(oldArray, std::make_index_sequence<size>);
}
template<typename T1, typename T2, size_t size, size_t... i>
constexpr inline std::array<T1, size> array_cast_impl(std::array<T2, size> oldArray, std::index_sequence<i...>) {
	return { static_cast<T1>(oldArray[i])... };
}

inline bool isLetter(char let) {
	return let >= 'a' && let <= 'z' || let >= 'A' && let <= 'Z'; 
}

inline std::vector<std::string> split(const std::string& string,char letter) {
	std::vector<std::string> retVal;
	//retVal.reserve(3);
	size_t currentA = 0;
	do {
		size_t spot = string.find(letter, currentA);
		if(spot!=currentA)
			retVal.push_back(string.substr(currentA, spot - currentA));
		currentA = spot + 1;
	}while(currentA);//std::string::npos + 1 = 0
	return retVal;
}

template<typename T>
inline std::vector<std::vector<T>> split(std::vector<T> stuff,T thing){
	std::vector<std::vector<T>> retVal;
	std::vector<T> current;
	for (int i = 0; i < stuff.size(); ++i) {
		if(stuff[i]==thing){
			retVal.push_back(std::move(current));
			current = {};
		}else{
			current.push_back(std::move(stuff[i]));
		}
	}return retVal;
}

inline bool isNumber(char let){
	return let >= '0'&&let <= '9';
}

constexpr size_t out_of_range = -1;
template<typename string, typename... pred>
inline size_t find_first_if_not(const string& str, size_t off, pred... fn) {
	for (; off < str.size(); ++off){
		bool t = false;
		(void)(std::initializer_list<int>{(t |= fn(str[off]),0)...});
		//if (!(fn(str[off])||...)){}
		if(!t)
			return off;
	}return -1;
}

template<typename string, typename... pred>
inline size_t find_first_if(const string& str, size_t off, pred... fn) {
	for (; off < str.size(); ++off) {
		bool t = false;
		(void)(std::initializer_list<int>{(t |= fn(str[off]), 0)...});
		//if ((fn(str[off])||...))
		if (t)
			return off;
	}return -1;
}


template<typename... fns>
struct fnOr {
	constexpr fnOr(fns... functions) :m_fns(std::make_tuple(std::forward<fns>(functions)...)) {}
	template<typename... Args>
	decltype(auto) operator()(Args... args){
		return operator_impl(std::forward_as_tuple(std::forward<Args>(args)...), std::index_sequence_for<fns...>()); 
	}
private:
	std::tuple<fns...> m_fns;
	template<typename tuple,size_t... i>
	decltype(auto) operator_impl(tuple args,std::index_sequence<i...>){
		bool t = false;
		(void)(std::initializer_list<int>{(t = t || std::apply(std::get<i>(m_fns), args), 0)...});
		return t;
	}
};

template<typename... fns>
inline constexpr auto make_fnOr(fns... fn){
	return fnOr<fns...>(std::forward<fns>(fn)...);
}

template<typename T,typename adjFn, typename predFn>
inline T breadthFirstSearch(T start,adjFn adj, predFn pred){
	std::vector<T> queue(1, start);
	while (queue.size()){
		std::vector<T> next;
		for(T& node:queue){
			if(pred(node))
				return node;
			for(T& newNode:adj(node))
				next.push_back(newNode);			
		}queue = std::move(next);
	}return {};
}

template<typename T,typename adjFn,typename predFn>
inline std::pair<T,bool> depthFirstSearch(T start,adjFn adj,predFn pred){
	if (pred(start))
		return { start,true };
	for(T&& next:adj(start)){
		const auto[node, success] = depthFirstSearch(next, adj, pred);
		if(success){
			return { node,true };
		}
	}return { start,false };
}

template<typename T, typename adjFn, typename predFn>
inline std::vector<T> breadthFirstSearchMultiple(T start, adjFn adj, predFn pred) {
	std::deque<T> queue(1, start);
	std::vector<T> retVal;
	while (queue.size()) {
		std::deque<T> next;
		for (T& node : queue) {
			if (pred(node))
				retVal.push_back(node);
			for (T& newNode : adj(node))
				next.push_back(newNode);
		}queue = std::move(next);
	}return retVal;
}

template<typename T, typename adjFn, typename predFn>
inline T breadthFirstSearchCyclic(T start, adjFn adj, predFn pred) {
	std::vector<T> queue(1, start);
	std::unordered_map<T, int> visited;
	int lvl = 0;
	while (queue.size()) {
		std::vector<T> next;
		for (T& node : queue) {
			if (pred(node))
				return node;
			for (T& newNode : adj(node))
				if(visited.find(newNode)==visited.end())
					next.push_back(newNode);
			visited[node] = lvl;
		}queue = std::move(next);
		++lvl;
	}return {};
}

template<typename T, typename adjFn, typename predFn>
inline std::vector<T> BFSM_P(T start, adjFn adj, predFn pred) {
	std::mutex lockThingy;
	std::vector<T> retVal;
	std::deque<T> queue(1, start);
	std::atomic<bool> done = false;
	std::vector<char> not_running_things;//vector<bool> is bad
	auto fn = [&](const int i) {
		while(!done){
			if(queue.size()){
				T current;
				{	std::unique_lock<std::mutex> lock{ lockThingy };
					current = std::move(queue.front());
					queue.pop_front();
				}not_running_things[i] = 1;
				if(pred(current)){
					std::unique_lock<std::mutex> lock{ lockThingy };
					retVal.push_back(std::move(current));
				}else{
					std::unique_lock<std::mutex> lock{ lockThingy };
					for(auto&& next:adj(current))
						queue.push_back(std::move(next));					
				}
			}else{
				not_running_things[i] = 0;
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
		}
	};
	while (!done) {
		
	}return retVal;
}

template<typename T,typename adjFn,typename predFn,int threads = 4>
class BFS_PAR {
public:
	BFS_PAR(T start,adjFn t_adj,predFn t_pred):
		m_queue(1,std::move(start)),
		adj(std::move(t_adj)),
		pred(std::move(t_pred)){};
	std::vector<T> doStuffs(){
		for(int i = 0;i<threads;++i){
			//m_threads[i] = concurrency::create_task<void>([&,l = i, this]() {while (!m_done) runThing(i); });
			m_threads[i] = std::thread([&, this](const int id) {while (!m_done) runThing(id); }, i);
		}while (!m_done) {
			runThing(threads);
			checkIfDone();
		}return retVal;
	}
	~BFS_PAR(){
		for(auto& t:m_threads)
			t.join();		
	}
private:
	void runThing(const int id){
		if (m_queue.size()) {
			T current;
			{	lock_t lock{ m_lock };
				current = std::move(m_queue.front());
				m_queue.pop_front();
			}m_notRunningThreads[id] = 1;
			if (pred(current)) {
				lock_t lock{ m_lock };
				retVal.push_back(std::move(current));
			}
			else {
				lock_t lock{ m_lock };
				for (auto&& next : adj(current))
					m_queue.push_back(std::move(next));
			}
		}
		else {
			m_notRunningThreads[id] = 0;
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
	}
	void checkIfDone(){
		for(int i = 0;i<threads+1;++i)
			if (m_notRunningThreads[i])
				return;//not done
		m_done = true;
	}
	std::mutex m_lock;
	std::deque<T> m_queue;
	adjFn adj;
	predFn pred;
	std::atomic<bool> m_done = false;
	std::array<bool, threads + 1> m_notRunningThreads = {};//+1 for main thread
	//std::array<concurrency::task<void>, threads> m_threads;
	std::array<std::thread, threads> m_threads;
	std::vector<T> retVal;
};

template<typename T,typename adjFn,typename predFn>
std::vector<T> BFS_Parallel(T start,adjFn adj, predFn pred){
	BFS_PAR<T, adjFn, predFn> thingy(start, adj, pred);
	return thingy.doStuffs();
}
/*
	auto fn = [&this](const int id){
		while(!m_done){
			runThing(id);
		}
	}
*/