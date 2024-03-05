/*
 *  prefs.cpp - Preferences handling
 *
 *  SIDPlayer (C) Copyright 1996-2004 Christian Bauer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "sys.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "prefs.h"


// Prefs items are stored in a linked list of these nodes
typedef struct prefs_node prefs_node;
struct prefs_node {
    prefs_node *next;
    const char *name;
    prefs_type type;
    void *data;
    prefs_desc *desc;
};

// List of prefs nodes
static prefs_node *the_prefs = NULL;

// Prototypes
static prefs_desc *find_prefs_desc(const char *name, prefs_desc *list);


/*
 *  Initialize preferences
 */

void PrefsInit(int argc, char **argv)
{
    int i, j;
    
    // Set defaults
    AddPrefsDefaults();

    // Override prefs with command line options
    for (i=1; i<argc; i++) {

        // Options are of the form '--keyword'
        const char *option = argv[i];
        if (strlen(option) < 3 || option[0] != '-' || option[1] != '-')
            continue;
        const char *keyword = option + 2;

        // Find descriptor for keyword
        const prefs_desc *d = find_prefs_desc(keyword, common_prefs_items);
        if (d == NULL)
            continue;
        argv[i] = NULL;

        // Get value
        i++;
        if (i >= argc) {
            fprintf(stderr, "Option '%s' must be followed by a value\n", option);
            continue;
        }
        const char *value = argv[i];
        argv[i] = NULL;

        // Add/replace prefs item
        switch (d->type) {
            case TYPE_STRING:
                if (d->multiple)
                    PrefsAddString(keyword, value);
                else
                    PrefsReplaceString(keyword, value, 0);
                break;

            case TYPE_BOOLEAN: {
                if (!strcmp(value, "true") || !strcmp(value, "on") || !strcmp(value, "yes"))
                    PrefsReplaceBool(keyword, true);
                else if (!strcmp(value, "false") || !strcmp(value, "off") || !strcmp(value, "no"))
                    PrefsReplaceBool(keyword, false);
                else
                    fprintf(stderr, "Value for option '%s' must be 'true' or 'false'\n", option);
                break;
            }

            case TYPE_INT32:
                PrefsReplaceInt32(keyword, atoi(value));
                break;

            default:
                break;
        }
    }

    // Remove processed arguments
    for (i=1; i<argc; i++) {
        int k;
        for (k=i; k<argc; k++)
            if (argv[k] != NULL)
                break;
        if (k > i) {
            k -= i;
            for (j=i+k; j<argc; j++)
                argv[j-k] = argv[j];
            argc -= k;
        }
    }
}


/*
 *  Deinitialize preferences
 */

void PrefsExit()
{
    // Free prefs list
    prefs_node *p = the_prefs, *next;
    while (p) {
        next = p->next;
        free((void *)p->name);
        free(p->data);
        free(p);
        p = next;
    }
}


/*
 *  Print preferences options help
 */

static void print_options(const prefs_desc *list)
{
    while (list->type != TYPE_END) {
        if (list->help) {
            const char *typestr, *defstr;
            char numstr[32];
            switch (list->type) {
                case TYPE_STRING:
                    typestr = "STRING";
                    defstr = PrefsFindString(list->name, 0);
                    if (defstr == NULL)
                        defstr = "none";
                    break;
                case TYPE_BOOLEAN:
                    typestr = "BOOL";
                    if (PrefsFindBool(list->name))
                        defstr = "true";
                    else
                        defstr = "false";
                    break;
                case TYPE_INT32:
                    typestr = "NUMBER";
                    sprintf(numstr, "%d", PrefsFindInt32(list->name));
                    defstr = numstr;
                    break;
                default:
                    typestr = "<unknown>";
                    defstr = "none";
                    break;
            }
            printf("  --%s %s\n    %s [default=%s]\n", list->name, typestr, list->help, defstr);
        }
        list++;
    }
}

void PrefsPrintUsage()
{
    printf("\nGeneral options:\n");
    print_options(common_prefs_items);
    printf("\nBoolean options are specified as '--OPTION true|on|yes' or\n'--OPTION false|off|no'.\n");
}


/*
 *  Find preferences descriptor by keyword
 */

static prefs_desc *find_prefs_desc(const char *name, prefs_desc *list)
{
    while (list->type != TYPE_END) {
        if (strcmp(list->name, name) == 0)
            return list;
        list++;
    }
    return NULL;
}

/*
 *  Set prefs items
 */

