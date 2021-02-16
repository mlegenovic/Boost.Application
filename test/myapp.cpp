#include "myapp.h"

#include <memory>

myapp::myapp(void)
{
}

myapp::~myapp(void)
{
}

int myapp::operator()(void)
{
   auto myargs = this_application()->find<application::args>();

   if (myargs)
   {
      std::vector<std::string> arg_vector = myargs->arg_vector();
   }

   return 0;
}