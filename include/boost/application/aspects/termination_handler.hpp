// Copyright 2011-2014 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_APPLICATION_ASPECT_TERMINATION_HANDLER_HPP
#define BOOST_APPLICATION_ASPECT_TERMINATION_HANDLER_HPP

#include <boost/application/handler.hpp>

namespace boost::application {

   class termination_handler : public handler<>
   {
   public:
      explicit termination_handler(const callback& cb)
         : handler<>(cb) {}
   };

   class termination_handler_default_behaviour : public termination_handler
   {
   public:
      explicit termination_handler_default_behaviour(const callback& cb)
         : termination_handler(cb) {}
   };

} // boost::application

#endif // BOOST_APPLICATION_ASPECT_TERMINATION_HANDLER_HPP

