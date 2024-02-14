#pragma once
#include <string>

enum QueryType : unsigned short
{
	InvalidQueryType = 0,
};

struct QueryObject
{
	QueryObject() = delete;
	QueryObject(const std::wstring_view queryString, QueryType inQueryType, bool isNeedResult)
		: query(queryString)
		, queryType(inQueryType)
		, needResult(isNeedResult)
	{}

	const std::wstring query;
	QueryType queryType;
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
	// QueryType = ExampleQueryType;
	///////////////////////////////
	// const QueryObject exampleQueryObject;
#pragma endregion Query
};