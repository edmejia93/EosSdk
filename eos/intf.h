// Copyright (c) 2013 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#ifndef EOS_INTF_H
#define EOS_INTF_H

#include <string>
#include <stddef.h>

#include <eos/base.h>
#include <eos/base_handler.h>
#include <eos/base_mgr.h>
#include <eos/iterator.h>

#include <eos/types/intf.h>

/**
 * @file
 * Base interface types.
 *
 * A base interface contains non-media or interface type specific information.
 * Also included in this module is the interface ID type intf_id_t, used
 * to uniquely identify any interface in the system of any type, as well as
 * common enumerates.
 */

namespace eos {

class intf_mgr;

/// This class receives changes to base interface attributes.
class EOS_SDK_PUBLIC intf_handler : public base_handler<intf_mgr, intf_handler> {
 public:
   explicit intf_handler(intf_mgr *);
   intf_mgr * get_intf_mgr() const;

   /**
    * Registers this class to receive change updates on the interface.
    *
    * Expects a boolean signifying whether notifications should be
    * propagated to this instance or not.
    */
   void watch_all_intfs(bool);

  /**
    * Registers this class to receive change updates on the given interface.
    *
    * Expects the id of the corresponding interface and a boolean signifying whether
    * notifications should be propagated to this instance or not.
    */
   void watch_intf(intf_id_t, bool);

   /**
    * Handler called when a new interface is created.
    *
    * After any *intf_handler::on_*intf_create method is called,
    * the given intf_id is valid and will succeed when used within
    * any of the *intf_mgr classes appropriate for that interface
    * type.
    * 
    * Between the various *intf_handler::on_*intf_create methods,
    * the on_intf_create method is guaranteed to called first.
    */
   virtual void on_intf_create(intf_id_t);
   /**
    * Handler called when an interface has been removed.
    *
    * After any *intf_handler::on_*intf_delete method is called,
    * the given intf_id is valid and will succeed when used within
    * all the *intf_mgr classes associated for that interface.
    * 
    * Between the various *intf_handler::on_*intf_delete methods,
    * the on_intf_delete method is guaranteed to called first.
    */
   virtual void on_intf_delete(intf_id_t);
   /**
    * Handler called when the operational status of an interface changes.
    *
    * Note that for physical interfaces, the on_oper_status handler will
    * get called with INTF_OPER_NULL when the underlying hardware for
    * that physical interface is removed.
    */
   virtual void on_oper_status(intf_id_t, oper_status_t);
   /// Handler called after an interface has been configured to be enabled.
   virtual void on_admin_enabled(intf_id_t, bool);
   /// Handler called when the configured description of an interface changes.
   virtual void on_intf_description(intf_id_t, const std::string &);
};

class intf_iter_impl;

/// An interface iterator
class EOS_SDK_PUBLIC intf_iter_t : public iter_base<intf_id_t,
                                                    intf_iter_impl> {
 private:
   friend class intf_iter_impl;
   explicit intf_iter_t(intf_iter_impl * const) EOS_SDK_PRIVATE;
};

/**
 * The interface manager.
 * This class inspects and configures base interface attributes.
 */
class EOS_SDK_PUBLIC intf_mgr : public base_mgr<intf_handler, intf_id_t> {
 public:
   virtual ~intf_mgr();

   /// Iterates over all interfaces currently available in the system.
   virtual intf_iter_t intf_iter() const = 0;

   /**
    * Returns whether the given interface exists.
    *
    * If exists returns true, then this intf_id_t can be successfully
    * passed into every method of the intf_mgr. If not, then methods
    * of the intf_mgr can throw a no_such_interface_error exception.
    */
   virtual bool exists(intf_id_t) const = 0;

   // Attribute accessors
   /// Returns true if the given interface is configured to be enabled
   virtual bool admin_enabled(intf_id_t) const = 0;
   /// Configures the enabled status of the interface
   virtual void admin_enabled_is(intf_id_t, bool) = 0;
   /// Returns the configured description of the given interface.
   virtual std::string description(intf_id_t) const = 0;
   /**
    * Configure the description of the given interface.
    * Creates a copy of the passed in string description.
    */
   virtual void description_is(intf_id_t, char const *) = 0;
   /// Configure the description of the given interface.
   virtual void description_is(intf_id_t, const std::string &) = 0;
   /// Inspects the current operational status of the given interface.
   virtual oper_status_t oper_status(intf_id_t) const = 0;

 protected:
   intf_mgr() EOS_SDK_PRIVATE;
   friend class intf_handler;

 private:
   EOS_SDK_DISALLOW_COPY_CTOR(intf_mgr);
};


/**
 * The interface counter manager.
 * This class inspects base interface counters and statistics.
 */
class EOS_SDK_PUBLIC intf_counter_mgr {
 public:
   virtual ~intf_counter_mgr();
   
   /// Get the current counters of the given interface.
   virtual intf_counters_t counters(intf_id_t) const = 0;
   /// Get the current traffic rates of the given interface.
   virtual intf_traffic_rates_t traffic_rates(intf_id_t) const = 0;
   
 protected:
   intf_counter_mgr() EOS_SDK_PRIVATE;
   
 private:
   EOS_SDK_DISALLOW_COPY_CTOR(intf_counter_mgr);
};

}

#include <eos/inline/intf.h>

#endif // EOS_INTF_H
