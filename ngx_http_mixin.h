#ifndef _NGX_HTTP_MIXIN_MODULE_H_INCLUDED_
#define _NGX_HTTP_MIXIN_MODULE_H_INCLUDED_

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
    ngx_str_t  server_name;
    ngx_str
    ngx_int_t  listen;
} ngx_http_mixin_t;

// typedef struct {
//    ngx_str_t command_name;
//    ngx_str_t argument;
// } ngx_http_mixin_t;

#endif /* _NGX_HTTP_MIXIN_MODULE_H_INCLUDED_ */