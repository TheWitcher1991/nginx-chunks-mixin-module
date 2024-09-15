#include <string>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_mixin.h"

static char* ngx_http_mixin(ngx_conf_t* cf, ngx_command_t* cmd, void* conf);

static ngx_command_t ngx_http_mixin_commands[] = {
    {
        ngx_string("mixin"),
        NGX_HTTP_MAIN_CONF | NGX_CONF_BLOCK | NGX_CONF_TAKE2,
        ngx_http_mixin,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command
};


static char* ngx_http_mixin(ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    ngx_http_mixin_t *gcf = conf;

    ngx_str_t  *value = cf->args->elts;
    ngx_str_t  server_name = value[1];
    ngx_str_t  listen = value[2];
    ngx_flag_t no_cache = value[3];

    if (server_name == NULL || listen == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "Creating server with name: %V and listen: %V", &server_name, &listen);

    ngx_str_t config_directive;

    string default_conf = "";

    default_conf = default_conf + "listen %V ssl;\nlisten [::]:%V ssl;\nserver_name %V;\nhttp2 on;\n";
    default_conf = default_conf + "ssl_protocols TLSv1.2 TLSv1.3;\n";
    default_conf = default_conf + "ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem;";
    default_conf = default_conf + "ssl_certificate /etc/letsencrypt/live/%V/fullchain.pem;\n";
    default_conf = default_conf + "ssl_certificate_key /etc/letsencrypt/live/%V/privkey.pem;\n";
    // default_conf = default_conf + "ssl_session_timeout 5m;\n";
    // default_conf = default_conf + "ssl_prefer_server_ciphers on;\n";

    if (no_cache == true) {
        default_conf = default_conf + "add_header Last-Modified  $date_gmt;\n";
        default_conf = default_conf + "no-store, no-cache, must-revalidate, proxy-revalidate, max-age=0;\n";
    }

    ngx_sprintf(config_directive.data, default_conf, &listen, &listen, &server_name, &server_name, &server_name);

    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "Generated config: %s", config_directive.data);

    return NGX_CONF_OK;
};

static ngx_http_module_t ngx_http_mixin_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static ngx_module_t ngx_http_mixin_module = {
    NGX_MODULE_V1,
    &ngx_http_mixin_module_ctx,
    ngx_http_mixin_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};
