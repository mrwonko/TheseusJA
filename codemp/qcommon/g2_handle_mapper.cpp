#include "g2_handle_mapper.h"

#include <limits>

Ghoul2HandleMapper::Ghoul2HandleMapper(vm_t*& vm)
	: mNextKey(1) // Start at 1, because 0 has special meaning
	, mVM(vm)
{
}

CGhoul2Info_v* Ghoul2HandleMapper::Lookup(g2handleptr_t handle) const
{
	// use real pointers in native modules
	if (mVM->dllHandle) return reinterpret_cast<CGhoul2Info_v*>(handle);

	g2handle_t g2handle = static_cast<g2handle_t>(handle);
	auto it = mMap.find(g2handle);

	if (it == mMap.end()) return nullptr;
	return it->second;
}

void Ghoul2HandleMapper::Update(g2handleptr_t& handleInout, CGhoul2Info_v* const newValue)
{
	// native modules use real pointers instead of this mapper
	if (!mVM->dllHandle) return;

	if (handleInout) {
		if (newValue) {
			// update
			mMap[handleInout] = newValue;
		}
		else {
			// deletion
			if (mMap.erase(handleInout)) {
				// remember released key for recycling
				mFreeList.push_back(handleInout);
			}
			handleInout = 0;
		}
	}
	else {
		if (newValue) {
			// insertion
			const g2handle_t key = NextKey();
			auto& res = mMap.insert({ key, newValue });
			if (!res.second) {
				Com_Error(ERR_FATAL, "NextKey %d already present in Ghoul2HandleMapper", key);
			}
			handleInout = static_cast<g2handleptr_t>(key);
		}
		else {
			// replace nothing with nothing - do nothing
		}
	}
}

Ghoul2HandleMapper::Map::key_type Ghoul2HandleMapper::NextKey()
{
	if (mFreeList.empty()) {
		if (mNextKey == std::numeric_limits<Map::key_type>::max()) {
			// We'll probably run out of memory before this happens?
			Com_Error(ERR_FATAL, "used up all available g2handle_t values, please restart the game");
		}
		return mNextKey++;
	}
	auto res = mFreeList.back();
	mFreeList.pop_back();
	return res;
}
