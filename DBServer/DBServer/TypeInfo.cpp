#include "PreCompile.h"

#include "TypeInfo.h"
#include "Property.h"

#include <utility>

void TypeInfo::AddProperty(const Property& inProperty)
{
	std::string propertyName(inProperty.GetName());
	if (propertyMap.empty() == false)
	{
		if (propertyMap.contains(propertyName))
		{
			return;
		}
	}

	propertyMap.emplace(propertyName, inProperty);
	propertyTypeNameList.emplace_back(inProperty.GetTypeName());
}

void TypeInfo::GetAllProperties(OUT std::vector<std::pair<PropertyName, PropertyTypeName>>& propertyList) const
{
	for (const auto& itor : propertyMap)
	{
		propertyList.emplace_back(itor.first, itor.second.GetTypeName());
	}
}

std::vector<PropertyTypeName> TypeInfo::GetAllPropertyTypeName() const
{
	return propertyTypeNameList;
}

size_t TypeInfo::GetNumOfProperty() const
{
	return propertyMap.size();
}
