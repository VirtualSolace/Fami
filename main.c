#include "../headers/finder.h"
#include "../headers/writer.h"
#include "../headers/info.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FileList *list = (FileList*)malloc(sizeof(FileList));
    list->file = NULL;
    list->count = 0;
    list->capacity = 0;

    int result = finder(list, "/");
    if (!result) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list->count; i++) {
        writer(list); // or writer(&list->file[i]) depending on design
    }

    for (size_t i = 0; i < list->count; i++) {
        free(list->file[i].path);
    }
    free(list->file);
    free(list);

    return EXIT_SUCCESS;
}
