#include "csv.h"

struct hotspot *parse_hotspot_list(const char *hotspots_filename)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(hotspots_filename, "r");

    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int return_code = getline(&line, &len, fp);
    if (return_code == -1)
    {
        exit(EXIT_FAILURE);
    }

    char *token;
    struct hotspot *last_hotspot = NULL;
    struct hotspot *new_hotspot = NULL;
    struct hotspot *hotspot_list = NULL;

    while (getline(&line, &len, fp) != -1)
    {
        if (last_hotspot == NULL)
        {
            hotspot_list = malloc(sizeof(struct hotspot));
            new_hotspot = hotspot_list;
        }
        else
        {
            new_hotspot = malloc(sizeof(struct hotspot));
            last_hotspot->next_hotspot = new_hotspot;
        }
        token = strtok(line, ",");
        new_hotspot->x = atoi(token);
        token = strtok(NULL, ",");
        new_hotspot->y = atoi(token);
        token = strtok(NULL, ",");
        new_hotspot->startround = atoi(token);
        token = strtok(NULL, ",");
        new_hotspot->endround = atoi(token);

        last_hotspot = new_hotspot;
    }

    fclose(fp);
    if (line)
        free(line);

    return hotspot_list;
}
