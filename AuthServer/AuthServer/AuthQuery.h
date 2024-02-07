#pragma once
#include <string>

struct QueryObject
{
	QueryObject() = delete;
	QueryObject(const std::wstring_view queryString, bool isNeedResult)
		: query(queryString), needResult(isNeedResult)
	{}

	const std::wstring query;
	bool needResult;
};

struct AuthQuerySet
{
private: 
	AuthQuerySet();
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
	// const QueryObject exampleQueryObject;
#pragma endregion Query
};