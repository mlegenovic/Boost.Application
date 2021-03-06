// Copyright 2011-2014 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_APPLICATION_PID_ASPECT_HPP
#define BOOST_APPLICATION_PID_ASPECT_HPP

#include <boost/application/config.hpp>

#if defined( BOOST_WINDOWS_API )
#include <boost/application/detail/windows/process_id_impl.hpp>
#elif defined( BOOST_POSIX_API )
#include <boost/application/detail/posix/process_id_impl.hpp>
#else
#error "Sorry, no boost application are available for this platform."
#endif

namespace boost::application {

   /*!
    * \brief A class aspect to be used to obtain a PID of proccess.
    *
    */
   class process_id 
      : public detail::process_id_impl {
      typedef detail::process_id_impl base_t;   

   public:

      typedef process_id_impl::native_pid_t native_pid_t;

      process_id() = default;

      explicit process_id(native_pid_t p)
         : base_t(p)
      {}

      virtual ~process_id() = default;

      /*!
       * Obtain a PID of proccess.
       *
       */
      native_pid_t pid() noexcept {
         return base_t::pid();
      }
   };

} // boost::application

#endif // BOOST_APPLICATION_PID_ASPECT_HPP
