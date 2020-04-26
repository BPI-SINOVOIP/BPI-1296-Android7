

#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

/*
 * url parser
 * {proto}://{user}:{password}@{host}:{port}{uri}
 * {proto}://{user}:{password}@{[host]}:{port}{uri}
 *
 * return:
 *   0  invalid url
 *   1  valid url
 */
////int _x_parse_url (char *url, char **proto, char** host, int *port,
////                  char **uri);

int _x_parse_url (char *url, char **proto, char** host, int *port,
                         char **user, char **password, char **uri);

/*
 * canonicalise url, given base
 * base must be valid according to _x_parse_url
 * url may only contain "://" if it's absolute
 *
 * return:
 *   the canonicalised URL (caller must free() it)
 */
char *_x_canonicalise_url (const char *base, const char *url);

#endif /* HTTP_HELPER_H */
