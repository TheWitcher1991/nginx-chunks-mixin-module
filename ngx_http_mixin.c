#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_mixin.h"

void append_to_conf(char *conf, const char *str) {
    strncat(conf, str, MAX_CONF_SIZE - strlen(conf) - 1);
}

void generate_default_conf(char *default_conf, bool no_cache, const char *http_service) {
    strcpy(default_conf, "");

    append_to_conf(default_conf, "listen %V ssl;\n");
    append_to_conf(default_conf, "listen [::]:%V ssl;\n");
    append_to_conf(default_conf, "server_name %V;\n");
    append_to_conf(default_conf, "http2 on;\n");
    append_to_conf(default_conf, "ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem;\n");
    append_to_conf(default_conf, "ssl_certificate /etc/letsencrypt/live/%V/fullchain.pem;\n");
    append_to_conf(default_conf, "ssl_certificate_key /etc/letsencrypt/live/%V/privkey.pem;\n");
    append_to_conf(default_conf, "ssl_protocols TLSv1.2 TLSv1.3;\n");
    // append_to_conf(default_conf, "ssl_session_timeout 5m;\n");
    // append_to_conf(default_conf, "ssl_prefer_server_ciphers on;\n");

    if (no_cache) {
        append_to_conf(default_conf, "add_header Last-Modified  $date_gmt;\n");
        append_to_conf(default_conf, "add_header Cache-Control  no-store, no-cache, must-revalidate, proxy-revalidate, max-age=0;\n");
    }

    if (http_service != NULL) {
        append_to_conf(default_conf, "location / {\n");
        append_to_conf(default_conf, "proxy_set_header   Host $http_host;\n");
        append_to_conf(default_conf, "proxy_set_header   X-Forwarded-Proto $scheme;\n");
        append_to_conf(default_conf, "proxy_set_header   X-Real-IP $remote_addr;\n");
        append_to_conf(default_conf, "proxy_set_header   X-Forwarded-For $proxy_add_x_forwarded_for;\n");
        append_to_conf(default_conf, "proxy_http_version 1.1;\n");
        append_to_conf(default_conf, "proxy_set_header   Connection \"\";\n");
        char proxy_pass[256];
        snprintf(proxy_pass, sizeof(proxy_pass), "proxy_pass         http://%s/;\n", http_service);
        append_to_conf(default_conf, proxy_pass);
        append_to_conf(default_conf, "}\n");
    }
}

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
    ngx_flag_t tmp_no_cache = value[3];
    ngx_str_t  tmp_service = value[4];

    char default_conf[MAX_CONF_SIZE];
    const char *http_service = tmp_service.data;
    bool no_cache = tmp_no_cache.data;

    if (server_name == NULL || listen == NULL) {
        return NGX_CONF_ERROR;
    }

    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "Creating server with name: %V and listen: %V", &server_name, &listen);

    ngx_str_t config_directive;

    generate_default_conf(default_conf, no_cache, http_service);

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
