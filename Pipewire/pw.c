#define _GNU_SOURCE
#include <regex.h>
#include <spa/utils/result.h>
#include <spa/param/audio/format-utils.h>
#include <spa/param/latency-utils.h>
#include <pipewire/pipewire.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "shreduler_private.h"

#include "instr.h"
#include "operator.h"
#include "object.h"
#include "import.h"

#include "gack.h"


struct object {
	struct spa_list link;

	uint32_t id;
#define OBJECT_ANY	0
#define OBJECT_NODE	1
#define OBJECT_PORT	2
#define OBJECT_LINK	3
	uint32_t type;
	struct pw_properties *props;
	uint32_t extra[2];
};

struct data {
	struct pw_main_loop *loop;

	const char *opt_remote;
#define MODE_LIST_OUTPUT	(1<<0)
#define MODE_LIST_INPUT		(1<<1)
#define MODE_LIST_PORTS		(MODE_LIST_OUTPUT|MODE_LIST_INPUT)
#define MODE_LIST_LINKS		(1<<2)
#define MODE_LIST		(MODE_LIST_PORTS|MODE_LIST_LINKS)
#define MODE_MONITOR		(1<<3)
#define MODE_DISCONNECT		(1<<4)
	uint32_t opt_mode;
	bool opt_id;
	bool opt_verbose;
	const char *opt_output;
	const char *opt_input;
	struct pw_properties *props;

	struct pw_context *context;

	struct pw_core *core;
	struct spa_hook core_listener;

	struct pw_registry *registry;
	struct spa_hook registry_listener;

	struct spa_list objects;

	int sync;
	int link_res;
	bool monitoring;
	bool list_inputs;
	bool list_outputs;
	const char *prefix;

	regex_t out_port_regex, *out_regex;
	regex_t in_port_regex, *in_regex;
};

static void link_proxy_error(void *data, int seq NUSED, int res, const char *message NUSED) {
	struct data *d = data;
	d->link_res = res;
}

static const struct pw_proxy_events link_proxy_events = {
	PW_VERSION_PROXY_EVENTS,
	.error = link_proxy_error,
};

static void core_sync(struct data *data) {
	data->sync = pw_core_sync(data->core, PW_ID_CORE, data->sync);
}

static int create_link(struct data *data) {
	struct pw_proxy *proxy;
	struct spa_hook listener;

	data->link_res = 0;

	proxy = pw_core_create_object(data->core,
			"link-factory",
			PW_TYPE_INTERFACE_Link,
			PW_VERSION_LINK,
			&data->props->dict, 0);
	if (proxy == NULL)
		return -errno;

	spa_zero(listener);
	pw_proxy_add_listener(proxy, &listener, &link_proxy_events, data);

	core_sync(data);
	pw_main_loop_run(data->loop);

        spa_hook_remove(&listener);

        pw_proxy_destroy(proxy);

	return data->link_res;
}

static struct object *find_object(struct data *data, uint32_t type, uint32_t id) {
	struct object *o;
	spa_list_for_each(o, &data->objects, link)
		if ((type == OBJECT_ANY || o->type == type) && o->id == id)
			return o;
	return NULL;
}

static struct object *find_node_port(struct data *data, struct object *node, enum pw_direction direction, const char *port_id ) {
	struct object *o;
	spa_list_for_each(o, &data->objects, link) {
		const char *o_port_id;
		if (o->type != OBJECT_PORT)
			continue;
		if (o->extra[1] != node->id)
			continue;
		if (o->extra[0] != direction)
			continue;
		if ((o_port_id = pw_properties_get(o->props, PW_KEY_PORT_ID)) == NULL)
			continue;
		if (spa_streq(o_port_id, port_id))
			return o;
	}
	return NULL;
}

static char *node_name(char *buffer, int size, struct object *n) {
	const char *name; 
	buffer[0] = '\0';
	if ((name = pw_properties_get(n->props, PW_KEY_NODE_NAME)) == NULL)
		return buffer;
	snprintf(buffer, size, "%s", name);
	return buffer;
}

static char *node_path(char *buffer, int size, struct object *n) {
	const char *name;
	buffer[0] = '\0';
	if ((name = pw_properties_get(n->props, PW_KEY_OBJECT_PATH)) == NULL)
		return buffer;
	snprintf(buffer, size, "%s", name);
	return buffer;
}

static char *port_name(char *buffer, int size, struct object *n, struct object *p) {
	const char *name1, *name2;
	buffer[0] = '\0';
	if ((name1 = pw_properties_get(n->props, PW_KEY_NODE_NAME)) == NULL)
		return buffer;
	if ((name2 = pw_properties_get(p->props, PW_KEY_PORT_NAME)) == NULL)
		return buffer;
	snprintf(buffer, size, "%s:%s", name1, name2);
	return buffer;
}

