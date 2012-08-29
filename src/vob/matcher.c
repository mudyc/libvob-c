

#include "vob/matcher.h"
#include "util/dbg.h"

static void matcher_add(VobMatcher *m, Coordsys *cs, char *key)
{
	g_hash_table_insert(m->key2cs, key, cs);
	g_hash_table_insert(m->cs2key, cs, key);
}
static Coordsys* matcher_get(VobMatcher *m, char *key)
{
	return g_hash_table_lookup(m->key2cs, key);
}
VobMatcher *vob_matcher(Region *r)
{
	VobMatcher *ret = (VobMatcher *)malloc(sizeof(VobMatcher));
	ret->add = &matcher_add;
	ret->get = &matcher_get;
	ret->cs2key = g_hash_table_new(g_direct_hash, g_direct_equal);
	ret->key2cs = g_hash_table_new(&g_str_hash, &g_str_equal);
	return ret;
}

void vob_matcher_clear(VobMatcher *m)
{
	g_hash_table_remove_all(m->cs2key);
	g_hash_table_remove_all(m->key2cs);
}
