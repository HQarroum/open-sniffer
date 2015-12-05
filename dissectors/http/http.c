#include "http.h"

/**
 * HTTP parser callbacks.
 */
static int on_header_field(http_parser* parser, const char* at, size_t length);
static int on_header_value(http_parser* parser, const char* at, size_t length);
static int on_url(http_parser* parser, const char* at, size_t length);
static int on_status(http_parser* parser, const char* at, size_t length);
static int on_body(http_parser* parser, const char* at, size_t length);

/**
 * HTTP dissector definition.
 */
static const osi_dissector_t http = {
  .layer         = OSI_LAYER_7,
  .name          = "http",
  .operations    = {
    .handles     = &http_dissector_handles,
    .dump        = &http_dissector_dump,
    .decapsulate = &http_dissector_decapsulate
  }
};

/**
 * HTTP parser settings.
 */
static const http_parser_settings parser_settings = {
  .on_message_begin	= NULL,
  .on_url		= on_url,
  .on_status		= on_status,
  .on_header_field	= on_header_field,
  .on_header_value	= on_header_value,
  .on_headers_complete	= NULL,
  .on_body		= on_body,
  .on_message_complete	= NULL,
  .on_chunk_header	= NULL,
  .on_chunk_complete	= NULL
};

/**
 * An empty parser settings.
 */
static const http_parser_settings parser_settings_empty;

/**
 * An array of structure allowing to map
 * method identifiers as declared in `http-parser.h`
 * in a human-readable representation.
 */
static const http_method_map methods[] = {
  { 0,  "DELETE"	},
  { 1,  "GET"		},
  { 2,  "HEAD"		},
  { 3,  "POST"		},
  { 4,  "PUT"		},
  { 5,  "CONNECT"	},
  { 6,  "OPTIONS"	},
  { 7,  "TRACE"		},
  { 8,  "COPY"		},
  { 9,  "LOCK"		},
  { 10, "MKCOL"		},
  { 11, "MOVE"		},
  { 12, "PROPFIND"	},
  { 13, "PROPPATCH"	},
  { 14, "SEARCH"	},
  { 15, "UNLOCK"	},
  { 16, "REPORT"	},
  { 17, "MKACTIVITY"	},
  { 18, "CHECKOUT"	},
  { 19, "MERGE"		},
  { 20, "MSEARCH"	},
  { 21, "NOTIFY"       	},
  { 22, "SUBSCRIBE"	},
  { 23, "UNSUBSCRIBE"	},
  { 24, "PATCH"		},
  { 25, "PURGE"		},
  { 26, "MKCALENDAR"	}
};

static const char* http_method_from_id(unsigned int id)
{
  for (size_t i = 0; i < sizeof(methods) / sizeof(*methods); ++i) {
    if (methods[i].id == id) {
      return (methods[i].value);
    }
  }
  return (NULL);
}

static http_parser http_dissector_get_parser()
{
  http_parser parser;
  
  http_parser_init(&parser, HTTP_BOTH);
  return (parser);
}

static int on_body(_UNUSED_ http_parser* parser, _UNUSED_ const char* at, size_t length)
{
  printf("Body size : %zu\n", length);
  return (0);
}

static int on_status(_UNUSED_ http_parser* parser, const char* at, size_t length)
{
  printf("Response status : %.*s\n", (int) length, at);
  return (0);
}

static int on_url(http_parser* parser, const char* at, size_t length)
{
  printf("%s %.*s\n", http_method_from_id(parser->method), (int) length, at);
  return (0);
}

static int on_header_field(_UNUSED_ http_parser* parser, const char* at, size_t length)
{
  printf("%.*s: ", (int) length, at);
  return (0);
}

static int on_header_value(_UNUSED_ http_parser* parser, const char* at, size_t length)
{
  printf("%.*s\n", (int) length, at);
  return (0);
}

/**
 * \brief Creates a new instance of an HTTP
 * dissector.
 */
const osi_dissector_t* http_dissector_new()
{
  return (&http);
}

int http_dissector_handles(const packet_t* packet)
{
  if (!packet || !packet->data
      || packet->protocol != 0
      || packet->session == NULL) {
    // Invalid packet.
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  http_parser parser = http_dissector_get_parser();
  http_parser_execute(&parser, &parser_settings_empty, packet->data, packet->size);
  if (parser.http_errno) {
    // An error occurred while parsing the packet,
    // we consider it as invalid.
    return (OSI_PACKET_NOT_SUPPORTED);
  }
  return (OSI_PACKET_SUPPORTED);
}
 
void http_dissector_dump(const packet_t* packet)
{
  http_parser		parser;
  
  if (!http_dissector_handles(packet)) {
    return;
  }
  
  parser = http_dissector_get_parser();
  printf(HTTP_OUTPUT_HEADER);
  http_parser_execute(&parser, &parser_settings, packet->data, packet->size);
  printf(HTTP_OUTPUT_FOOTER);
}

packet_t* http_dissector_decapsulate(_UNUSED_ const packet_t* packet)
{
  return (NULL);
}
