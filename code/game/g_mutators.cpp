#include <array>

#include "bg_mutators.h"
#include "g_local.h"
#include "g_mutators.h"
#include "g_shared.h"
#include "g_functions.h"
#include "qcommon/q_math.h"
#include "qcommon/q_shared.h"
#include "qcommon/q_string.h"
#include "teams.h"

static const std::vector<mutator_e> allowedMutators = {
	// clang-format off
	MUTATOR_NONE,
	MUTATOR_FILMNOIR,
	MUTATOR_JAWAINVASION,
	MUTATOR_PEACEANDQUIET,
	MUTATOR_FAMOUS,
	MUTATOR_ROSHINVASION,
	MUTATOR_SPLIT,
	MUTATOR_DRUNK,
	MUTATOR_GIANTS,
	MUTATOR_INCLEMENTWEATHER,
	MUTATOR_PARANOIA,
	MUTATOR_DICKBUTT,
	MUTATOR_SPINNYCAM,
	MUTATOR_RANDOMBOSSFIGHT,
	MUTATOR_MOUSEBLASTERS,
	MUTATOR_BOUNCYSANDCREATURES,
	MUTATOR_SLIPPERYSLOPE,
	MUTATOR_SECONDPERSONCAM,
	MUTATOR_PREDATOR,
	MUTATOR_TYRANT,
	// clang-format on
};

// pick a random mutator that we're not currently on
static mutator_e SelectRandomMutator() {
	mutator_e result = level.mutators.state.activeMutator;
	while (result == level.mutators.state.activeMutator ||
		   !std::any_of(allowedMutators.cbegin(), allowedMutators.cend(), [&result](const mutator_e &elem) { return elem == result; })) {
		result = (mutator_e)Q_irand(0, NUM_MUTATORS - 1);
	}
	return result;
}

static void Begin_RoshInvasion(void) {
	for (auto &ent : g_entities) {
		if (ent.inuse && ent.s.eType == ET_PLAYER && ent.NPC && ent.client->NPC_class != CLASS_VEHICLE) {
			G_RemovePlayerModel(&ent);
			G_SetG2PlayerModel(&ent, "rosh_penin", NULL, NULL, NULL);
			NPC_SetAnim(&ent, SETANIM_LEGS, ent.client->ps.legsAnim, SETANIM_FLAG_NORMAL | SETANIM_FLAG_RESTART, SETANIM_BLEND_DEFAULT);
			NPC_SetAnim(&ent, SETANIM_TORSO, ent.client->ps.torsoAnim, SETANIM_FLAG_NORMAL | SETANIM_FLAG_RESTART, SETANIM_BLEND_DEFAULT);

			// override sounds 🫤 these are gi.Free'd
			ent.client->clientInfo.customBasicSoundDir = G_NewString("rosh");
			ent.client->clientInfo.customCombatSoundDir = G_NewString("rosh");
			ent.client->clientInfo.customExtraSoundDir = G_NewString("rosh");
			ent.client->clientInfo.customJediSoundDir = G_NewString("rosh");

			CG_RegisterNPCCustomSounds(&ent.client->clientInfo);
		}
	}
}

static void End_RoshInvasion(void) {
	for (auto &ent : g_entities) {
		if (ent.inuse && ent.s.eType == ET_PLAYER && ent.NPC && ent.client->NPC_class != CLASS_VEHICLE) {
			G_RemovePlayerModel(&ent);
			NPC_ParseParms(ent.NPC_type, &ent); // wow what a hack...
		}
	}
}

static void Begin_Drunk(void) {
	// save the timescale
	level.mutators.drunk.savedTimescale = g_timescale->value;

	// reduce timescale
	const float drunkTimescale = 0.666666666f;
	gi.cvar_set("timescale", va("%.2f", drunkTimescale));

	// reduce the next mutator time to accommodate for the slower timescale
	level.mutators.state.time.next -= MUTATOR_CYCLE_TIME * drunkTimescale;
}
static void End_Drunk(void) { gi.cvar_set("timescale", va("%.2f", level.mutators.drunk.savedTimescale)); }

