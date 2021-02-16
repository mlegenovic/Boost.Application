// Copyright 2013 Vicente J. Botet Escriba.
// Copyright 2014 Renato Tegon Forti.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_APPLICATION_ASPECT_MAP_HPP
#define BOOST_APPLICATION_ASPECT_MAP_HPP

#include <memory>
#include <utility>
#include <typeindex>
#include <mutex>

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
# pragma once
#endif

/// \file boost/application/aspect_map.hpp
/// \brief Contains only the boost::application::aspect_map container class that is capable of
/// store any application aspects in thread safe way. 
/// Internal locking and external locking support.

namespace boost::application {

   /*!
    * \brief The aspect_map class implementation.
    *
    * This is a base class for application context.
    * This class provides all methods to aspects manipulation available
    * to user.
    *
    * This class used 'lock' object as 'permit' that gives you rights
    * to manipulate aspects on it.
    *
    * Internal and External locking Version that can be used as part of an
    * atomic transaction are available.
    *
    * <STRONG> Thread Safe: </STRONG> Yes <BR>
    * <STRONG> Exception Safe: </STRONG>  Yes
    *
    */
   class aspect_map {
      typedef size_t size_type;

      typedef std::type_index key_type;
      typedef std::shared_ptr<void> value_type;
      typedef std::unordered_map<key_type, value_type> map_type;

      map_type aspects_;
      mutable std::recursive_mutex mutex_;

      /// @cond
      template <class T>
      key_type aspec_id() {
        return std::type_index(typeid(T));
      }
      /// @endcond

     void lock() const { mutex_.lock(); }
     void unlock() const { mutex_.unlock(); }

     friend class std::unique_lock<aspect_map>;

   public:

      aspect_map() = default;

      aspect_map(aspect_map const&) = delete;
      aspect_map& operator=(aspect_map const&) = delete;

      /*!
       * Lookup a aspect and return the shared_ptr<T> of it.
       * Internal locking Version.
       *
       * \b Effects: If the the aspect associated to the type T exists,
       *             returns an shared_ptr<T> of it; otherwise a disengaged
       *             object.
       *
       * \return The pointer /c shared_ptr aspect or nullptr /c shared_ptr
       *         if aspect don't exists.
       * \throw Nothing.
       */
      template <class T>
      std::shared_ptr<T> find() {
         std::unique_lock<aspect_map> guard(*this);
         return find<T>(guard);
      }

      /*!
       * Lookup a aspect and return the shared_ptr<T> of it.
       * External locking Version, can be used as part of an atomic transaction.
       *
       * \b Effects: If the the aspect associated to the type T exists,
       *             returns an shared_ptr<T> of it; otherwise a disengaged
       *             object.
       *
       * \return The pointer /c shared_ptr aspect or nullptr /c shared_ptr
       *         if aspect don't exists.
       * \throw std::logic_error if guard hold Wrong Object;
       *        Does not owns correct lock.
       */
      template <class T>
      std::shared_ptr<T> find(std::unique_lock<aspect_map>& guard) {
         ensure_correct_lock(guard);
         auto it = aspects_.find( aspec_id<T>() );

         if (aspects_.cend() == it)
            return std::shared_ptr<T>();

         return std::static_pointer_cast<T>(it->second);
      }

      /*!
       * Checks if aspect associated to the type T exists
       * Internal locking Version.
       *
       * The name count is chosen for sake of consistency with existing associative
       * container requirements and std::map.
       *
       * \b Effects: If the the aspect associated to the type T exists,
       *             returns an shared_ptr<T> of it; otherwise a disengaged
       *             object.
       *
       * \return 1 if aspect is present; 0 otherwise.
       * \throw Nothing.
       */
      template <class T>
      size_type count() {
         std::unique_lock<aspect_map> guard(*this);
         return count<T>(guard);
      }

