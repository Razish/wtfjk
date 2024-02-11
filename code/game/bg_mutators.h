#pragma once

#include "../qcommon/q_shared.h"

// if you add/remove these, you also need to update:
//
// - mutatorStrings in code/game/bg_mutators.cpp
// - allowedMutators in code/game/g_mutators.cpp
// - mutatorFuncs in code/game/g_mutators.cpp
//
enum mutator_e {
	MUTATOR_NONE = 0,
	MUTATOR_FILMNOIR,			 // TODO: film noir
	MUTATOR_JAWAINVASION,		 // TODO: jawas are spawned at nearby entities out of LoS, and behind the player
	MUTATOR_PEACEANDQUIET,		 // TODO: hearing loss, complete with tinnitus
	MUTATOR_FAMOUS,				 // TODO: all NPCs follow you
	MUTATOR_ROSHINVASION,		 // all NPCs become rosh and taunt excessively
	MUTATOR_SPLIT,				 // killing an enemy splits it into two smaller enemies
	MUTATOR_DRUNK,				 // vision is warped, controls are occasionally inverted, enemy animations sped up
	MUTATOR_GIANTS,				 // all enemies are giants
	MUTATOR_INCLEMENTWEATHER,	 // thunder and lightning, very very frightening
	MUTATOR_PARANOIA,			 // TODO: bunch of ghost enemies spawn and float around
	MUTATOR_DICKBUTT,			 // TODO: dickbutts!
	MUTATOR_SPINNYCAM,			 // camera spins around slowly (roll)
	MUTATOR_RANDOMBOSSFIGHT,	 // it's time to fight a boss (or two (or three)), 'cause why not :D
	MUTATOR_MOUSEBLASTERS,		 // enemy blasters shoot out mouse droids that ~inject drugs into you~ snipe you...for now
	MUTATOR_BOUNCYSANDCREATURES, // TODO: you become a small, upside down, bouncy sand creature; you have to eat the enemies from above by landing on them
	MUTATOR_SLIPPERYSLOPE,		 // you keep stepping on one big banana peel
	MUTATOR_SECONDPERSONCAM,	 // experience the world from the eyes of every NPC watching you
	MUTATOR_PREDATOR,			 // all enemies are cloaked
	MUTATOR_TYRANT,				 // turn desann into a tyrant from RE: slow anims, only walks, stomp sound
	MUTATOR_H̸̜́Ḛ̸̀_̵̯́C̷̯͘Õ̵͖M̷̩̂Ḙ̵̅S̸͚̉,			 // H̸̖̿Ę̴͗ ̴͇͘C̷̩̓O̶̼̓M̵̯̄Ë̵͎S̴̲̓
	NUM_MUTATORS
};

struct mutatorState_t {
	mutator_e activeMutator;
	struct {
		int current;
		int next;
	} time;
};

extern const stringID_table_t mutatorStrings[NUM_MUTATORS + 1];

extern mutator_e BG_GetCurrentMutator(void);
