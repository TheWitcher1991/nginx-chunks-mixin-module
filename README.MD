## Nginx mixin for base server ```is dev```

```nginx
server {
    listen ${INTERFACE_HTTPS_PORT} ssl;
    listen [::]:${INTERFACE_HTTPS_PORT} ssl;
    server_name ${DNS};
    
    http2 on;
    
    ssl_dhparam /etc/letsencrypt/ssl-dhparams.pem;
    ssl_certificate /etc/letsencrypt/live/${DNS}/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/${DNS}/privkey.pem;
    ssl_protocols TLSv1.2 TLSv1.3;
    
    # with condition
    add_header Last-Modified  $date_gmt;
    add_header Cache-Control  "no-store, no-cache, must-revalidate, proxy-revalidate, max-age=0";
}
```
