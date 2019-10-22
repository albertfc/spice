/*
 * This file is part of SPICE.
 * Copyright (C) 2019 Albert Farres

 * SPICE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SPICE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SPICE.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

//#include "ustl/uvector.h"
#include "constants.h"

struct TimerIface {
	virtual ~TimerIface() = default;
	virtual void increments_one_ms( void )= 0;
};

class Timer {
public:
	// regirter a function to be run every ms
	//void register_ms( TimerIface * const t ) volatile { timers.push_back( t ); }
	// Cannot use a vector because it cannot be volatile :(
	void register_ms( TimerIface * const t ) volatile { timers[_idx++] = t; }
	// call this function every ms to run all registered functions
	void run( void ) volatile { for( auto t: timers ) if(t) t->increments_one_ms(); }

private:
	//ustl::vector<TimerIface *> timers;
	int _idx=0;
	TimerIface * timers[TIME_OBJS_MAX];
};
