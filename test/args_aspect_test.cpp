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

BOOST_AUTO_TEST_CASE(args_aspect_test)
{
   application::args myargs(unit_test::framework::master_test_suite().argc,
                            unit_test::framework::master_test_suite().argv);

   BOOST_CHECK(myargs.argc());

   const std::vector< std::string > &argvec = myargs.arg_vector();

   BOOST_CHECK(argvec.size());
}