static char *port_path(char *buffer, int size, struct object *p) {
	const char *name;
	buffer[0] = '\0';
	if ((name = pw_properties_get(p->props, PW_KEY_OBJECT_PATH)) == NULL)
		return buffer;
	snprintf(buffer, size, "%s", name);
	return buffer;
}

static char *port_alias(char *buffer, int size, struct object *n NUSED, struct object *p) {
	const char *name;
	buffer[0] = '\0';
	if ((name = pw_properties_get(p->props, PW_KEY_PORT_ALIAS)) == NULL)
		return buffer;
	snprintf(buffer, size, "%s", name);
	return buffer;
}

static void print_port(struct object *n,
		struct object *p /*, bool verbose*/, const Gwion gwion, const M_Vector array) {
  char buffer[1024];
  m_str name = port_name(buffer, sizeof(buffer), n, p);
  if(!strlen(name)) return;
  M_Object str = new_string(gwion, name);
  m_vector_add(array, &str);
}

static void print_port_id(struct data *data, uint32_t peer, const Gwion gwion, const M_Vector array) {
	struct object *n, *p;
	if ((p = find_object(data, OBJECT_PORT, peer)) == NULL)
		return;
	if ((n = find_object(data, OBJECT_NODE, p->extra[1])) == NULL)
		return;
	print_port(n, p/*, false*/, gwion, array);
}

static void do_list_port_links(struct data *data, struct object *node, struct object *port, const Gwion gwion, const M_Vector array) {
	struct object *o;
	bool first = false;
	if ((data->opt_mode & MODE_LIST_PORTS) == 0)
		first = true;
	spa_list_for_each(o, &data->objects, link) {
		uint32_t peer;
		char prefix[64], id[16] = "";

		if (data->opt_id)
			snprintf(id, sizeof(id), "%4d ", o->id);

		if (o->type != OBJECT_LINK)
			continue;

		if (port->extra[0] == PW_DIRECTION_OUTPUT &&
		    o->extra[0] == port->id) {
			peer = o->extra[1];
			snprintf(prefix, sizeof(prefix), "%s  |-> ", id);
		}
		else if (port->extra[0] == PW_DIRECTION_INPUT &&
		    o->extra[1] == port->id) {
			peer = o->extra[0];
			snprintf(prefix, sizeof(prefix), "%s  |<- ", id);
		}
		else
			continue;

		if (first) {
			print_port(node, port /*, data->opt_verbose*/, gwion, array);
			first = false;
		}
		print_port_id(data, peer, gwion, array);
	}
}

static int node_matches(struct data *data NUSED, struct object *n, const char *name) {
	char buffer[1024];
	uint32_t id = atoi(name);
	if (n->id == id)
		return 1;
	if (spa_streq(node_name(buffer, sizeof(buffer), n), name))
		return 1;
	if (spa_streq(node_path(buffer, sizeof(buffer), n), name))
		return 1;
	return 0;
}

static int port_matches(struct object *n, struct object *p, const char *name) {
	char buffer[1024];
	uint32_t id = atoi(name);
	if (p->id == id)
		return 1;
	if (spa_streq(port_name(buffer, sizeof(buffer), n, p), name))
		return 1;
	if (spa_streq(port_path(buffer, sizeof(buffer), p), name))
		return 1;
	if (spa_streq(port_alias(buffer, sizeof(buffer), n, p), name))
		return 1;
	return 0;
}

static int port_regex(struct object *n, struct object *p, regex_t *regex) {
	char buffer[1024];
	if (regexec(regex, port_name(buffer, sizeof(buffer), n, p), 0, NULL, 0) == 0)
		return 1;
	return 0;
}

static void do_list_ports(struct data *data, struct object *node,
		enum pw_direction direction, regex_t *regex, const Gwion gwion ,const M_Object ret) {
	struct object *o;
	spa_list_for_each(o, &data->objects, link) {
		if (o->type != OBJECT_PORT)
			continue;
		if (o->extra[1] != node->id)
			continue;
		if (o->extra[0] != direction)
			continue;

		if (regex && !port_regex(node, o, regex))
			continue;

		if (data->opt_mode & MODE_LIST_PORTS)
			print_port(node, o /*, data->opt_verbose*/, gwion, ARRAY(ret));
		else if (data->opt_mode & MODE_LIST_LINKS)
			do_list_port_links(data, node, o, gwion, ARRAY(ret));
	}
}

