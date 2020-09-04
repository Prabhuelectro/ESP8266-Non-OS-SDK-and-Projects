#ifndef ESP_NANO_HTTPD_H_
#define ESP_NANO_HTTPD_H_

#include <c_types.h>
#include <ip_addr.h>
#include <espconn.h>
#include <user_interface.h>
#include <json/jsontree.h>

/* http callback structure */
typedef struct {
	const char *path;
	void (*handler)(struct espconn *, void *, uint32_t);
	void *arg;
	uint32_t arg_len;
} http_callback_t;

/* http request structure */
typedef struct {
	enum {
		TYPE_UNKNOWN= 0,
		TYPE_GET	= 1,
		TYPE_POST	= 2
	} type;
	enum {
		REQ_GOT_HEADER	 = 0,
		REQ_CONTENT_CHUNK= 1
	} read_state;

	const char* path;
	const char* query;
	const char* content_type;
	void *content;
	uint32_t content_len;

	uint32_t cont_chunk_len;
	uint32_t cont_bytes_left;

	const void *callback;
	void *prv_data;

	const char *resp_content;
	uint8_t    *resp_alloc;
	uint32_t    resp_bytes_left;
} http_request_t;

void esp_nano_httpd_register_content(const http_callback_t *content_info );
void esp_nano_httpd_init(void);
void esp_nano_httpd_init_AP(uint8_t wifi_mode, const char *AP_ssid, const char *AP_pass);

void send_http_response(struct espconn *conn, const char *code, const char *cont_type, const char *content, uint32_t cont_len);
void send_html(struct espconn *conn, void *html, uint32_t len);
void send_text(struct espconn *conn, void *txt, uint32_t len);
void send_css(struct espconn *conn, void *css, uint32_t len);
void send_svg(struct espconn *conn, void *svg, uint32_t len);
void send_json_tree(struct espconn *conn, struct jsontree_object *js_tree, uint32_t cache_size);

void resp_http_ok(struct espconn *conn);
void resp_http_404(struct espconn *conn);
void resp_http_error(struct espconn *conn);

#endif /* ESP_NANO_HTTPD_H_ */
