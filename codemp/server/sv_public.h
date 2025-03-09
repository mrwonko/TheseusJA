#pragma once

// forward declarations for the different sharedEntity_t kinds

enum class ModuleContext {
	Native,
	QVM,
};

template<ModuleContext Ctx>
struct sharedEntityMapper_t;

using sharedEntity_native_t = sharedEntityMapper_t<ModuleContext::Native>;
using sharedEntity_qvm_t = sharedEntityMapper_t<ModuleContext::QVM>;