static void do_list(struct data *data, const Gwion gwion, const M_Object ret) {
	struct object *n;

	spa_list_for_each(n, &data->objects, link) {
		if (n->type != OBJECT_NODE)
			continue;
		if (data->list_outputs)
			do_list_ports(data, n, PW_DIRECTION_OUTPUT, data->out_regex, gwion ,ret);
		if (data->list_inputs)
			do_list_ports(data, n, PW_DIRECTION_INPUT, data->in_regex, gwion ,ret);
	}
}

static int do_link_ports(struct data *data) {
	uint32_t in_port = 0, out_port = 0;
	struct object *n, *p;
	struct object *in_node = NULL, *out_node = NULL;

	spa_list_for_each(n, &data->objects, link) {
		if (n->type != OBJECT_NODE)
			continue;

		if (out_node == NULL && node_matches(data, n, data->opt_output)) {
			out_node = n;
			continue;
		} else if (in_node == NULL && node_matches(data, n, data->opt_input)) {
			in_node = n;
			continue;
		}

		spa_list_for_each(p, &data->objects, link) {
			if (p->type != OBJECT_PORT)
				continue;
			if (p->extra[1] != n->id)
				continue;

			if (out_port == 0 && p->extra[0] == PW_DIRECTION_OUTPUT &&
			    port_matches(n, p, data->opt_output))
				out_port = p->id;
			else if (in_port == 0 && p->extra[0] == PW_DIRECTION_INPUT &&
			    port_matches(n, p, data->opt_input))
				in_port = p->id;
		}
	}

	if (in_node && out_node) {
		int i, ret;
		char port_id[32];
		bool all_links_exist = true;

		for (i=0;; i++) {
			snprintf(port_id, sizeof(port_id), "%d", i);

			struct object *port_out = find_node_port(data, out_node, PW_DIRECTION_OUTPUT, port_id);
			struct object *port_in = find_node_port(data, in_node, PW_DIRECTION_INPUT, port_id);

			if (!port_out || !port_in)
				break;

			pw_properties_setf(data->props, PW_KEY_LINK_OUTPUT_PORT, "%u", port_out->id);
			pw_properties_setf(data->props, PW_KEY_LINK_INPUT_PORT, "%u", port_in->id);

			if ((ret = create_link(data)) < 0 && ret != -EEXIST)
				return ret;

			if (ret >= 0)
				all_links_exist = false;
		}
		return (all_links_exist ? -EEXIST : 0);
	}

	if (in_port == 0 || out_port == 0)
		return -ENOENT;

	pw_properties_setf(data->props, PW_KEY_LINK_OUTPUT_PORT, "%u", out_port);
	pw_properties_setf(data->props, PW_KEY_LINK_INPUT_PORT, "%u", in_port);

	return create_link(data);
}

static int do_unlink_ports(struct data *data) {
	struct object *l, *n, *p;
	bool found_any = false;
	struct object *in_node = NULL, *out_node = NULL;

	if (data->opt_input != NULL) {
		/* 2 args, check if they are node names */
		spa_list_for_each(n, &data->objects, link) {
			if (n->type != OBJECT_NODE)
				continue;

			if (out_node == NULL && node_matches(data, n, data->opt_output)) {
				out_node = n;
				continue;
			} else if (in_node == NULL && node_matches(data, n, data->opt_input)) {
				in_node = n;
				continue;
			}
		}
	}

	spa_list_for_each(l, &data->objects, link) {
		if (l->type != OBJECT_LINK)
			continue;

		if (data->opt_input == NULL) {
			/* 1 arg, check link id */
			if (l->id != (uint32_t)atoi(data->opt_output))
				continue;
		} else if (out_node && in_node) {
			/* 2 args, check nodes */
			if ((p = find_object(data, OBJECT_PORT, l->extra[0])) == NULL)
				continue;
			if ((n = find_object(data, OBJECT_NODE, p->extra[1])) == NULL)
				continue;
			if (n->id != out_node->id)
				continue;

			if ((p = find_object(data, OBJECT_PORT, l->extra[1])) == NULL)
				continue;
			if ((n = find_object(data, OBJECT_NODE, p->extra[1])) == NULL)
				continue;
			if (n->id != in_node->id)
				continue;
		} else {
			/* 2 args, check port names */
			if ((p = find_object(data, OBJECT_PORT, l->extra[0])) == NULL)
				continue;
			if ((n = find_object(data, OBJECT_NODE, p->extra[1])) == NULL)
				continue;
			if (!port_matches(n, p, data->opt_output))
				continue;

			if ((p = find_object(data, OBJECT_PORT, l->extra[1])) == NULL)
				continue;
			if ((n = find_object(data, OBJECT_NODE, p->extra[1])) == NULL)
				continue;
			if (!port_matches(n, p, data->opt_input))
				continue;
		}
		pw_registry_destroy(data->registry, l->id);
		found_any = true;
	}
	if (!found_any)
		return -ENOENT;

	core_sync(data);
	pw_main_loop_run(data->loop);

	return 0;
}

