#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_mixin.h"

static char* ngx_http_mixin(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);

static ngx_command_t ngx_http_mixin_commands[] = {

};

static ngx_http_module_t ngx_http_mixin_module_ctx = {

};

static ngx_module_t ngx_http_mixin_module = {

};

static char* ngx_http_mixin(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {

};