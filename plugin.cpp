#include <ISvenModAPI.h>
#include <interface.h>

#include <hl_sdk/common/protocol.h>

#include "plugin.h"
#include "patterns.h"

#include "convar.h"
#include "dbg.h"
#include "messagebuffer.h"

CTwlzFakeHack g_TwlzFakeHack;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CTwlzFakeHack, IClientPlugin, CLIENT_PLUGIN_INTERFACE_VERSION, g_TwlzFakeHack);

static char speedhack_cvar_name[] = "sc_speedhack";
static char speedhack_cvar_value[] = "69420";
CMessageBuffer CvarValueBuffer;
sizebuf_t *clc_buffer;

CON_COMMAND(twlz_fake_hack, "Trigger anti-cheat detection") {
	CNetMessageParams *params = Utils()->GetNetMessageParams();
	CvarValueBuffer.Init(params->buffer, params->readcount, params->badread);

	CMessageBuffer ClientToServerBuffer;
	ClientToServerBuffer.Init(clc_buffer);

	if (g_pClient->IsConnected()) {
		ClientToServerBuffer.WriteByte(CLC_REQUESTCVARVALUE2);
		ClientToServerBuffer.WriteLong(1337);
		ClientToServerBuffer.WriteString(speedhack_cvar_name);
		ClientToServerBuffer.WriteString(speedhack_cvar_value);

		Utils()->ApplyReadToNetMessageBuffer(&CvarValueBuffer);

		ConMsg("[Twlz Fake Hack] Operation completed.\n");
	} else {
		ConMsg("[Twlz Fake Hack] Not connected to a server.\n");
	}
}

api_version_t CTwlzFakeHack::GetAPIVersion() {
	return SVENMOD_API_VER;
}

bool CTwlzFakeHack::Load(CreateInterfaceFn pfnSvenModFactory, ISvenModAPI* pSvenModAPI, IPluginHelpers *pPluginHelpers) {
	BindApiToGlobals(pSvenModAPI);

	P_CL_Send_CvarValue2 = MemoryUtils()->FindPattern(SvenModAPI()->Modules()->Hardware, Patterns::Hardware::CL_Send_CvarValue2);
	void *m_pclc_buffer = MemoryUtils()->FindPattern(SvenModAPI()->Modules()->Hardware, Patterns::Hardware::clc_buffer);

	clc_buffer = *reinterpret_cast<sizebuf_t**>((unsigned char*)m_pclc_buffer + 1);

	ConVar_Register();

	return true;
}

void CTwlzFakeHack::PostLoad(bool) {
}

void CTwlzFakeHack::Unload(void) {
	ConVar_Unregister();
}

bool CTwlzFakeHack::Pause(void) {
	return true;
}

void CTwlzFakeHack::Unpause(void) {
}

void CTwlzFakeHack::OnFirstClientdataReceived(client_data_t *pcldata, float flTime) {
}

void CTwlzFakeHack::OnBeginLoading(void) {
}

void CTwlzFakeHack::OnEndLoading(void) {
}

void CTwlzFakeHack::OnDisconnect(void) {
}

void CTwlzFakeHack::GameFrame(client_state_t state, double frametime, bool bPostRunCmd) {
}

void CTwlzFakeHack::Draw(void) {
}

void CTwlzFakeHack::DrawHUD(float time, int intermission) {
}

const char* CTwlzFakeHack::GetName(void) {
	return "Twlz Fake Hack";
}

const char* CTwlzFakeHack::GetAuthor(void) {
	return "Steve (Noxturnix)";
}

const char* CTwlzFakeHack::GetVersion(void) {
	return "1.0.0";
}

const char* CTwlzFakeHack::GetDescription(void) {
	return "Force anti-cheat detection on Twlz";
}

const char* CTwlzFakeHack::GetURL(void) {
	return "https://github.com/Noxturnix/twlz_fake_hack";
}

const char* CTwlzFakeHack::GetDate(void) {
	return SVENMOD_BUILD_TIMESTAMP;
}

const char* CTwlzFakeHack::GetLogTag(void) {
	return "TWLZ_FAKE_HACK";
}