#include "PreCompile.h"
#include "BuildConfg.h"

#include "GoogleTest.h"
#include "Reflection.h"

#if UNIT_TEST
struct Base
{
	DEFINE_CLASS_INFO(Base)
};

struct Derived1 : Base
{
	DEFINE_CLASS_INFO(Derived1)
};

struct Derived2 : Base
{
	DEFINE_CLASS_INFO(Derived2)
};

TEST(ReflectionTest, HasSuperTest)
{
	EXPECT_EQ(HasSuper<Base>, false);
	EXPECT_EQ(HasSuper<Derived1>, true);
	EXPECT_EQ(HasSuper<Derived2>, true);
}

TEST(ReflectionTest, GetSuperTest)
{
	Derived1 derived;
	auto super = derived.GetTypeInfo().GetSuper();
	EXPECT_NE(super, nullptr);

	Base base;
	auto willBeNull = base.GetTypeInfo().GetSuper();
	EXPECT_EQ(willBeNull, nullptr);
}

TEST(ReflectionTest, CastTest)
{
	Derived1 derived1;
	auto super1 = derived1.GetTypeInfo().GetSuper();
	ASSERT_NE(super1, nullptr);

	Derived2 derived2;
	Base* basePtr = &derived2;
	auto willBeNull1 = Cast<Derived1>(basePtr);
	EXPECT_EQ(willBeNull1, nullptr);

	Base base;
	auto willBeNull2 = base.GetTypeInfo().GetSuper();
	EXPECT_EQ(willBeNull2, nullptr);
}
#endif