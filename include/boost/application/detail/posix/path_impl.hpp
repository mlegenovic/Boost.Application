// Copyright 2014 Renato Tegon Forti
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_APPLICATION_DETAIL_POSIX_PATH_FROM_ME_HPP
#define BOOST_APPLICATION_DETAIL_POSIX_PATH_FROM_ME_HPP

#include <boost/application/config.hpp>
#include <boost/predef/os.h>

#include <filesystem>
#include <system_error>
#include <cstdlib>
#include <pwd.h>
#include <unistd.h>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

namespace boost::application::detail {

    class default_path_impl
    {
        std::filesystem::path full_path_;

        std::filesystem::path path_from_me(std::error_code &ec)  {
            return std::filesystem::read_symlink("/proc/self/exe", ec);
        }

        std::filesystem::path getenv(const char* env_name)
        {
            const char* res = ::getenv(env_name);
            return res ? res : std::filesystem::path();
        }

    public:

        std::filesystem::path current_path()
        {
           return std::filesystem::current_path();
        }

        const std::filesystem::path& location(std::error_code &ec)
        {
            if(!full_path_.empty())
                return full_path_;

            std::filesystem::path full_path
                = path_from_me(ec);
            if(ec)
                full_path_ = std::filesystem::path();

            full_path_ = full_path;

            return full_path_;
        }

        const std::filesystem::path& location()
        {
            if(!full_path_.empty())
                return full_path_;

            std::error_code ec;

            full_path_ = location(ec);

            if (ec) {
                boost::throw_exception(
                    std::system_error(
                    ec, "location() failed"
                    ));
            }

            return full_path_;
        }

        inline std::filesystem::path home_path()
        {
            std::filesystem::path path = getenv("HOME");
            if(path.empty())
            {
                struct passwd* pwd = getpwuid(getuid());
                if(pwd)
                    return pwd->pw_dir;
                return ".";
            }
            return path;
        }

        inline std::filesystem::path app_data_path()
        {
            std::filesystem::path path = getenv("XDG_DATA_HOME");
            if(path.empty()) {
    #if BOOST_OS_MACOS
                return home_path() / "Library/Preferences/";
    #else
                return home_path() / ".local/share";
    #endif
            }
            return path;
        }

        inline std::filesystem::path config_path()
        {

            std::filesystem::path path = getenv("XDG_CONFIG_HOME");
            if(path.empty()) {
    #if BOOST_OS_MACOS
                return home_path() / "Library/Preferences/";
    #else
                return home_path() / ".config";
    #endif
            }
            return path;
        }

        inline std::filesystem::path temp_path()
        {
            std::filesystem::path path = getenv("TMPDIR");
            if(path.empty())
                return "/tmp"; // Fallback if TMPDIR not available
            return path;
        }
    };

} // namespace boost::application::detail

#endif // BOOST_APPLICATION_DETAIL_POSIX_PATH_FROM_ME_HPP
