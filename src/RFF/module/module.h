#pragma once

#include <tuple>

#include "eventHandler/eventHandler.h"
#include "RFF.h"

namespace RFF {
    
	/**
		\brief Groups event handlers and dispatches events
	*/
	template <typename ...evtHandlers_t>
	class Module {
        typedef Module<evtHandlers_t...> me_t;
        
	public:
		std::tuple<evtHandlers_t* ...> evtHandlers;


        Module() = delete;
        Module(evtHandlers_t& ... modules) : evtHandlers{std::make_tuple(&modules...)}{};
        Module(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        Module(me_t&& other) = default;
        me_t& operator=(me_t&& other) = default;

		template<typename evt_t>
		void callHandlers(evt_t const& event) {
			std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
				(..., (*eh)(evt_t{event}));
			}, evtHandlers);
		}



	};
}
