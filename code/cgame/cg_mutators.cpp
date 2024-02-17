#include "cgame/cg_local.h"
#include "cgame/cg_media.h"
#include "../game/bg_mutators.h"
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

void CG_DrawMutatorsHUD(void) {
	if (!cg.snap || !cg.mutators.state.time.current || !cg.mutators.state.time.next) {
		return;
	}

	// current mutator text
	const float fontScale = 1.0f;
	const char *mutatorStr = GetStringForID(mutatorStrings, cg.mutators.state.activeMutator);
	const int w = cgi_R_Font_StrLenPixels(mutatorStr, cgs.media.qhFontSmall, fontScale);
	const int h = cgi_R_Font_HeightPixels(cgs.media.qhFontSmall, fontScale);
	cgi_R_Font_DrawString((SCREEN_WIDTH / 2.0f) - (w / 2.0f), 16.0f + h, mutatorStr, colorTable[CT_RED], cgs.media.qhFontSmall, -1, fontScale);

	// next mutator countdown meter
	const vec4_t background = {0.0f, 0.0f, 0.0f, 0.1f};
	const int timeElapsed = cg.snap->serverTime - cg.mutators.state.time.current; // hmm, sometimes this is > timeBetween by a couple frames
	const int timeBetween = cg.mutators.state.time.next - cg.mutators.state.time.current;
	const float fracElapsed = (float)Q_min(timeElapsed, timeBetween) / (float)timeBetween;
	assert(fracElapsed >= 0.0f && fracElapsed <= 1.0f);
	const float minValue = fracElapsed * SCREEN_WIDTH;
	const float maxValue = SCREEN_WIDTH;
	CG_RenderMeterHorizonal(0, 0, SCREEN_WIDTH, 8.0f, minValue, maxValue, colorTable[CT_RED], background, false);
}

bool CG_IsTyrant(const centity_t &cent) {
	return cent.gent && cent.gent->s.eType == ET_PLAYER && cent.gent->NPC && cent.gent->client->NPC_class == CLASS_DESANN &&
		   !strcmp(cent.gent->NPC_type, "tyrant");
}
