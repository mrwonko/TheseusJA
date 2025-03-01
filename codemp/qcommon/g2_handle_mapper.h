/*
===========================================================================
Copyright(C) 2025, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and /or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see < http://www.gnu.org/licenses/>.
========================================================================== =
*/

#pragma once

#include <unordered_map>
#include <deque>

#include "qcommon/qcommon.h"
#include "ghoul2/ghoul2_shared.h"

// When using a QVM, we cannot pass CGhoul2Info_v* pointers directly into the modules (as the memory is not accessible),
// so this class translates them into opaque g2handle_t values instead.
// Native (non-QVM) modules in theory should also treat these pointers as opaque handles, but they might not,
// so we expose the original pointers unchanged in that case.
class Ghoul2HandleMapper {
	typedef std::unordered_map<g2handle_t, CGhoul2Info_v*> Map;
	typedef std::deque<Map::key_type> FreeList;

public:
	Ghoul2HandleMapper(vm_t*& vm);

	// Resolves a handle.
	// NULL and invalid handles yield NULL.
	CGhoul2Info_v* Lookup(g2handleptr_t handle) const;
	// Update creates, updates or deletes an entry, based on which arguments are null.
	// Passing a null handle and a value creates a mapping entry and returns it in the handle.
	// Passing a valid handle and a null value deletes the entry and nulls the handle.
	// Passing a valid handle and a new value updates the entry value and keeps the handle unchanged.
	void Update(g2handleptr_t& handleInout, CGhoul2Info_v* newValue);

private:
	// Uses the latest entry in the Free List, or otherwise the first unused index.
	// ERR_FATALs when out of keys.
	Map::key_type NextKey();

	Map mMap;
	Map::key_type mNextKey;
	// We keep track of released handles so they can be re-used.
	FreeList mFreeList;
	vm_t*& mVM;
};