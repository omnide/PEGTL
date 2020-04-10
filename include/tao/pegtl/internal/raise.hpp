// Copyright (c) 2014-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_PEGTL_INTERNAL_RAISE_HPP
#define TAO_PEGTL_INTERNAL_RAISE_HPP

#include <stdexcept>

#include "../config.hpp"

#include "enable_control.hpp"

#include "../apply_mode.hpp"
#include "../rewind_mode.hpp"
#include "../rule_list.hpp"

namespace TAO_PEGTL_NAMESPACE::internal
{
   template< typename T >
   struct raise
   {
      using rule_t = raise;
      using subs_t = empty_list;

#if defined( _MSC_VER )
#pragma warning( push )
#pragma warning( disable : 4702 )
#endif
      template< apply_mode,
                rewind_mode,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         Control< T >::raise( static_cast< const Input& >( in ), st... );
         throw std::logic_error( "code should be unreachable: Control< T >::raise() did not throw an exception" );  // LCOV_EXCL_LINE
#if defined( _MSC_VER )
#pragma warning( pop )
#endif
      }
   };

   template< typename T >
   inline constexpr bool enable_control< raise< T > > = false;

}  // namespace TAO_PEGTL_NAMESPACE::internal

#endif
