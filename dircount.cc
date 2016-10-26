#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unordered_map>
using namespace std;

int file_cnt = 0, link_cnt = 0, dir_cnt = 0;
unsigned long space_used = 0;
struct stat buf;
unordered_map<ino_t, bool> ht;

void listdir (const char *name) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name))) return;
  if (!(entry = readdir(dir))) return;

  do {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
    if (ht[entry->d_ino]) continue;
    if (entry->d_type == DT_DIR) {
      dir_cnt++;
      char path[4096];
      int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
      path[len] = 0;
      printf("[d] %s/%s\n", name, entry->d_name);
      listdir(path);
    }
    else {
      if (entry->d_type == DT_LNK) {
        link_cnt++;
        printf("[l] %s/%s\n", name, entry->d_name);
      }
      else{
        file_cnt++;
        char path[4096];
        int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
        path[len] = 0;
        stat(path, &buf);
        space_used += buf.st_blocks;
        printf("[f] %lu\t%s/%s\n", buf.st_blocks, name, entry->d_name);
      }
    }
    ht[entry->d_ino] = true;
  } while ((entry = readdir(dir)));
  closedir(dir);
}

int main (int argc, char** argv) {
  char buf[PATH_MAX + 1];
  char *dirpath = (argc > 1) ? realpath(argv[1], buf) : realpath(".", buf);
  listdir(dirpath);
  printf("\ntotals\nfile count: %d\tdir count: %d\tlink count: %d\nspace used: %lu blocks\n\t%lu bytes\n", file_cnt, dir_cnt, link_cnt, space_used, space_used*512);
  return 0;
}
