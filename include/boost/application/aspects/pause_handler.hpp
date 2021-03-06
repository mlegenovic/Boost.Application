// Copyright 2011-2014 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_APPLICATION_ASPECT_PAUSE_HANDLER_HPP
#define BOOST_APPLICATION_ASPECT_PAUSE_HANDLER_HPP

#include <boost/application/config.hpp>
#include <boost/application/handler.hpp>

namespace boost::application {

   class pause_handler : public handler<>
   {
   public:
      pause_handler(const callback& cb)
         : handler<>(cb) {}
   };

   class pause_handler_default_behaviour : public pause_handler
   {
   public:
      pause_handler_default_behaviour(const callback& cb)
         : pause_handler(cb) {}
   };

} // boost::application

#endif // BOOST_APPLICATION_ASPECT_PAUSE_HANDLER_HPP
