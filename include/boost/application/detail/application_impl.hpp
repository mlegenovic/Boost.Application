// Copyright 2011-2014 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_APPLICATION_DETAIL_APPLICATION_IMPL_HPP
#define BOOST_APPLICATION_DETAIL_APPLICATION_IMPL_HPP

// application
#include <boost/application/context.hpp>

namespace boost::application::detail {

   // share context on modes
   class application_impl
   {
   public:
      application_impl(const application_impl&) = delete;
      application_impl& operator=(const application_impl&) = delete;

      explicit application_impl(context &cxt)
         : context_(cxt)
      {
      }

      context &get_context()
      {
         return context_;
      }

   protected:
      context &context_;

   };

} // boost::application::detail

#endif // BOOST_APPLICATION_DETAIL_APPLICATION_IMPL_HPP
