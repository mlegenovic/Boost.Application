// Copyright 2014 Renato Tegon Forti
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_APPLICATION_CONTEXT_HPP
#define BOOST_APPLICATION_CONTEXT_HPP

/// \file boost/application/context.hpp
/// \brief This file hold a context of application.
/// 
/// The library user has 2 options to use context, use a global context or local context that
/// are passed through application constructor as parameter.
///     
///  - global_context (is a singleton)
///  - context (to be used as parameter)
/// 
///  Then the 'application constructor' provided by user 
///  can be defined in 2 ways (signatures). 
/// 
///  1. 'param' based version, in this case the constructor will that receive a 
///     'context' of application as parameter.    
/// 
/// \b Examples:
/// \code
/// myapp(application::context& context)
///    : context_(context)
/// {}
/// \endcode
///
///  2. 'global_context' based version, in this case the constructor don't receive 
///      anything, the user can get access to 'context' using 
///      application::global_context::get(). 
/// 
/// \b Examples:
/// \code
/// myapp()
/// {
///    // use application::global_context::get()
/// }
/// \endcode

#include <memory>
#include <mutex>
#include <shared_mutex>

// application
#include <boost/application/config.hpp>
#include <boost/application/aspect_map.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost::application {

   namespace detail {

      // application global, replaces the use of Boost.Singularity

      template <class T> struct T_instance
      {
         static std::shared_ptr<T> ptr;
         static std::shared_mutex lock;
      };

      template <class T> std::shared_ptr<T> T_instance<T>::ptr;
      template <class T> std::shared_mutex T_instance<T>::lock;

   } // application::detail

   /*!
    * \brief This class is the base of Boost.Application.
    *        This class can hold any apect, and user can add one,
    *        or use some application modes that will add specifics
    *        aspects to it.
    *
    *        The heavy work is done by aspect_map.
    *
    *        Thanks to Vicente J. Botet Escriba that provide this
    *        important class and concept to the Boost.Application
    *        library.
    *
    * This class hold all aspects of an application.
    * you can use one of the ready-to-use aspects provided by library,
    * or define your own aspects.
    */
   class basic_context : public aspect_map
   {
   public:
      basic_context() = default;
      basic_context(const basic_context&) = delete;
      basic_context& operator=(const basic_context&) = delete;

      // nothing here! Reserved for future use.
   };

   class global_context : public basic_context
   {
   public:

      static inline std::shared_ptr<global_context> create() {
        boost::system::error_code ec;
        create(ec);

        if(ec) 
           BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR_USING_MY_EC("global context is already created", ec); 

         return instance_t::ptr;
      }

      static inline std::shared_ptr<global_context> create(boost::system::error_code &ec) BOOST_NOEXCEPT  {
         std::lock_guard<std::shared_mutex> u_guard(instance_t::lock);

         ec.clear();
         if(already_created()) {
            ec = boost::system::error_code(
                 boost::system::errc::file_exists,
                 boost::system::generic_category()
                 );

            return std::shared_ptr<global_context>();
         }

         instance_t::ptr.reset(new context_t());
         return instance_t::ptr;
      }
	  
      static inline void destroy()  {
         boost::system::error_code ec;
         destroy(ec);

         if(ec) 
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR_USING_MY_EC("no global context to destroy", ec);  
      }

      static inline void destroy(boost::system::error_code &ec) BOOST_NOEXCEPT {
         std::lock_guard<std::shared_mutex> u_guard(instance_t::lock);

         ec.clear();
         if(!already_created()) {
            ec = boost::system::error_code(
                 boost::system::errc::bad_file_descriptor,
                 boost::system::generic_category()
                 );
           return;
         }

         instance_t::ptr.reset();
      }

      static inline std::shared_ptr<global_context> get() {
         boost::system::error_code ec;
         std::shared_ptr<global_context> cxt =get(ec);

         if(ec)
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR_USING_MY_EC("there is no global context", ec); 

         return cxt;
      }

      static inline std::shared_ptr<global_context> get(boost::system::error_code &ec) BOOST_NOEXCEPT {
         std::shared_lock<std::shared_mutex> s_guard(instance_t::lock);

         ec.clear();
         if(!already_created()) {
            ec = boost::system::error_code(
                 boost::system::errc::bad_file_descriptor,
                 boost::system::generic_category()
                 );
           return std::shared_ptr<global_context>();
         }

         return instance_t::ptr;
      }

   protected:
      global_context() { }

   private:
      typedef global_context context_t;
      typedef detail::T_instance<context_t> instance_t;
      typedef std::shared_ptr<context_t> context_ptr_t;
	  
      static inline bool already_created() {
          return (instance_t::ptr != 0);
      }
   };

   typedef basic_context context;
   typedef std::shared_ptr<global_context> global_context_ptr;
   typedef std::shared_ptr<basic_context> context_ptr;

} // boost::application

#endif // BOOST_APPLICATION_CONTEXT_HPP