      /*!
       * Checks if aspect associated to the type T exists
       * External locking Version, can be used as part of an atomic transaction.
       *
       * The name count is chosen for sake of consistency with existing associative
       * container requirements and std::map.
       *
       * \b Effects: If the the aspect associated to the type T exists,
       *             returns an shared_ptr<T> of it; otherwise a disengaged
       *             object.
       *
       * \return 1 if aspect is present; 0 otherwise.
       * \throw Nothing.
       */
      template <class T>
      size_type count(std::unique_lock<aspect_map>& guard) {
         // this is not needed
         // ensure_correct_lock(guard);
         if(find<T>(guard))
            return 1;

         return 0;
      }

      /*!
       * Insert a aspect if its aspect id (key_type(T)) is not already present.
       * Internal locking Version.
       *
       * \b Effects: Insert aspect if id (key_type(T)) key is not present
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       * \param asp The aspect that you want to add to the aspect_map.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw Any exception throw due to resources unavailable.
       */
      template <class T>
      std::shared_ptr<T> insert(std::shared_ptr<T> asp) {
         std::unique_lock<aspect_map> guard(*this);
         return insert<T>(asp, guard);
      }

      /*!
       * Insert a aspect if its aspect id (key_type(T)) is not already present.
       * External locking Version, can be used as part of an atomic transaction.
       *
       * \b Effects: Insert aspect if id (key_type(T)) key is not present
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       * \param asp The aspect that you want to add to the aspect_map.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw std::logic_error if guard hold Wrong Object;
       *        Does not owns correct lock, or any exception throw due to
       *        resources unavailable.
       */
      template <class T>
      std::shared_ptr<T> insert(std::shared_ptr<T> asp,
         std::unique_lock<aspect_map>& guard) {
         ensure_correct_lock(guard);
         key_type ti = aspec_id<T>();

         auto temp = find<T>(guard);

         if(temp)
            return temp;

         aspects_.insert(std::make_pair(ti, asp));
         return std::shared_ptr<T>();
      }

      /*!
       * Exchange a stored aspect to another one.
       * Internal locking Version.
       *
       * \b Effects: Insert aspect if its key is not present, otherwise replace
       *             existing aspect with new aspect.
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       * \param asp The aspect that you want to add/exchange.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw Nothing.
       */
      template <class T>
      std::shared_ptr<T> exchange(std::shared_ptr<T> asp) {
         std::unique_lock<aspect_map> guard(*this);
         return exchange(asp, guard);
      }

      /*!
       * Exchange a stored aspect to another one.
       * External locking Version, can be used as part of an atomic transaction.
       *
       * \b Effects: Insert aspect if its key is not present, otherwise replace
       *             existing aspect with new aspect.
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       * \param asp The aspect that you want to add/exchange.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw std::logic_error if guard hold Wrong Object;
       *        Does not owns correct lock.
       */
      template <class T>
      std::shared_ptr<T> exchange(std::shared_ptr<T> asp,
         std::unique_lock<aspect_map>& guard) {
         ensure_correct_lock(guard);
         key_type ti = aspec_id<T>();

         auto temp = find<T>(guard);

         if(temp)
            aspects_.erase(ti);

         aspects_.insert(std::make_pair(ti, asp));
         return std::shared_ptr<T>();
      }

      /*!
       * Remove a stored aspect of the aspect_map.
       * Internal locking Version.
       *
       * \b Effects: Erase object with given aspect type (key_type(T)) from the
       *             container, if such is present.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw Nothing.
       */
      template <class T>
      std::shared_ptr<T> erase() {
         std::unique_lock<aspect_map> guard(*this);
         return erase<T>(guard);
      }

