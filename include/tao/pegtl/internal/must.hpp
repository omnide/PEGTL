// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_MUST_HPP
#define TAO_PEGTL_INTERNAL_MUST_HPP

#include "../config.hpp"

#include "enable_control.hpp"
#include "seq.hpp"
#include "success.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   // The general case applies must<> to each of the
   // rules in the 'Rules' parameter pack individually.

   template< typename... Rules >
   struct must
      : seq< must< Rules >... >
   {};

   template<>
   struct must<>
      : success
   {};

   // While in theory the implementation for a single rule could
   // be simplified to must< Rule > = sor< Rule, raise< Rule > >, this
   // would result in some unnecessary run-time overhead.

   template< typename Rule >
   struct must< Rule >
   {
      using rule_t = must;
      using subs_t = rule_list< Rule >;

      template< apply_mode A,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         if( !Control< Rule >::template match< A, rewind_mode::dontcare, Action, Control >( in, st... ) ) {
            Control< Rule >::raise( static_cast< const Input& >( in ), st... );
         }
         return true;
      }
   };

   template< typename... Rules >
   inline constexpr bool enable_control< must< Rules... > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
