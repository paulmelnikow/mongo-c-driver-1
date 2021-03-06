#include <mongoc.h>
#include <mongoc-cursor-private.h>

#include "TestSuite.h"
#include "test-libmongoc.h"

static void
test_get_host (void)
{
   const mongoc_host_list_t *hosts;
   mongoc_host_list_t host;
   mongoc_client_t *client;
   mongoc_cursor_t *cursor;
   char *uri_str = test_framework_get_uri_str (NULL);
   mongoc_uri_t *uri;
   const bson_t *doc;
   bson_error_t error;
   bool r;
   bson_t q = BSON_INITIALIZER;

   uri = mongoc_uri_new (uri_str);

   hosts = mongoc_uri_get_hosts(uri);

   client = test_framework_client_new (uri_str);
   cursor = _mongoc_cursor_new(client, "test.test", MONGOC_QUERY_NONE, 0, 1, 1,
                               false, &q, NULL, NULL);
   r = mongoc_cursor_next(cursor, &doc);
   if (!r && mongoc_cursor_error(cursor, &error)) {
      MONGOC_ERROR("%s", error.message);
      abort();
   }

   assert (doc == mongoc_cursor_current (cursor));

   mongoc_cursor_get_host(cursor, &host);
   ASSERT_CMPSTR (host.host, hosts->host);
   ASSERT_CMPSTR (host.host_and_port, hosts->host_and_port);
   ASSERT_CMPINT (host.port, ==, hosts->port);
   ASSERT_CMPINT (host.family, ==, hosts->family);

   bson_free (uri_str);
   mongoc_uri_destroy(uri);
   mongoc_client_destroy (client);
   mongoc_cursor_destroy (cursor);
}

static void
test_clone (void)
{
   mongoc_cursor_t *clone;
   mongoc_cursor_t *cursor;
   mongoc_client_t *client;
   const bson_t *doc;
   bson_error_t error;
   bool r;
   bson_t q = BSON_INITIALIZER;

   client = test_framework_client_new (NULL);

   {
      /*
       * Ensure test.test has a document.
       */

      mongoc_collection_t *col;

      col = mongoc_client_get_collection (client, "test", "test");
      r = mongoc_collection_insert (col, MONGOC_INSERT_NONE, &q, NULL, &error);
      ASSERT (r);

      mongoc_collection_destroy (col);
   }

   cursor = _mongoc_cursor_new(client, "test.test", MONGOC_QUERY_NONE, 0, 1, 1,
                               false, &q, NULL, NULL);
   ASSERT(cursor);

   r = mongoc_cursor_next(cursor, &doc);
   if (!r || mongoc_cursor_error(cursor, &error)) {
      MONGOC_ERROR("%s", error.message);
      abort();
   }
   ASSERT (doc);

   clone = mongoc_cursor_clone(cursor);
   ASSERT(cursor);

   r = mongoc_cursor_next(clone, &doc);
   if (!r || mongoc_cursor_error(clone, &error)) {
      MONGOC_ERROR("%s", error.message);
      abort();
   }
   ASSERT (doc);

   mongoc_cursor_destroy(cursor);
   mongoc_cursor_destroy(clone);
   mongoc_client_destroy(client);
}


static void
test_invalid_query (void)
{
   mongoc_client_t *client;
   mongoc_cursor_t *cursor;
   bson_error_t error;
   const bson_t *doc = NULL;
   bson_t *q;
   bool r;

   client = test_framework_client_new (NULL);
   assert (client);

   q = BCON_NEW ("foo", BCON_INT32 (1), "$orderby", "{", "}");

   cursor = _mongoc_cursor_new (client, "test.test", MONGOC_QUERY_NONE, 0, 1, 1,
                                false, q, NULL, NULL);
   r = mongoc_cursor_next (cursor, &doc);
   assert (!r);
   mongoc_cursor_error (cursor, &error);
   assert (strstr (error.message, "$query"));
   assert (error.domain == MONGOC_ERROR_CURSOR);
   assert (error.code == MONGOC_ERROR_CURSOR_INVALID_CURSOR);
   assert (doc == NULL);

   bson_destroy (q);
   mongoc_cursor_destroy (cursor);
   mongoc_client_destroy (client);
}


void
test_cursor_install (TestSuite *suite)
{
   TestSuite_Add (suite, "/Cursor/get_host", test_get_host);
   TestSuite_Add (suite, "/Cursor/clone", test_clone);
   TestSuite_Add (suite, "/Cursor/invalid_query", test_invalid_query);
}
