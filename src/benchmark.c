#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <time.h>

#include <clo.h>
#include <jansson.h>
#include "cJSON.h"

uint64_t ntime(void)
{
  struct timespec tv;

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tv);
  return ((uint64_t) tv.tv_sec * 1000000000) + tv.tv_nsec;
}

void log_libclo(size_t n, char *message, int code, char *file, char *func, int line)
{
  char buffer[256];
  int e;
  size_t i;

  for (i = 0; i < n; i ++)
    {
      e = clo_encode((clo[]) {
          clo_object({"message", clo_string(message)},
                     {"code", clo_integer(code)},
                     {"file", clo_string(file)},
                     {"func", clo_string(func)},
                     {"line", clo_integer(line)}
                     )}, buffer, sizeof buffer);
      if (e == -1)
        err(1, "clo_encode");
      puts(buffer);
    }
}

void log_jansson(size_t n, char *message, int code, char *file, char *func, int line)
{
  char *string;
  json_t *o;
  size_t i;

  for (i = 0; i < n; i ++)
    {
      o = json_object();
      (void) json_object_set_new(o, "message", json_string(message));
      (void) json_object_set_new(o, "code", json_integer(code));
      (void) json_object_set_new(o, "file", json_string(file));
      (void) json_object_set_new(o, "func", json_string(func));
      (void) json_object_set_new(o, "line", json_integer(line));
      string = json_dumps(o, JSON_COMPACT);
      json_decref(o);
      puts(string);
      free(string);
    }
}

void log_cJSON(size_t n, char *message, int code, char *file, char *func, int line)
{
  char *string;
  cJSON *root;
  size_t i;

  for (i = 0; i < n; i ++)
    {
      root = cJSON_CreateObject();
      cJSON_AddStringToObject(root, "message", message);
      cJSON_AddNumberToObject(root, "code", code);
      cJSON_AddStringToObject(root, "file", file);
      cJSON_AddStringToObject(root, "func", func);
      cJSON_AddNumberToObject(root, "line", line);
      string = cJSON_Print(root);
      cJSON_Delete(root);
      puts(string);
      free(string);
    }
}

int main(int argc, char **argv)
{
  struct {
    void (*log)(size_t, char *, int, char *, char *, int); char *name;
  } variant[] = {{log_libclo, "libclo"}, {log_jansson, "jansson"}, {log_cJSON, "cJSON"}, {0}}, *v;
  size_t n = argc == 2 ? strtol(argv[1], NULL, 0) : 1;
  int e;
  uint64_t n1, n2;

  e = open("nofile", O_RDONLY);
  if (e == 0)
    errx(1, "open succeded");

  puts("starting benchmark");
  for (v = variant; v->log; v ++)
    {
      n1 = ntime();
      v->log(n, "error", errno, (char *) __FILE__, (char *) __func__, (int) __LINE__);
      n2 = ntime();
      (void) fprintf(stderr, "%16s: %16g ns\n", v->name, (double) (n2 - n1) / n);
    }
}
