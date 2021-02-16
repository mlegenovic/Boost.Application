// Copyright 2011-2014 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_APPLICATION_WAIT_FOR_TERMINATION_REQUEST_ASPECT_HPP
#define BOOST_APPLICATION_WAIT_FOR_TERMINATION_REQUEST_ASPECT_HPP

#include <memory>

#if defined( BOOST_WINDOWS_API )
#include <boost/application/detail/windows/wait_for_termination_request_impl.hpp>
#elif defined( BOOST_POSIX_API )
#include <boost/application/detail/posix/wait_for_termination_request_impl.hpp>
#else
#error "Sorry, no boost application are available for this platform."
#endif

namespace boost::application {

   /*!
    * \brief A contract class to be used by the user on your own
    *        class implementation of wait_for_termination_request aspect.
    *
    */
   class wait_for_termination_request
   {
   public:
      wait_for_termination_request() = default;
      wait_for_termination_request(const wait_for_termination_request&) = delete;
      virtual ~wait_for_termination_request() = default;

      wait_for_termination_request& operator=(const wait_for_termination_request&) = delete;

      /*!
       * Wait for termination request that need be
       * implemented on derived class.
       *
       */
      virtual void wait() = 0;

      /*!
       * Continue, that need be implemented on derived class.
       *
       */
      virtual void proceed() = 0;
   };

   /*!
    * \brief This aspect class handle termination request of application.
    *        User can override this behavior and define your own.
    *
    */
   class wait_for_termination_request_default_behaviour
      : public wait_for_termination_request
   {
   public:
      wait_for_termination_request_default_behaviour()
         : impl_(new detail::wait_for_termination_request_impl()){}

      /*!
       * Wait for termination request.
       *
       */
      void wait() override {
         impl_->wait();
      }

      /*!
       * Continue, cause wait to be relesed
       *
       */
      void proceed() override {
         impl_->proceed();
      }

   private:

      std::shared_ptr<detail::wait_for_termination_request_impl> impl_;

   };

} // boost::application

#endif // BOOST_APPLICATION_WAIT_FOR_TERMINATION_REQUEST_ASPECT_HPP