static void Begin_Giants(void) {
	for (auto &ent : g_entities) {
		if (ent.inuse && ent.s.eType == ET_PLAYER && ent.NPC && ent.client->NPC_class != CLASS_VEHICLE) {
			ent.mutators.giants.scaleAmount = flrand(3.0f, 8.0f);
			VectorCopy(ent.s.modelScale, ent.mutators.giants.oldModelScale);
			VectorScale(ent.s.modelScale, ent.mutators.giants.scaleAmount, ent.s.modelScale);
			// FIXME: this is wrong 😭
			VectorScale(ent.mins, ent.mutators.giants.scaleAmount, ent.mins);
			VectorScale(ent.maxs, ent.mutators.giants.scaleAmount, ent.maxs);
		}
	}
}
static void End_Giants(void) {
	for (auto &ent : g_entities) {
		if (ent.inuse && ent.s.eType == ET_PLAYER && ent.NPC && ent.client->NPC_class != CLASS_VEHICLE && fabsf(ent.mutators.giants.scaleAmount) > 0.001f) {
			VectorScale(ent.mins, 1.0f / ent.mutators.giants.scaleAmount, ent.mins);
			VectorScale(ent.maxs, 1.0f / ent.mutators.giants.scaleAmount, ent.maxs);
			VectorCopy(ent.mutators.giants.oldModelScale, ent.s.modelScale);
		}
	}
}

static void Begin_InclementWeather(void) {
	// Interesting... the weather patterns here seem natural, not artificial.
	// I wonder if the ring's environment systems are malfunctioning... or if the designers wanted the installation to have inclement weather.

	// FIXME: this should stash current weather configstrings and overwrite them all

	for (int i = CS_WORLD_FX; i < CS_WORLD_FX + MAX_WORLD_FX; i++) {
		// FIXME: check if the CS is set, then reset to "clear". apparently "" is not valid 🤷
		gi.SetConfigstring(i, "");
	}

	level.mutators.spawningEnts = true;
	gentity_t *spawned = G_Spawn();
	spawned->classname = G_NewString("fx_rain");
	spawned->spawnflags |= 4 | 64; // heavy + lightning
	if (!G_CallSpawn(spawned)) {
		G_FreeEntity(spawned);
	}
	spawned->mutators.inclementWeather.spawnedBy = true;
	level.mutators.spawningEnts = false;

	const char *weatherEffects[] = {
		"acidrain",
		"constantwind ( 300 300 300 )", // FIXME: randomise
		"fog",
		"gustingwind",
		"heavyrain",
		"heavyrainfog",
		"light_fog",
		"lightrain",
		"rain",
		"sand",
		"snow",
		"spacedust",
		"wind",
		"outsidepain", // this actually turns it OFF (acidraid turns it on)
		"outsideshake",
	};
	for (const auto &weatherEffect : weatherEffects) {
		G_FindConfigstringIndex(weatherEffect, CS_WORLD_FX, MAX_WORLD_FX, qtrue);
	}
}
static void End_InclementWeather(void) {
	for (auto &ent : g_entities) {
		if (/*!Q_stricmpn(ent.classname, "fx_", 3) &&*/ ent.mutators.inclementWeather.spawnedBy) {
			G_FreeEntity(&ent);
		}
	}
	for (int i = CS_WORLD_FX; i < CS_WORLD_FX + MAX_WORLD_FX; i++) {
		// FIXME: check if the CS is set, then reset to "clear". apparently "" is not valid 🤷
		gi.SetConfigstring(i, "");
	}
	G_FindConfigstringIndex("outsideshake", CS_WORLD_FX, MAX_WORLD_FX, qtrue);
	// G_FindConfigstringIndex("outsidepain", CS_WORLD_FX, MAX_WORLD_FX, qtrue);
	G_FindConfigstringIndex("constantwind ( 0 0 0 )", CS_WORLD_FX, MAX_WORLD_FX, qtrue);
	G_FindConfigstringIndex("clear", CS_WORLD_FX, MAX_WORLD_FX, qtrue);
}

static void Begin_RandomBossFight(void) {
	// spawn them in front of us
	const gentity_t &self = g_entities[0];
	vec3_t fwd;
	AngleVectors(self.client->ps.viewangles, fwd, nullptr, nullptr);
	fwd[2] = 0.0f;

	vec3_t targetPos;
	VectorMA(self.currentOrigin, 128.0f, fwd, targetPos);

	trace_t tr;
	gi.trace(&tr, self.currentOrigin, playerMins, playerMaxs, targetPos, self.s.number, self.clipmask, (EG2_Collision)0, 0);
	if (tr.allsolid) {
		tr.endpos[2] += playerMaxs[2] / 2.0f;
	}

	static const char *bosses[] = {
		"NPC_Alora", "NPC_BobaFett", "NPC_Desann", "NPC_Rax", "NPC_Tavion_New", "NPC_Tavion",
	};
	static const size_t numBosses = ARRAY_LEN(bosses);
	const int numBossesToFight = Q_irand(1, 3);
	for (int i = 0; i < numBossesToFight; i++) {
		auto &classname = bosses[Q_irand(0, numBosses - 1)];
		gentity_t *spawned = G_Spawn();
		spawned->classname = G_NewString(classname);

		G_SetOrigin(spawned, tr.endpos);
		VectorCopy(spawned->currentOrigin, spawned->s.origin);

		if (!G_CallSpawn(spawned)) {
			G_FreeEntity(spawned);
			continue;
		}
	}
}

