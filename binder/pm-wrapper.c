#include "../utils.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utils/Log.h>

#define PACKAGE_LIST_PATH "/data/system/packages.list"
#define PACKAGE_NAME_MAX_LEN (1<<16)

/* Tries to resolve a package name from a uid via the packages list file.
 *
 * If there is no matching uid, it will return an empty string which can
 * be resolved by appops in some cases (i.e. apps with uid = 0, uid = AID_SHELL).
 *
 * Since packages may share UID, this function will return the first present
 * in packages.list.
 */
const char* resolve_package_name(int uid) {
    char *packages = read_file(PACKAGE_LIST_PATH);

    if (packages == NULL) {
        goto notfound;
    }

    char *p = packages;
    while (*p) {
        char *line_end = strstr(p, "\n");
        if (line_end == NULL)
            break;

        char *token;
        char *pkgName = strtok_r(p, " ", &token);
        if (pkgName != NULL) {
            char *pkgUid = strtok_r(NULL, " ", &token);
            if (pkgUid != NULL) {
                char *endptr;
                errno = 0;
                int pkgUidInt = strtoul(pkgUid, &endptr, 10);
                if ((errno == 0 && endptr != NULL && !(*endptr)) && pkgUidInt == uid)
                    return strdup(pkgName);
            }
        }
        p = ++line_end;
    }
    free(packages);

notfound:
    return "";
}
