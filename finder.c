#include "../headers/finder.h"
#include "../headers/info.h"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool finder(FileList *list, const char *dirpath) {
    int dFD = open(dirpath, O_DIRECTORY | O_RDONLY);
    if (dFD < 0) return false;

    struct dirent *entry;
    DIR *dp = fdopendir(dFD);
    if (!dp) {
        close(dFD);
        return false;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Build full path
        size_t pathlen = strlen(dirpath) + strlen(entry->d_name) + 2;
        char *fullpath = malloc(pathlen);
        snprintf(fullpath, pathlen, "%s/%s", dirpath, entry->d_name);

        struct stat st;
        if (fstatat(dFD, entry->d_name, &st, AT_SYMLINK_NOFOLLOW) == 0) {
            if (S_ISDIR(st.st_mode)) {
                // Recurse into directories
                finder(list, fullpath);
            } else if (S_ISREG(st.st_mode)) {
                // Add file to list
                if (list->count == list->capacity) {
                    list->capacity = list->capacity ? list->capacity * 2 : 16;
                    list->file = realloc(list->file, list->capacity * sizeof(File));
                }
                list->file[list->count].path = fullpath;
                list->file[list->count].size = (uint32_t)st.st_size;
                list->count++;
                continue;
            }
        }

        free(fullpath); // free non-files and failed stats
    }

    closedir(dp);
    return true;
}
