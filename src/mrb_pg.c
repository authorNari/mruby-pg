#include <mruby.h>
#include <stdio.h>
#include <mruby/data.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/hash.h>
#include <mruby/class.h>
#include <mruby/variable.h>

/* PostgreSQL headers */
#include "libpq-fe.h"
#include "libpq/libpq-fs.h"
#include "pg_config_manual.h"

static void
pgconn_free(mrb_state *mrb, void *conn)
{
  if (conn != NULL) {
    PQfinish((PGconn *)conn);
  }
}

const struct mrb_data_type mrb_pgconn_type = {
  "pgconn", pgconn_free,
};

static mrb_value
pgconn_init(mrb_state *mrb, mrb_value self)
{
  PGconn *conn = NULL;
  mrb_value *argv;
  int argc;
  mrb_value conninfo;

  argc = mrb_get_args(mrb, "*", &argv, &argc);
  conninfo = mrb_funcall_argv(mrb,
                              mrb_obj_value(mrb_class_get_under(mrb, mrb_module_get(mrb, "PG"), "Connection")),
                              mrb_intern_lit(mrb, "parse_connect_args"),
                              argc, argv);
  conn = PQconnectdb(mrb_string_value_ptr(mrb, conninfo));

  if (conn == NULL) {
    mrb_raise(mrb, mrb_class_get(mrb, "PG::Error"),
              "PQconnectdb() unable to allocate structure");
  }

  DATA_PTR(self) = conn;
  DATA_TYPE(self) = &mrb_pgconn_type;

  if (PQstatus(conn) == CONNECTION_BAD) {
    mrb_raise(mrb, mrb_class_get_under(mrb, mrb_module_get(mrb, "PG"), "Error"), PQerrorMessage(conn));
  }

  return self;
}

void
mrb_mruby_pg_gem_init(mrb_state* mrb) {
  struct RClass *_mPG, *_cPGconn;

  _mPG = mrb_define_module(mrb, "PG");

  _cPGconn = mrb_define_class_under(mrb, _mPG, "Connection", mrb->object_class);
  MRB_SET_INSTANCE_TT(_cPGconn, MRB_TT_DATA);
  mrb_define_method(mrb, _cPGconn, "initialize", pgconn_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, _cPGconn, "connect", pgconn_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, _cPGconn, "open", pgconn_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, _cPGconn, "setdb", pgconn_init, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, _cPGconn, "setdblogin", pgconn_init, MRB_ARGS_OPT(1));

  mrb_define_class_under(mrb, _mPG, "Error", mrb->eStandardError_class);
}

void
mrb_mruby_pg_gem_final(mrb_state* mrb) {
  /* finalizer */
}
