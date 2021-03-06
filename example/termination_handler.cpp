// -----------------------------------------------------------------------------
// limit_single_instance_callback.cpp : examples that show how use
// Boost.Application to make a simplest interactive (terminal) application
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

#include <iostream>
#include <boost/application.hpp>
#include <thread>
#include <chrono>

using namespace boost::application;

class myapp
{
public:

  myapp(context& context)
      : context_(context)
   {
   }

   ~myapp()
   {
      std::cout << "~myapp()" << std::endl;
   }

   void work_thread()
   {
      while(1)
      {
         std::this_thread::sleep_for(std::chrono::seconds(2));
         std::cout << "running" << std::endl;
      }
   }

   // param
   int operator()()
   {
      std::cout << "operator()" << std::endl;

	  // using [state]

      /*
      std::shared_ptr<status> state =
         context.get_aspect<status>();

      while(state->state() != status::stopped)
      {
         boost::this_thread::sleep(boost::posix_time::seconds(2));
         std::cout << "running" << std::endl;
      }
      */

      // or using [wait_for_termination_request]

      // launch a work thread
      std::thread thread([this] { work_thread(); });

      context_.find<wait_for_termination_request>()->wait();

      return 0;
   }

   bool stop()
   {
      char type;
      do
      {
         std::cout << "Do you want to exit? [y/n]" << std::endl;
         std::cin >> type;
      }
      while( !std::cin.fail() && type!='y' && type!='n' );

      if(type == 'y')
          // tell to app to continue.
         return true;

      // tell to app to exit.
      return false;
   }

private:

   context& context_;

};

// main

int main(int /*argc*/, char */*argv*/[])
{
   context app_context;
   myapp app(app_context);

   handler<>::callback stop = [&app] { return app.stop(); };

   app_context.insert<termination_handler>(
      std::make_shared<termination_handler_default_behaviour>(stop));

   return launch<common>(app, app_context);
}
