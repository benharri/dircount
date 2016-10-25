#include <dirent.h>
#include <string>
#include <sys/stat.h>
#include <unordered_map>
#include <iostream>
using namespace std;

int dircnt = 0, filecnt = 0, lnkcnt = 0;
unsigned long space_used;
struct stat buf;
unordered_map<ino_t, bool> ht;

bool listFileAndType(const string &dir) {
  DIR *dirp = opendir(dir.c_str());

  if (dirp) {
    struct dirent *dp = 0;

    while ((dp = readdir(dirp)) != 0 ) {
      string file( dp->d_name );

      if (file == "." || file == "..") continue;

      if (dp->d_type == DT_DIR) {
        string dirPath = dir + "/" + file;
        cout << "[d] " << dirPath << endl;
        dircnt++;
        // recurse into directory
        listFileAndType( dirPath );
      }
      else {
        if (ht[dp->d_ino]) continue;
        string statpath;
        switch(dp->d_type) {
          case DT_REG:
            filecnt++;
            cout << "[f] ";
            statpath = dir + "/" + file;
            stat(statpath.c_str(), &buf);
            space_used += buf.st_blocks;
            break;
          case DT_LNK:
            lnkcnt++;
            cout << "[l] ";
            break;
          default:
            cout << "[none] ";
            break;
        }
        ht[dp->d_ino] = true;

        cout << statpath << endl;
      }
    }

    closedir( dirp );

    return true;
  }
  else {
    return false;
  }
}


int main( int argc, char **argv ) {
  const string dir = (argc > 1 ? argv[1] : ".");

  if (!listFileAndType(dir)) {
    cout << "Error:  Cannot open directory '" << dir << "'" << endl;
  }
  cout << endl << "totals" << endl;
  cout << "file count: " << filecnt << "\tdir cnt: " << dircnt << "\tlink cnt: " << lnkcnt << endl;
  cout << "space used: " << space_used << " blocks" << endl;
  cout << "\t" << space_used*512 << " bytes" << endl;
  return 0;
}
