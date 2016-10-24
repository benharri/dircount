#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unordered_map>
using namespace std;

class Hashtable {
  unordered_map<const ino_t*, bool> htmap;
public:
  void put(const ino_t* key, bool value) {
    htmap[key] = value;
  }
  const bool get(const ino_t* key) {
    return htmap[key];
  }
};

int file_cnt = 0, link_cnt = 0, dir_cnt = 0;
unsigned long space_used = 0;
struct stat buf;
Hashtable ht;

void listdir (const char *name) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name))) return;
  if (!(entry = readdir(dir))) return;

  do {
    if (entry->d_type == DT_LNK) {
      link_cnt++;
      printf("there was a link");
      // RIP in pieces
      char path[4096];
      int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
      path[len] = 0;
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

      printf("[l] %s/%s\n", name, entry->d_name);
      listdir(path);
    }
    else if (entry->d_type == DT_DIR) {
      dir_cnt++;
      char path[4096];
      int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
      path[len] = 0;
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
      // printf("%*s[%s]\n", level*2, "", entry->d_name);
      printf("[d] %s/%s\n", name, entry->d_name);
      listdir(path);
    }
    else{
      file_cnt++;
      if (ht.get(&entry->d_ino)) continue;
      ht.put(&entry->d_ino, true);
      printf("[f] %d %s/%s\n", (int)entry->d_ino, name, entry->d_name);

      char path[4096];
      int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
      path[len] = 0;
      stat(path, &buf);
      space_used += buf.st_blocks * 512;
    }
  } while (entry = readdir(dir));
  closedir(dir);
}

int main (int argc, char** argv) {
  listdir(argv[1]);
  printf("\ntotals\nfile count: %d\tdir count: %d\tlink count: %d\nspace used: %lu\n", file_cnt, dir_cnt, link_cnt, space_used);
  return 0;
}
