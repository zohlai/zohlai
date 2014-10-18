/*
 * Copyright (c) 2014 Zohlai Development Group
 * FNC code from bcode/ilbelkyr.
 *
 * Tools to debug some IRCd features:
 *   * /os fnc - forces a nick change.
 *   * /os ffnc - same as fnc but doesn't check whether the target nick already exists.
 *   * /os tempvhost - temporarily changes the host of a user.
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"operserv/debug", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Zohlai Development Group"
);

static void os_cmd_fnc(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_ffnc(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_tempvhost(sourceinfo_t *si, int parc, char *parv[]);

command_t os_fnc = { "FNC", N_("Force a nick change."), PRIV_USER_ADMIN, 2, os_cmd_fnc, { .path = "operserv/fnc" } };
command_t os_ffnc = { "FFNC", N_("Force a nick change, regaining the new nick if already in use."), PRIV_USER_ADMIN, 2, os_cmd_ffnc, { .path = "operserv/ffnc" } };
command_t os_tempvhost = { "TEMPVHOST", N_("Changes the host of a user temporarily."), PRIV_USER_VHOST, 4, os_cmd_tempvhost, { .path = "operserv/tempvhost" } };

void _modinit(module_t *m)
{
	service_named_bind_command("operserv", &os_fnc);
	service_named_bind_command("operserv", &os_ffnc);
	service_named_bind_command("operserv", &os_tempvhost);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("operserv", &os_fnc);
	service_named_unbind_command("operserv", &os_ffnc);
	service_named_unbind_command("operserv", &os_tempvhost);
}

static void os_cmd_fnc(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *nick = parv[1];
	user_t *u, *v;
	service_t *svs = service_find("operserv");

	if(!target || !nick)
	{
		command_fail(si, fault_badparams, "Usage: \2FNC\2 <target> <nick>");
		return;
	}

	if(!(u = user_find_named(target)))
	{
		command_fail(si, fault_nosuch_target, "\2%s\2 is not on the network", target);
		return;
	}

	v = user_find_named(nick);
	if(v && v != u)
	{
		command_fail(si, fault_noprivs, "\2%s\2 is already on the network", nick);
		return;
	}

	fnc_sts(svs->me, u, nick, FNC_FORCE);
	command_success_nodata(si, "A forced nick change from \2%s\2 to \2%s\2 has been sent.", target, nick);
	logcommand(si, CMDLOG_ADMIN, "FNC: \2%s\2 -> \2%s\2", target, nick);
}

static void os_cmd_ffnc(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *nick = parv[1];
	user_t *u, *v;
	service_t *svs = service_find("operserv");

	if(!target || !nick)
	{
		command_fail(si, fault_badparams, "Usage: \2FFNC\2 <target> <nick>");
		return;
	}

	if(!(u = user_find_named(target)))
	{
		command_fail(si, fault_nosuch_target, "\2%s\2 is not on the network", target);
		return;
	}

	v = user_find_named(nick);
	if(v && v != u)
	{
		fnc_sts(svs->me, u, nick, FNC_FORCE);
		command_success_nodata(si, "A forced nick regain from \2%s\2 to \2%s\2 has been sent.", target, nick);
		logcommand(si, CMDLOG_ADMIN, "FFNC: \2%s\2 -> \2%s\2 (regain)", target, nick);
	}
	else {
		fnc_sts(svs->me, u, nick, FNC_FORCE);
		command_success_nodata(si, "A forced nick change from \2%s\2 to \2%s\2 has been sent.", target, nick);
		logcommand(si, CMDLOG_ADMIN, "FFNC: \2%s\2 -> \2%s\2", target, nick);
	}
}

static void os_cmd_tempvhost(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *newvhost = parv[1];
	user_t *u;

	if(!target || !newvhost)
	{
		command_fail(si, fault_badparams, "Usage: \2TEMPVHOST\2 <target> <newvhost>");
		return;
	}

	if(!(u = user_find_named(target)))
	{
		command_fail(si, fault_nosuch_target, "\2%s\2 is not on the network", target);
		return;
	}

	if (!check_vhost_validity(si, newvhost))
		return;

	user_sethost(nicksvs.me->me, u, newvhost);
	command_success_nodata(si, "The host of \2%s\2 has been changed to \2%s\2.", target, newvhost);
	logcommand(si, CMDLOG_ADMIN, "VHOST:TEMP: \2%s\2 to \2%s\2", newvhost, target);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
