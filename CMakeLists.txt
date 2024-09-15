cmake_minimum_required(VERSION 3.10)
project(nginx_mixin_module C)

add_library(ngx_http_mixin MODULE ngx_http_mixin.c)