// Copyright 2014 Renato Tegon Forti
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_APPLICATION_PATH_ASPECT_HPP
#define BOOST_APPLICATION_PATH_ASPECT_HPP

#if defined( BOOST_WINDOWS_API )
#include <boost/application/detail/windows/path_impl.hpp>
#elif defined( BOOST_POSIX_API )
#include <boost/application/detail/posix/path_impl.hpp>
#else
#error "Sorry, no boost application are available for this platform."
#endif

namespace boost::application {

    class path 
    {
    public:
        path() : impl_(new detail::default_path_impl) {}

        std::filesystem::path current_path(void)
        {
            return impl_->current_path();
        }
      
        std::filesystem::path location(std::error_code &ec)
        {
            return impl_->location(ec);
        }

        std::filesystem::path location()
        {
            return impl_->location();
        }

        std::filesystem::path executable_path_name(std::error_code &ec)
        {
           return impl_->location(ec);
        }

        std::filesystem::path executable_path_name()
        {
            return impl_->location();
        }

        std::filesystem::path executable_path(std::error_code &ec)
        {
            return location(ec).parent_path();
        }

        std::filesystem::path executable_path()
        {
            return impl_->location().parent_path();
        }

        std::filesystem::path executable_full_name(std::error_code &ec)
        {
            return impl_->location(ec).filename();
        }

        std::filesystem::path executable_full_name(void)
        {
            return impl_->location().filename();
        }

        std::filesystem::path executable_name(std::error_code &ec)
        {
            return impl_->location(ec).stem();
        }

        std::filesystem::path executable_name(void)
        {
            return impl_->location().stem();
        }

        std::filesystem::path home_path(void)
        {
            return impl_->home_path();
        }

        std::filesystem::path config_path(void)
        {
            return impl_->config_path();
        }

        std::filesystem::path app_data_path(void)
        {
            return impl_->app_data_path();
        }

        std::filesystem::path temp_path(void)
        {
            return impl_->temp_path();
        }

    private:
        std::shared_ptr<detail::default_path_impl> impl_;
    };

} // boost::application

#endif // BOOST_APPLICATION_PATH_ASPECT_HPP
