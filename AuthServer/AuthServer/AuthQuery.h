#pragma once
#include <string>

struct AuthQuerySet
{
private: 
	AuthQuerySet() = default;
	~AuthQuerySet() = default;
	AuthQuerySet(const AuthQuerySet& other) = delete;
	AuthQuerySet& operator=(const AuthQuerySet& other) = delete;

public:
	static AuthQuerySet& GetInst()
	{
		static AuthQuerySet instance;
		return instance;
	}

#pragma region Query
public:
	///////////////////////////////
	// ExampleSPName
	// input :
	//			(type) first parameter
	//			(type) second parameter
	// output :
	//			void
	///////////////////////////////
	//const std::wstring exampleQuery = "{CALL ExampleSPName(?, ?)}";
#pragma endregion Query
};