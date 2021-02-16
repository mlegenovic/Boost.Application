// Copyright 2011-2014 Renato Tegon Forti
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_APPLICATION_FEATURE_NS_SELECT_BOOST

#include <iostream>
#include <boost/application.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost;

class myapp
{
public:

   myapp(application::context&) { }

   int operator()() {
      return 0;
   }

   bool stop() {
      return true; // return true to stop, false to ignore
   }

   bool pause() {
      return true; // return true to pause, false to ignore
   }

   bool resume() {
      return true; // return true to resume, false to ignore
   }
};

BOOST_AUTO_TEST_CASE(auto_handler_test)
{
   application::context app_context;
   application::auto_handler<myapp> app(app_context);

   boost::system::error_code ec;

   BOOST_CHECK(application::launch<application::server>(app, app_context, ec) == 0);
   BOOST_CHECK(ec.value());
}
