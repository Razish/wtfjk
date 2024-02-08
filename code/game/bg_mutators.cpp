#include "bg_mutators.h"

#ifdef SP_GAME
	#include "g_local.h"
#else
	#include "cg_local.h"
#endif

const stringID_table_t mutatorStrings[NUM_MUTATORS + 1] = {
	// clang-format off
	ENUM2STRING(MUTATOR_NONE),
	ENUM2STRING(MUTATOR_FILMNOIR),
	ENUM2STRING(MUTATOR_JAWAINVASION),
	ENUM2STRING(MUTATOR_PEACEANDQUIET),
	ENUM2STRING(MUTATOR_FAMOUS),
	ENUM2STRING(MUTATOR_ROSHINVASION),
	ENUM2STRING(MUTATOR_SPLIT),
	ENUM2STRING(MUTATOR_DRUNK),
	ENUM2STRING(MUTATOR_GIANTS),
	ENUM2STRING(MUTATOR_INCLEMENTWEATHER),
	ENUM2STRING(MUTATOR_PARANOIA),
	ENUM2STRING(MUTATOR_DICKBUTT),
	ENUM2STRING(MUTATOR_SPINNYCAM),
	ENUM2STRING(MUTATOR_RANDOMBOSSFIGHT),
	{NULL, -1}
	// clang-format on
};

mutator_e BG_GetCurrentMutator(void) {
	#ifdef SP_GAME
		return level.mutators.state.activeMutator;
	#else
		return cg.mutators.activeMutator;
	#endif
}