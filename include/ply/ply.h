#ifndef _PLY_H
#define _PLY_H

#include <stdio.h>

#include "sym.h"
#include "utils.h"

struct ksyms;
struct ply;
struct node;
struct ir;

/* api */
struct ply_probe {
	struct ply_probe *next, *prev;
	struct ply *ply;

	char *probe;
	struct node *ast;

	struct symtab locals;

	struct provider *provider;
	void *provider_data;

	struct ir *ir;
	int bpf_fd;
};

struct ply_config {
	size_t map_elems;
	size_t string_size;

	unsigned unicode:1; /* allow unicode in output. */
	unsigned hex:1;	    /* prefer hexadecimal output for scalars. */
	unsigned sort:1;    /* sort maps before output, requires more memory. */
	unsigned ksyms:1;   /* create ksyms cache. */
};

extern struct ply_config ply_config;

struct ply {
	struct ply_probe *probes;
	struct symtab globals;
	struct ksyms *ksyms;

	char *group;
	int   group_fd;
};

#define ply_probe_foreach(_ply, _probe)					\
	for ((_probe) = (_ply)->probes;	(_probe); (_probe) = (_probe)->next)

static inline struct ply_probe *sym_to_probe(struct sym *sym)
{
	if (sym->st->global)
		return NULL;

	return container_of(sym->st, struct ply_probe, locals);
}

void ply_maps_print(struct ply *ply);

int ply_start(struct ply *ply);
int ply_stop(struct ply *ply);

int ply_load(struct ply *ply);
int ply_unload(struct ply *ply);

int ply_add_probe(struct ply *ply, struct ply_probe *probe);
int ply_compile(struct ply *ply);


int  ply_fparse(struct ply *ply, FILE *fp);
int  ply_parsef(struct ply *ply, const char *fmt, ...);
void ply_free  (struct ply *ply);
int  ply_alloc (struct ply **plyp);

#endif	/* _PLY_H */
