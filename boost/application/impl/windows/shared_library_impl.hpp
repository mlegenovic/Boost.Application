// shared_library.hpp -------------------------------------------------------//
// -----------------------------------------------------------------------------

// Copyright 2011-2012 Renato Tegon Forti

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

// -----------------------------------------------------------------------------

// Revision History
// 05-04-2012 dd-mm-yyyy - Initial Release

// -----------------------------------------------------------------------------

#ifndef BOOST_APPLICATION_SHARED_LIBRARY_IMPL_HPP
#define BOOST_APPLICATION_SHARED_LIBRARY_IMPL_HPP

#include <iostream>

#include <boost/application/config.hpp>
#include <boost/application/shared_library_initializers.hpp>

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

namespace boost { namespace application {

   class shared_library;
   class shared_library_impl : public noncopyable 
   {
      friend class shared_library;

   public:
      shared_library_impl()
         : handle_(NULL)
      {
      }
		
      virtual ~shared_library_impl()
      {
         unload();
      }
      
      template <typename T>
      shared_library_impl(const library_type<T> &sh)
         : handle_(NULL)
      {
         load(sh);
      }

      template <typename T>
      shared_library_impl(const library_type<T> &sh, boost::system::error_code &ec)
         : handle_(NULL)
      {
         load(sh, ec);
      }

      template <typename T>
      shared_library_impl(const library_type<T> &sh, unsigned long mode)
         : handle_(NULL)
      {
         load(sh, mode);
      }

      template <typename T>
      shared_library_impl(const library_type<T> &sh, unsigned long mode, 
                          boost::system::error_code &ec)
         : handle_(NULL)
      {
         load(sh, mode, ec);
      }

      template <typename T>
      void load(const library_type<T> &sh)
      {
         if (handle_) 
            unload();

         DWORD flags(0);
         path_ = sh.get().c_str();

         if (path_.is_absolute()) 
            flags |= LOAD_WITH_ALTERED_SEARCH_PATH; // usual mode, generic (plugin mode)

         boost::system::error_code ec;
         if(!load(static_cast<unsigned long>(flags), ec))
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR("LoadLibrary() failed");
      }

      template <typename T>
      void load(const library_type<T> &sh, 
                boost::system::error_code &ec)
      {
         if (handle_) 
            unload();
         
         DWORD flags(0);
         path_ = sh.get().c_str();

         if (path_.is_absolute()) 
            flags |= LOAD_WITH_ALTERED_SEARCH_PATH; // usual mode, generic (plugin mode)

         load(static_cast<unsigned long>(flags), ec);
      }

      template <typename T>
      void load(const library_type<T> &sh, unsigned long mode)
      {
         path_ = sh.get().c_str();

         boost::system::error_code ec;
         if(!load(static_cast<unsigned long>(mode), ec))
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR("LoadLibrary() failed");
      }

      template <typename T>
      void load(const library_type<T> &sh, unsigned long mode, 
                boost::system::error_code &ec)
      {
         path_ = sh.get().c_str();

         load(static_cast<unsigned long>(mode), ec);
      }

      void unload()
      {
         boost::lock_guard<boost::mutex> lock(mutex_);

         if (handle_)
         {
            FreeLibrary((HMODULE) handle_);
            handle_ = NULL;
         }

         path_.clear();
      }

      bool is_loaded() const
      {
         return (handle_ != 0); 
      }

      template <typename T>
      bool search_symbol(const symbol_type<T> &sb)
      {
         boost::system::error_code ec;
         if(get_symbol(sb, ec) == NULL)
         {
            return false;
         }

         return true;
      }

      template <typename T>
      void* get_symbol(const symbol_type<T> &sb)
      {
         boost::system::error_code ec;

         void* symbol = symbol_addr(sb, ec);
         if(symbol == NULL)
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR("GetProcAddress() failed");

         return symbol;
      }

      template <typename T>
      void* get_symbol(const symbol_type<T> &sb, boost::system::error_code &ec)
      {
         return symbol_addr(sb, ec);
      }

      template <typename T>
      void* operator()(const symbol_type<T> &sb)
      {
         boost::system::error_code ec;

         void* symbol = symbol_addr(sb, ec);
         if(symbol == NULL)
            BOOST_APPLICATION_THROW_LAST_SYSTEM_ERROR("GetProcAddress() failed");

         return symbol;
      }

      template <typename T>
      void* operator()(const symbol_type<T> &sb, boost::system::error_code &ec)
      {
         return symbol_addr(sb, ec);
      }

      const boost::filesystem::path& get_path() const
      {
         return path_;
      }

      static character_types::string_type suffix()
      {
#if defined(BOOST_APPLICATION_STD_WSTRING)
         return character_types::string_type(L".dll");
#else
         return character_types::string_type(".dll");
#endif
      }
	  
	  protected:
	  
      template <typename T>
      void* symbol_addr(const symbol_type<T> &sb, boost::system::error_code &ec)
      {
         boost::lock_guard<boost::mutex> lock(mutex_);

         // Judging by the documentation and
         // at GetProcAddress there is no version for UNICODE.
         // There can be it and is correct, as in executed 
         // units names of functions are stored in narrow characters.
         std::string as_std_string( sb.get().begin(), sb.get().end() );

         if (handle_)
            return (void*) GetProcAddress((HMODULE) handle_, as_std_string.c_str());
         else 
            BOOST_APPLICATION_SET_LAST_SYSTEM_ERROR(ec);

         return NULL;
      }
	  
      bool load(unsigned long mode, boost::system::error_code &ec)
      {
         boost::lock_guard<boost::mutex> lock(mutex_);

         DWORD flags = static_cast<DWORD>(mode);

#if defined(BOOST_APPLICATION_STD_WSTRING)
         // LoadLibraryExW
         handle_ = LoadLibraryEx(path_.wstring().c_str(), 0, flags);
#else 
         // LoadLibraryExA
         handle_ = LoadLibraryEx(path_.string().c_str(), 0, flags);
#endif

         if (!handle_) 
         {
            BOOST_APPLICATION_SET_LAST_SYSTEM_ERROR(ec);
            return false;
         }

         return true;
      }

   private:
      
      boost::mutex mutex_;  
      boost::filesystem::path path_;
      void* handle_;
   };

}} // boost::application

#endif // BOOST_APPLICATION_SHARED_LIBRARY_IMPL_HPP

