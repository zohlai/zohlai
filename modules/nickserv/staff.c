/*
 * Copyright (c) 2005 Atheme Development Group
 * Copyright (c) 2014 Max Teufel
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Adds a flag to indicate that the account belongs to a staffer.
 *
 */

#include "atheme.h"
#include "list_common.h"
#include "list.h"

DECLARE_MODULE_V1
(
	"nickserv/staff", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Zohlai Development Group"
);

static void ns_cmd_staff(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_staff = { "STAFF", N_("Adds a flag to indicate that the account belongs to a staffer."),
		      PRIV_GRANT, 2, ns_cmd_staff, { .path = "nickserv/staff" } };

static bool is_staff(const mynick_t *mn, const void *arg) {
	myuser_t *mu = mn->owner;

	return !!metadata_find(mu, "private:staff:setter");
}

void _modinit(module_t *m)
{
	service_named_bind_command("nickserv", &ns_staff);

	use_nslist_main_symbols(m);

	static list_param_t staff;
	staff.opttype = OPT_BOOL;
	staff.is_match = is_staff;

	list_register("staff", &staff);
}

void _moddeinit(module_unload_intent_t intent)
{
	service_named_unbind_command("nickserv", &ns_staff);

	list_unregister("staff");
}

static void ns_cmd_staff(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	char *action = parv[1];
	myuser_t *mu;

	if (!target || !action)
	{
		command_fail(si, fault_needmoreparams, STR_INSUFFICIENT_PARAMS, "STAFF");
		command_fail(si, fault_needmoreparams, _("Usage: STAFF <account> <ON|OFF>"));
		return;
	}

	if (!(mu = myuser_find_ext(target)))
	{
		command_fail(si, fault_nosuch_target, _("\2%s\2 is not registered."), target);
		return;
	}

	if (!strcasecmp(action, "ON"))
	{
		if (metadata_find(mu, "private:staff:setter"))
		{
			command_fail(si, fault_badparams, _("\2%s\2 is already a member of staff."), entity(mu)->name);
			return;
		}

		metadata_add(mu, "private:staff:setter", get_oper_name(si));
		metadata_add(mu, "private:staff:timestamp", number_to_string(CURRTIME));

		wallops("%s set the STAFF option for the account \2%s\2.", get_oper_name(si), entity(mu)->name);
		logcommand(si, CMDLOG_ADMIN, "STAFF:ON: \2%s\2", entity(mu)->name);
		command_success_nodata(si, _("\2%s\2 is now a member of staff."), entity(mu)->name);
	}
	else if (!strcasecmp(action, "OFF"))
	{
		if (!metadata_find(mu, "private:staff:setter"))
		{
			command_fail(si, fault_badparams, _("\2%s\2 is not a member of staff."), entity(mu)->name);
			return;
		}

		metadata_delete(mu, "private:staff:setter");
		metadata_delete(mu, "private:staff:timestamp");

		wallops("%s removed the STAFF option on the account \2%s\2.", get_oper_name(si), entity(mu)->name);
		logcommand(si, CMDLOG_ADMIN, "STAFF:OFF: \2%s\2", entity(mu)->name);
		command_success_nodata(si, _("\2%s\2 is no longer a member of staff."), entity(mu)->name);
	}
	else
	{
		command_fail(si, fault_needmoreparams, STR_INVALID_PARAMS, "STAFF");
		command_fail(si, fault_needmoreparams, _("Usage: STAFF <account> <ON|OFF>"));
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