static void add_data(const char *name, prefs_type type, void *data, int size)
{
    void *d = malloc(size);
    if (d == NULL)
        return;
    memcpy(d, data, size);
    prefs_node *p = malloc(sizeof(prefs_node));
    p->next = 0;
    p->name = strdup(name);
    p->type = type;
    p->data = d;
    p->desc = find_prefs_desc(p->name, common_prefs_items);
    if (the_prefs) {
        prefs_node *prev = the_prefs;
        while (prev->next)
            prev = prev->next;
        prev->next = p;
    } else
        the_prefs = p;
}

void PrefsAddString(const char *name, const char *s)
{
    add_data(name, TYPE_STRING, (void *)s, strlen(s) + 1);
}

void PrefsAddBool(const char *name, bool b)
{
    add_data(name, TYPE_BOOLEAN, &b, sizeof(bool));
}

void PrefsAddInt32(const char *name, int32 val)
{
    add_data(name, TYPE_INT32, &val, sizeof(int32));
}


/*
 *  Replace prefs items
 */

static prefs_node *find_node(const char *name, prefs_type type, int index)
{
    prefs_node *p = the_prefs;
    int i = 0;
    while (p) {
        if ((type == TYPE_ANY || p->type == type) && !strcmp(p->name, name)) {
            if (i == index)
                return p;
            else
                i++;
        }
        p = p->next;
    }
    return NULL;
}

void PrefsReplaceString(const char *name, const char *s, int index)
{
    prefs_node *p = find_node(name, TYPE_STRING, index);
    if (p) {
        if (p->desc && p->desc->func)
            ((prefs_func_string)(p->desc->func))(name, (const char *)p->data, s);
        free(p->data);
        p->data = strdup(s);
    } else
        add_data(name, TYPE_STRING, (void *)s, strlen(s) + 1);
}

void PrefsReplaceBool(const char *name, bool b)
{
    prefs_node *p = find_node(name, TYPE_BOOLEAN, 0);
    if (p) {
        if (p->desc && p->desc->func)
            ((prefs_func_bool)(p->desc->func))(name, *(bool *)(p->data), b);
        *(bool *)(p->data) = b;
    } else
        add_data(name, TYPE_BOOLEAN, &b, sizeof(bool));
}

void PrefsReplaceInt32(const char *name, int32 val)
{
    prefs_node *p = find_node(name, TYPE_INT32, 0);
    if (p) {
        if (p->desc && p->desc->func)
            ((prefs_func_int32)(p->desc->func))(name, *(int32 *)(p->data), val);
        *(int32 *)(p->data) = val;
    } else
        add_data(name, TYPE_INT32, &val, sizeof(int32));
}


/*
 *  Get prefs items
 */

const char *PrefsFindString(const char *name, int index)
{
    prefs_node *p = find_node(name, TYPE_STRING, index);
    if (p)
        return (char *)(p->data);
    else
        return NULL;
}

bool PrefsFindBool(const char *name)
{
    prefs_node *p = find_node(name, TYPE_BOOLEAN, 0);
    if (p)
        return *(bool *)(p->data);
    else
        return false;
}

int32 PrefsFindInt32(const char *name)
{
    prefs_node *p = find_node(name, TYPE_INT32, 0);
    if (p)
        return *(int32 *)(p->data);
    else
        return 0;
}


/*
 *  Remove prefs items
 */

void PrefsRemoveItem(const char *name, int index)
{
    prefs_node *p = find_node(name, TYPE_ANY, index);
    if (p) {
        free((void *)p->name);
        free(p->data);
        prefs_node *q = the_prefs;
        if (q == p) {
            the_prefs = NULL;
            free(p);
            return;
        }
        while (q) {
            if (q->next == p) {
                q->next = p->next;
                free(p);
                return;
            }
            q = q->next;
        }
    }
}


/*
 *  Set prefs change callback function
 */

static void set_callback(const char *name, prefs_func f)
{
    prefs_desc *d = find_prefs_desc(name, common_prefs_items);
    if (d == NULL)
        return;
    d->func = f;
}

void PrefsSetCallbackString(const char *name, prefs_func_string f)
{
    set_callback(name, (prefs_func)f);
}

void PrefsSetCallbackBool(const char *name, prefs_func_bool f)
{
    set_callback(name, (prefs_func)f);
}

void PrefsSetCallbackInt32(const char *name, prefs_func_int32 f)
{
    set_callback(name, (prefs_func)f);
}
