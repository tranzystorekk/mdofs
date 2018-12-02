#ifndef CASCREL_CATCHTESTCONVENIENCE_HPP
#define CASCREL_CATCHTESTCONVENIENCE_HPP

#include "catch.hpp"

#define REQUIRE_EQ(expected, actual) REQUIRE(expected == actual)
#define CHECK_EQ(expected, actual) CHECK(expected == actual)

#define TC_METHOD_WITH_CLASS_NAME( className, testName ) \
TEST_CASE_METHOD( className, #className "." #testName )
#define TC_METHOD_WITH_CLASS_NAME_TAGGED( className, testName, tags ) \
TEST_CASE_METHOD( className, #className "." #testName, tags )

#endif //CASCREL_CATCHTESTCONVENIENCE_HPP
