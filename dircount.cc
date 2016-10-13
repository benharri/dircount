#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

void listdir (const char *name, int level) {
  DIR *dir;
  struct dirent *entry;

  if (!(dir = opendir(name)))
    return;
  if (!(entry = readdir(dir)))
    return;

  do {
    if (entry->d_type == DT_DIR) {
      // DT_LNK
      char path[4096];
      int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
      path[len] = 0;
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;
      printf("%*s[%s]\n", level*2, "", entry->d_name);
      listdir(path, level + 1);
    }
    else if (entry->d_type == DT_LNK) {
      // rip in pieces
    }
    else
      printf("%*s- %s\n", level*2, "", entry->d_name);
  } while (entry = readdir(dir));
  closedir(dir);
}

int main (void) {
  listdir(".", 0);
  return 0;
}
