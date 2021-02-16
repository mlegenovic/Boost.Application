#define BOOST_APPLICATION_FEATURE_NS_SELECT_BOOST

//[intro
#include <iostream>
#include <memory>
#include <boost/application.hpp>
#include <boost/test/unit_test.hpp>

// other unit
#include "myapp.h"

BOOST_AUTO_TEST_CASE(tunit_test)
{
   myapp app;
   application::global_context_ptr ctx = application::global_context::create();

   this_application()->insert<application::args>(
      std::make_shared<application::args>(unit_test::framework::master_test_suite().argc,
                                          unit_test::framework::master_test_suite().argv));

   BOOST_CHECK(application::launch<application::common>(app, ctx) == 0);

   application::global_context::destroy();
}