static void registry_event_global(void *data, uint32_t id, uint32_t permissions NUSED,
				  const char *type, uint32_t version NUSED,
				  const struct spa_dict *props){
	struct data *d = data;
	uint32_t t, extra[2];
	struct object *obj;
	const char *str;

	if (props == NULL)
		return;

	spa_zero(extra);
	if (spa_streq(type, PW_TYPE_INTERFACE_Node)) {
		t = OBJECT_NODE;
	} else if (spa_streq(type, PW_TYPE_INTERFACE_Port)) {
		t = OBJECT_PORT;
		if ((str = spa_dict_lookup(props, PW_KEY_PORT_DIRECTION)) == NULL)
			return;
		if (spa_streq(str, "in"))
			extra[0] = PW_DIRECTION_INPUT;
		else if (spa_streq(str, "out"))
			extra[0] = PW_DIRECTION_OUTPUT;
		else
			return;
		if ((str = spa_dict_lookup(props, PW_KEY_NODE_ID)) == NULL)
			return;
		extra[1] = atoi(str);
	} else if (spa_streq(type, PW_TYPE_INTERFACE_Link)) {
		t = OBJECT_LINK;
		if ((str = spa_dict_lookup(props, PW_KEY_LINK_OUTPUT_PORT)) == NULL)
			return;
		extra[0] = atoi(str);
		if ((str = spa_dict_lookup(props, PW_KEY_LINK_INPUT_PORT)) == NULL)
			return;
		extra[1] = atoi(str);
	} else
		return;

	obj = calloc(1, sizeof(*obj));
	obj->type = t;
	obj->id = id;
	obj->props = pw_properties_new_dict(props);
	memcpy(obj->extra, extra, sizeof(extra));
	spa_list_append(&d->objects, &obj->link);
}

static void registry_event_global_remove(void *data, uint32_t id) {
	struct data *d = data;
	struct object *obj;

	if ((obj = find_object(d, OBJECT_ANY, id)) == NULL)
		return;

	spa_list_remove(&obj->link);
	pw_properties_free(obj->props);
	free(obj);
}

static const struct pw_registry_events registry_events = {
	PW_VERSION_REGISTRY_EVENTS,
	.global = registry_event_global,
	.global_remove = registry_event_global_remove,
};

static void on_core_done(void *data, uint32_t id NUSED, int seq) {
	struct data *d = data;
	if (d->sync == seq)
		pw_main_loop_quit(d->loop);
}

static void on_core_error(void *data, uint32_t id, int seq, int res, const char *message) {
	struct data *d = data;

	pw_log_error("error id:%u seq:%d res:%d (%s): %s",
			id, seq, res, spa_strerror(res), message);

	if (id == PW_ID_CORE && res == -EPIPE)
		pw_main_loop_quit(d->loop);
}

static const struct pw_core_events core_events = {
	PW_VERSION_CORE_EVENTS,
	.done = on_core_done,
	.error = on_core_error,
};

ANN static bool start(struct data *data) {
  int argc; char **argv;
  pw_init(&argc, &argv);
  spa_list_init(&data->objects);
  if(!(data->props = pw_properties_new(NULL, NULL))) return false;
  pw_properties_set(data->props, PW_KEY_OBJECT_LINGER, "true");
  if(!(data->loop = pw_main_loop_new(NULL))) return false;
  if(!(data->context = pw_context_new(pw_main_loop_get_loop(data->loop), NULL, 0)))
    return false;
  if(!(data->core = pw_context_connect(data->context,
			pw_properties_new(
				PW_KEY_REMOTE_NAME, data->opt_remote,
				NULL),
			0))) return false;
  pw_core_add_listener(data->core,
                       &data->core_listener,
                       &core_events, data);
  data->registry = pw_core_get_registry(data->core,
                       PW_VERSION_REGISTRY, 0);
  pw_registry_add_listener(data->registry,
                       &data->registry_listener,
                       &registry_events, data);
  data->prefix = "";
  core_sync(data);
  pw_main_loop_run(data->loop);
  return true;
}