static void Begin_BouncySandCreatures(void) {
	auto &self = g_entities[0];
	G_ChangePlayerModel(&self, "sand_creature_bouncy");

	const float scale = 0.2f;
	VectorSet(self.s.modelScale, scale, scale, scale);

	// override sounds 🫤 these are gi.Free'd
	if (self.client->clientInfo.customBasicSoundDir) {
		Q_strncpyz(self.mutators.bouncysandcreatures.customBasicSoundDir, self.client->clientInfo.customBasicSoundDir,
				   sizeof(self.mutators.bouncysandcreatures.customBasicSoundDir));
		gi.Free(self.client->clientInfo.customBasicSoundDir);
	} else {
		self.mutators.bouncysandcreatures.customBasicSoundDir[0] = '\0';
	}
	self.client->clientInfo.customBasicSoundDir = G_NewString("sand_creature");

	if (self.client->clientInfo.customCombatSoundDir) {
		Q_strncpyz(self.mutators.bouncysandcreatures.customCombatSoundDir, self.client->clientInfo.customCombatSoundDir,
				   sizeof(self.mutators.bouncysandcreatures.customCombatSoundDir));
		gi.Free(self.client->clientInfo.customCombatSoundDir);
	} else {
		self.mutators.bouncysandcreatures.customCombatSoundDir[0] = '\0';
	}
	self.client->clientInfo.customCombatSoundDir = G_NewString("sand_creature");

	if (self.client->clientInfo.customExtraSoundDir) {
		Q_strncpyz(self.mutators.bouncysandcreatures.customExtraSoundDir, self.client->clientInfo.customExtraSoundDir,
				   sizeof(self.mutators.bouncysandcreatures.customExtraSoundDir));
		gi.Free(self.client->clientInfo.customExtraSoundDir);
	} else {
		self.mutators.bouncysandcreatures.customExtraSoundDir[0] = '\0';
	}
	self.client->clientInfo.customExtraSoundDir = G_NewString("sand_creature");

	if (self.client->clientInfo.customJediSoundDir) {
		Q_strncpyz(self.mutators.bouncysandcreatures.customJediSoundDir, self.client->clientInfo.customJediSoundDir,
				   sizeof(self.mutators.bouncysandcreatures.customJediSoundDir));
		gi.Free(self.client->clientInfo.customJediSoundDir);
	} else {
		self.mutators.bouncysandcreatures.customJediSoundDir[0] = '\0';
	}
	self.client->clientInfo.customJediSoundDir = G_NewString("sand_creature");

	CG_RegisterNPCCustomSounds(&self.client->clientInfo);
}

static void End_BouncySandCreatures(void) {
	auto &self = g_entities[0];
	G_ChangePlayerModel(&self, "player");

	const float scale = 1.0f;
	VectorSet(self.s.modelScale, scale, scale, scale);

	if (self.client->clientInfo.customBasicSoundDir) {
		gi.Free(self.client->clientInfo.customBasicSoundDir);
	}
	self.client->clientInfo.customBasicSoundDir = G_NewString(self.mutators.bouncysandcreatures.customBasicSoundDir);
	self.mutators.bouncysandcreatures.customBasicSoundDir[0] = '\0';

	if (self.client->clientInfo.customCombatSoundDir) {
		gi.Free(self.client->clientInfo.customCombatSoundDir);
	}
	self.client->clientInfo.customCombatSoundDir = G_NewString(self.mutators.bouncysandcreatures.customCombatSoundDir);
	self.mutators.bouncysandcreatures.customCombatSoundDir[0] = '\0';

	if (self.client->clientInfo.customExtraSoundDir) {
		gi.Free(self.client->clientInfo.customExtraSoundDir);
	}
	self.client->clientInfo.customExtraSoundDir = G_NewString(self.mutators.bouncysandcreatures.customExtraSoundDir);
	self.mutators.bouncysandcreatures.customExtraSoundDir[0] = '\0';

	if (self.client->clientInfo.customJediSoundDir) {
		gi.Free(self.client->clientInfo.customJediSoundDir);
	}
	self.client->clientInfo.customJediSoundDir = G_NewString(self.mutators.bouncysandcreatures.customJediSoundDir);
	self.mutators.bouncysandcreatures.customJediSoundDir[0] = '\0';

	CG_RegisterNPCCustomSounds(&self.client->clientInfo);
}

