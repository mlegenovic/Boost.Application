// -----------------------------------------------------------------------------
// simple_application_with_global_context.cpp : examples that show how use
// Boost.Application to make a simplest interactive (terminal) application 
// using global_context
//
// Note 1: The Boost.Application (Aspects v4) and this sample are in 
//         development process.
// -----------------------------------------------------------------------------

// Copyright 2011-2013 Renato Tegon Forti
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying 
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// -----------------------------------------------------------------------------

//[intro_global_context

#include <iostream>
#include <boost/application.hpp>

using namespace boost;

// singleton access

/*<<Optionally, create a function to give us easy access to global context>>*/
inline application::global_context_ptr this_application() {
   return application::global_context::get();
}

// my functor application

class myapp
{
public:

   /*<<Define the application operator (singleton, no param)>>*/
   int operator()()
   {
      std::cout << "Test" << std::endl;
      auto myargs = this_application()->find<application::args>();

      if (myargs)
      {
         const std::vector<std::string> &arg_vector = myargs->arg_vector();

         // only print args on screen
         for(const auto& arg : arg_vector) {
            std::cout << arg << std::endl;
         }
      }


      /*<<Add other application logic>>*/
      // code your application

      return 0;
   }

};

// main

int main(int argc, char *argv[])
{   
   /*<<Create a global context application aspect pool>>*/   
   application::global_context_ptr ctx = application::global_context::create();

   /*<<Instantiate your application>>*/    
   myapp app;
 
   /*<<Add an aspect for future use. An 'aspect' can be customized, or new aspects can be created>>*/ 
   this_application()->insert<application::args>(
      std::make_shared<application::args>(argc, argv));

   /*<<Start the application on the desired mode (common, server)>>*/  
   int ret = application::launch<application::common>(app, this_application());

   /*<<Destroy the application global context>>*/  
   application::global_context::destroy();

   return ret;
}
//]