ANN static void clear(struct data *data) {
  spa_hook_remove(&data->registry_listener);
  pw_proxy_destroy((struct pw_proxy*)data->registry);
  spa_hook_remove(&data->core_listener);
  pw_core_disconnect(data->core);
  pw_context_destroy(data->context);
  pw_main_loop_destroy(data->loop);
  pw_deinit();
}

static SFUN(gwpw_link) {
  struct data data = {};
  if(!start(&data)) {
    xfun_handle(shred, "PwInit");
    return;
  }
  regex_t out_port_regex;
  regex_t in_port_regex;
  data.opt_output = STRING(*(M_Object*)MEM(0));
  data.opt_input = STRING(*(M_Object*)MEM(SZ_INT));
  if (regcomp(&out_port_regex, data.opt_output, REG_EXTENDED | REG_NOSUB) == 0)
                data.out_regex = &out_port_regex;
  if (regcomp(&in_port_regex, data.opt_input, REG_EXTENDED | REG_NOSUB) == 0)
                data.in_regex = &in_port_regex;
  if (do_link_ports(&data) < 0)
    xfun_handle(shred, "PwLink");
  clear(&data);
}

static SFUN(gwpw_unlink) {
  struct data data = { .opt_mode = MODE_DISCONNECT };
  if(!start(&data)) {
    xfun_handle(shred, "PwInit");
    return;
  }
  regex_t out_port_regex;
  regex_t in_port_regex;
  data.opt_output = STRING(*(M_Object*)MEM(0));
  data.opt_input = STRING(*(M_Object*)MEM(SZ_INT));
  if (regcomp(&out_port_regex, data.opt_output, REG_EXTENDED | REG_NOSUB) == 0)
                data.out_regex = &out_port_regex;
  if (regcomp(&in_port_regex, data.opt_input, REG_EXTENDED | REG_NOSUB) == 0)
                data.in_regex = &in_port_regex;
  if (do_unlink_ports(&data) < 0) {
    xfun_handle(shred, "PwLink");
  }
  if (data.out_regex)
    regfree(data.out_regex);
  if (data.in_regex)
    regfree(data.in_regex);
  clear(&data);
}

static SFUN(gwpw_list) {
  struct data data = { .opt_mode = *(uint32_t*)MEM(0) };
  if(!start(&data)) {
    xfun_handle(shred, "PwInit");
    return;
  }
  if ((data.opt_mode & (MODE_LIST_PORTS|MODE_LIST_LINKS)) == MODE_LIST_LINKS)
    data.list_inputs = data.list_outputs = true;
  if ((data.opt_mode & MODE_LIST_INPUT) == MODE_LIST_INPUT)
    data.list_inputs = true;
  if ((data.opt_mode & MODE_LIST_OUTPUT) == MODE_LIST_OUTPUT)
    data.list_outputs = true;
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
  const M_Object ret = new_array(shred->info->mp, code->ret_type, 0);
  *(M_Object*)RETURN = ret;
  do_list(&data, shred->info->vm->gwion, ret);
  clear(&data);
}

GWION_IMPORT(Pipewire) {
  GWI_OB(gwi_struct_ini(gwi, "Pipewire"));

  GWI_BB(gwi_func_ini(gwi, "void", "link"));
  GWI_BB(gwi_func_arg(gwi, "string", "port1"));
  GWI_BB(gwi_func_arg(gwi, "string", "port2"));
  GWI_BB(gwi_func_end(gwi, gwpw_link, ae_flag_static));

  GWI_BB(gwi_func_ini(gwi, "void", "unlink"));
  GWI_BB(gwi_func_arg(gwi, "string", "port1"));
  GWI_BB(gwi_func_arg(gwi, "string", "port2"));
  GWI_BB(gwi_func_end(gwi, gwpw_unlink, ae_flag_static));

  GWI_BB(gwi_enum_ini(gwi, "Type"));
  GWI_BB(gwi_enum_add(gwi, "OUTPUT", MODE_LIST_OUTPUT));
  GWI_BB(gwi_enum_add(gwi, "INPUT", MODE_LIST_INPUT));
  GWI_BB(gwi_enum_add(gwi, "PORTS", MODE_LIST_PORTS));
  GWI_BB(gwi_enum_add(gwi, "LINK", MODE_LIST_LINKS));
  GWI_OB(gwi_enum_end(gwi));

  GWI_BB(gwi_func_ini(gwi, "string[]", "list"));
  GWI_BB(gwi_func_arg(gwi, "Type", "type"));
  GWI_BB(gwi_func_end(gwi, gwpw_list, ae_flag_static));

  GWI_OB(gwi_struct_end(gwi));
  return GW_OK;
}