static void Begin_Tyrant(void) {
	// spawn them behind us
	const gentity_t &self = g_entities[0];
	vec3_t fwd;
	AngleVectors(self.client->ps.viewangles, fwd, nullptr, nullptr);
	VectorScale(fwd, -1.0f, fwd);
	fwd[2] = 0.0f;

	vec3_t targetPos;
	VectorMA(self.currentOrigin, 256.0f, fwd, targetPos);

	trace_t tr;
	gi.trace(&tr, self.currentOrigin, playerMins, playerMaxs, targetPos, self.s.number, self.clipmask, (EG2_Collision)0, 0);
	if (tr.allsolid) {
		tr.endpos[2] += playerMaxs[2] / 2.0f;
	}

	// now spawn them
	gentity_t *spawned = G_Spawn();
	spawned->classname = G_NewString("NPC_Tyrant");
	G_SetOrigin(spawned, tr.endpos);
	VectorCopy(spawned->currentOrigin, spawned->s.origin);
	if (!G_CallSpawn(spawned)) {
		G_FreeEntity(spawned);
	}
	for (auto &ent : g_entities) {
		if (!ent.inuse || !ent.NPC || ent.client->NPC_class != CLASS_DESANN) {
			continue;
		}
		if (strcmp(ent.NPC_type, "tyrant")) {
			continue;
		}
		ent.NPC->aiFlags |= NPCAI_WALKING;
		ent.NPC->scriptFlags |= SCF_WALKING | SCF_NO_ACROBATICS | SCF_NO_COMBAT_TALK | SCF_DONT_FLEE;
	}
}

static const struct mutatorFuncs_t {
	void (*begin)(void);
	void (*think)(void);
	void (*end)(void);
} mutatorFuncs[NUM_MUTATORS] = {
	{nullptr, nullptr, nullptr},								   // MUTATOR_NONE
	{nullptr, nullptr, nullptr},								   // MUTATOR_FILMNOIR
	{nullptr, nullptr, nullptr},								   // MUTATOR_JAWAINVASION
	{nullptr, nullptr, nullptr},								   // MUTATOR_PEACEANDQUIET
	{nullptr, nullptr, nullptr},								   // MUTATOR_FAMOUS
	{Begin_RoshInvasion, nullptr, End_RoshInvasion},			   // MUTATOR_ROSHINVASION
	{nullptr, nullptr, nullptr},								   // MUTATOR_SPLIT
	{Begin_Drunk, nullptr, End_Drunk},							   // MUTATOR_DRUNK
	{Begin_Giants, nullptr, End_Giants},						   // MUTATOR_GIANTS
	{Begin_InclementWeather, nullptr, End_InclementWeather},	   // MUTATOR_INCLEMENTWEATHER
	{nullptr, nullptr, nullptr},								   // MUTATOR_PARANOIA
	{nullptr, nullptr, nullptr},								   // MUTATOR_DICKBUTT
	{nullptr, nullptr, nullptr},								   // MUTATOR_SPINNYCAM
	{Begin_RandomBossFight, nullptr, nullptr},					   // MUTATOR_RANDOMBOSSFIGHT
	{nullptr, nullptr, nullptr},								   // MUTATOR_MOUSEBLASTERS
	{Begin_BouncySandCreatures, nullptr, End_BouncySandCreatures}, // MUTATOR_BOUNCYSANDCREATURES
	{nullptr, nullptr, nullptr},								   // MUTATOR_SLIPPERYSLOPE
	{nullptr, nullptr, nullptr},								   // MUTATOR_SECONDPERSONCAM
	{nullptr, nullptr, nullptr},								   // MUTATOR_PREDATOR
	{Begin_Tyrant, nullptr, nullptr},							   // MUTATOR_TYRANT
};

void Mutator_CheckUpdate() {
	if (level.mutators.state.time.next > level.time) {
		if (mutatorFuncs[level.mutators.state.activeMutator].think) {
			mutatorFuncs[level.mutators.state.activeMutator].think();
		}
		return;
	}

	if (mutatorFuncs[level.mutators.state.activeMutator].end) {
		mutatorFuncs[level.mutators.state.activeMutator].end();
	}

	level.mutators.state.activeMutator = SelectRandomMutator();
	level.mutators.state.time.current = level.time;
	level.mutators.state.time.next = level.mutators.state.time.current + MUTATOR_CYCLE_TIME;

	if (mutatorFuncs[level.mutators.state.activeMutator].begin) {
		mutatorFuncs[level.mutators.state.activeMutator].begin();
	}

	// this is networked to the client
	gi.cvar_set("mutator", va("%i:%i:%i", level.mutators.state.activeMutator, level.mutators.state.time.current, level.mutators.state.time.next));
}