      /*!
       * Remove a stored aspect of the aspect_map.
       * External locking Version, can be used as part of an atomic transaction.
       *
       * \b Effects: Erase object with given aspect type (key_type(T)) from the
       *             container, if such is present.
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       * \throw std::logic_error if guard hold Wrong Object;
       *        Does not owns correct lock.
       */
      template <class T>
      std::shared_ptr<T> erase(std::unique_lock<aspect_map>& guard) {
         ensure_correct_lock(guard);

         key_type ti = aspec_id<T>();

         auto temp = find<T>(guard);

         if(temp) {
            aspects_.erase(ti);
            return temp;
         }

         return std::shared_ptr<T>();
      }

      /*!
       * Insert a aspect, performing a reduction if given aspect
       * type (key_type(T)) is already present.
       *
       * \b Effects: Insert aspect if aspect type (key_type(T)) is not present;
       *             otherwise insert f(old, obj.second), where old is the
       *             mapped values of the existing item.
       *             Functor f may be invoked multiple times
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       *
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       *
       * \throw Any exception throw due to resources unavailable.
       */
      template<class T, class F>
      std::shared_ptr<T> reduce(std::shared_ptr<T> asp, F f) {
         std::unique_lock<aspect_map> guard(*this);
         return reduce<T, F>(asp, f, guard);
      }

      /*!
       * Insert a aspect, performing a reduction if given aspect
       * type (key_type(T)) is already present.
       *
       * \b Effects: Insert aspect if aspect type (key_type(T)) is not present;
       *             otherwise insert f(old, obj.second), where old is the
       *             mapped values of the existing item.
       *             Functor f may be invoked multiple times
       *
       * \pre <tt> asp.get() != nullptr </tt>
       * \post <tt> get_aspect<T>.get() != nullptr </tt>
       *
       * \return An <tt> shared_ptr </tt> of aspect of the type T previously
       *         associated with the given aspect type (key_type(T)),
       *         or a disengaged <tt> shared_ptr </tt> if the key was
       *         not present.
       *
       * \throw std::logic_error if guard hold Wrong Object;
       *        Does not owns correct lock, or any exception throw due to
       *        resources unavailable.
       */
      template<class T, class F>
      std::shared_ptr<T> reduce(std::shared_ptr<T> asp, F f, std::unique_lock<aspect_map>& guard) {
         ensure_correct_lock(guard);
         key_type ti = aspec_id<T>();

         auto tmp = find<T>(guard);

         if (tmp) {
            std::shared_ptr<T> red = f(tmp, asp);
            aspects_.erase(ti);
            aspects_.insert(std::make_pair(ti, red));

            return tmp;
         }

         aspects_.insert(std::make_pair(ti, asp));
         return std::shared_ptr<T>();
      }

      /*!
       * Estimate of the number of aspects in the container.
       *
       * \b Effects: This function does not modify the container in any way.
       *
       * \return Estimate of the number of aspects in the container. The
       *         estimate is exact if no modications occur during the
       *         invocation.
       * \throw Nothing.
       */
      size_type size() const {
         std::unique_lock<aspect_map> guard(const_cast<aspect_map&>(*this));
         return aspects_.size();
      }

      /*!
       * Checks if container is empty
       *
       * \b Effects: This function does not modify the container in any way.
       *
       * \return Returns whether the map container is empty
       *         (i.e. whether its size is 0).
       *
       * \throw Nothing.
       */
      bool empty() const {
         return (size() != 0);
      }

      /*!
       * Clear container.
       * Internal locking Version Only.
       *
       * \b Effects:  Erases all aspects from the container
       *
       * \throw Nothing.
       */
      void clear() {
         std::unique_lock<aspect_map> guard(*this);
         aspects_.clear();
      }

   private:

      /// @cond
      inline void ensure_correct_lock(std::unique_lock<aspect_map>& guard) {
         if (guard.mutex() != this)
            throw std::logic_error("Locking Error: Wrong Object Locked");
         if (!guard.owns_lock())
            guard.lock();
      }
      /// @endcond

   }; // aspect_map

}  // boost::application

#endif // BOOST_APPLICATION_ASPECT_MAP_HPP
