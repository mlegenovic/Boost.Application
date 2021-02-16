// Copyright 2011-2013 Renato Tegon Forti
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org/libs/application for documentation.

/*!
 * Configuration file used by Boost.Application.
 *
 * Unicode setup for Windows, System Error Handle
 * and C++11/Boost feature Select area handled here.
 *
 */

#ifndef BOOST_APPLICATION_CONFIG_HPP
#define BOOST_APPLICATION_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/predef/os.h>
#include <boost/application/system_error.hpp>
#include <boost/application/version.hpp>

// because of asio we cant use CXX11_HDR_SYSTEM_ERROR
// The standard library does not provide header <system_error>.
// BOOST_NO_CXX11_HDR_SYSTEM_ERROR

#if defined(BOOST_WINDOWS_API)
#   if defined(_UNICODE) || defined(UNICODE)
#      define BOOST_APPLICATION_STD_WSTRING
#   endif
#endif

// error handle for Boost.Application lib, based on Boost.System.
// user can set this macro for example to BOOST_THROW_EXCEPTION
// if they whant to use Boost.Exception.

#define BOOST_APPLICATION_THROW(EX) throw EX

// error ctrl

// report location of error.
#define BOOST_APPLICATION_SOURCE_LOCATION                                      \
   "in file '" __FILE__ "', line " BOOST_STRINGIZE(__LINE__) ": "

// define BOOST_APPLICATION_TROWN_MY_OWN_EXCEPTION if you want
// THROW your own EXCEPTION
#if defined(BOOST_APPLICATION_TROWN_MY_OWN_EXCEPTION)

#define BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR(what)                        \
   BOOST_APPLICATION_TROWN_MY_OWN_EXCEPTION(                                   \
      what " : " + boost::system::error_code(                                  \
         last_error(),                                                         \
            boost::system::system_category()).message(),                       \
               last_error())

#define BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR_USING_MY_EC(what, ec)        \
   BOOST_APPLICATION_TROWN_MY_OWN_EXCEPTION(                                   \
      what " : " + ec.message(),                                               \
         ec.value())

#else

// use this version if you have not recovered the 'c' ie right after the error.
// this version recovery 'ec' internaly.
#define BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR(what)                        \
   BOOST_APPLICATION_THROW(boost::system::system_error(                        \
      boost::system::error_code(last_error(),                                  \
         boost::system::system_category()),                                    \
            BOOST_APPLICATION_SOURCE_LOCATION what))

// use this version when you already have the value of 'ec', ie you already
// called 'last_error'.
#define BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR_USING_MY_EC(what, ec)        \
   BOOST_APPLICATION_THROW(boost::system::system_error(                        \
      ec, BOOST_APPLICATION_SOURCE_LOCATION what))

#endif

#endif // BOOST_APPLICATION_CONFIG_HPP
