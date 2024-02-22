#include "cgame/cg_local.h"
#include "cgame/cg_media.h"
#include "../game/bg_mutators.h"
#include "g_local.h"
#include "qcommon/q_math.h"
#include "qcommon/q_shared.h"
#include "teams.h"

static void CG_RenderMeterHorizonal(float x, float y, float w, float h, float minValue, float maxValue, const vec4_t colour, const vec4_t background,
									bool flipped) {
	CG_DrawRect(x, y, w, h, 1.0f, background);

	if (minValue > 0) {
		float consume = minValue;
		while (consume > 0.0f) {
			float percent = (consume / maxValue) * w;
			if (consume > maxValue) {
				percent = w;
			}
			float padding = 2.0f;
			// then fill it
			if (flipped) {
				x = x + (w - percent);
			}
			CG_DrawRect(x + padding, y + padding, w - (w - percent) - (padding * 2), h - (padding * 2), 1.0f, colour);
			consume = consume - maxValue;
		}
	}
}

#ifdef _DEBUG
static void CG_DrawMutatorsDebugHUD(void) {
	const gentity_t &self = g_entities[0];
	const float margin = 16.0f;
	const float fontScale = 0.75f;
	char buf[MAX_STRING_CHARS] = {};

	Q_strcat(buf, sizeof(buf), va("cg.time: %i\n", cg.time));
	Q_strcat(buf, sizeof(buf), va("cg.mutators.state.activeMutator: %i\n", cg.mutators.state.activeMutator));
	Q_strcat(buf, sizeof(buf), va("cg.mutators.state.time.current: %i\n", cg.mutators.state.time.current));
	Q_strcat(buf, sizeof(buf), va("cg.mutators.state.time.next: %i\n", cg.mutators.state.time.next));
	Q_strcat(buf, sizeof(buf), va("self.s.modelScale: %s\n", vtos(self.s.modelScale)));

	if (cg.mutators.state.activeMutator == MUTATOR_SECONDPERSONCAM) {
		const int distance =
			(cg.mutators.secondpersoncam.foundEnt == ENTITYNUM_NONE)
				? 0
				: (int)Distance(self.client->renderInfo.eyePoint, g_entities[cg.mutators.secondpersoncam.foundEnt].client->renderInfo.eyePoint);
		Q_strcat(buf, sizeof(buf), va("cg.mutators.secondpersoncam.foundEnt: %i (dist: %i)", cg.mutators.secondpersoncam.foundEnt, distance));
	}
	const int h = cgi_R_Font_HeightPixels(cgs.media.qhFontSmall, fontScale);
	cgi_R_Font_DrawString(margin, margin + h * 6.0f, buf, colorTable[CT_RED], cgs.media.qhFontSmall, -1, fontScale);
}
#endif

void CG_DrawMutatorsHUD(void) {
	if (!cg.snap || !cg.mutators.state.time.current || !cg.mutators.state.time.next) {
		return;
	}

	// current mutator text
	const float margin = 16.0f;
	const float fontScale = 1.0f;
	const char *mutatorStr = GetStringForID(mutatorStrings, cg.mutators.state.activeMutator);
	const int w = cgi_R_Font_StrLenPixels(mutatorStr, cgs.media.qhFontSmall, fontScale);
	const int h = cgi_R_Font_HeightPixels(cgs.media.qhFontSmall, fontScale);
	cgi_R_Font_DrawString((SCREEN_WIDTH / 2.0f) - (w / 2.0f), margin + h, mutatorStr, colorTable[CT_RED], cgs.media.qhFontSmall, -1, fontScale);

	// next mutator countdown meter
	const vec4_t background = {0.0f, 0.0f, 0.0f, 0.1f};
	const int timeElapsed = cg.snap->serverTime - cg.mutators.state.time.current; // hmm, sometimes this is > timeBetween by a couple frames
	const int timeBetween = cg.mutators.state.time.next - cg.mutators.state.time.current;
	const float fracElapsed = (float)Q_min(timeElapsed, timeBetween) / (float)timeBetween;
	assert(fracElapsed >= 0.0f && fracElapsed <= 1.0f);
	const float minValue = fracElapsed * SCREEN_WIDTH;
	const float maxValue = SCREEN_WIDTH;
	CG_RenderMeterHorizonal(0, 0, SCREEN_WIDTH, 8.0f, minValue, maxValue, colorTable[CT_RED], background, false);

#ifdef _DEBUG
	CG_DrawMutatorsDebugHUD();
#endif
}

bool CG_IsTyrant(const centity_t &cent) {
	return cent.gent && cent.gent->s.eType == ET_PLAYER && cent.gent->NPC && cent.gent->client->NPC_class == CLASS_DESANN &&
		   !strcmp(cent.gent->NPC_type, "tyrant");
}
