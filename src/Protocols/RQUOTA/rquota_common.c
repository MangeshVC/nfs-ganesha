/*
 * Copyright CEA/DAM/DIF  2016
 *  Author: Philippe Deniel (philippe.deniel@cea.fr)
 *
 * --------------------------
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#include <stdio.h>
#include <string.h>
#include "log.h"
#include "export_mgr.h"

/**
 * Check if leading slash is missing, if yes then prepend
 * root path to the pathname
 */
char *check_handle_lead_slash(char *quota_path, char *temp_path,
			      size_t temp_path_size)
{
	if (quota_path[0] != '/') {
		/* prepend root path */
		struct gsh_export *exp;
		int pathlen;
		int qpathlen;

		exp = get_gsh_export(0);
		pathlen = strlen(exp->fullpath);
		if (pathlen >= temp_path_size)
			return NULL;
		memcpy(temp_path, exp->fullpath, pathlen);

		/* Add trailing slash if it is missing */
		if ((pathlen > 0) &&
		    (temp_path[pathlen - 1] != '/'))
			temp_path[pathlen++] = '/';

		qpathlen = strlen(quota_path);
		if ((pathlen + qpathlen) >= temp_path_size) {
			LogInfo(COMPONENT_NFSPROTO,
				"Quota path %s too long", quota_path);
			return NULL;
		}
		memcpy(temp_path + pathlen, quota_path, qpathlen + 1);
		return temp_path;
	} else {
		return quota_path;
	}
}
