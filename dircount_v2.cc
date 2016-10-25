#include <dirent.h> 
#include <string> 
#include <sys/stat.h>
#include <unordered_map>
#include <iostream> 
using namespace std;

class Hashtable {
  unordered_map<const ino_t*, bool> ht;
public:
  void put(const ino_t* key, bool value) { ht[key] = value; }
  const bool get(const ino_t* key) { return ht[key]; }
};

int dircnt = 0, filecnt = 0, lnkcnt = 0;
unsigned long space_used;
struct stat buf;
Hashtable ht;

bool listFileAndType( const string &dir ) { 
  DIR *dirp = opendir( dir.c_str() ); 

  if ( dirp ) { 
    struct dirent *dp = 0; 

    while ( (dp = readdir( dirp )) != 0 ) { 
      string file( dp->d_name ); 

      if ( file == "." || file == ".." )   // skip these 
        continue; 

      if ( dp->d_type == DT_DIR ) { 
        string dirPath = dir + "/" + file; 

        cout << "[d] " << file << endl; 
	dircnt++;
        // recurse into directory 
        listFileAndType( dirPath ); 
      } 
      else { 
	//filecnt++;
	if (ht.get(&dp->d_ino)) continue;	
        switch( dp->d_type ) { 
          case DT_REG: 
	      filecnt++;
              cout << "[f] "; 
              break; 
          case DT_LNK: 
              lnkcnt++;
              cout << "[l] "; 
              break; 
	  default:
	      cout << "[none] ";
	      break;
        } 
	string statpath = dir + "/" + file;
        ht.put(&dp->d_ino, true);
	stat(statpath.c_str(), &buf);
	space_used += buf.st_blocks * 512;

        cout << dir << "/" << file << ": "; 
        cout << endl; 
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
  const string dir = (argc > 1 ? argv[1] : "foo"); 

  if (!listFileAndType(dir)) { 
    cout << "Error:  Cannot open directory '" << dir << "'" << endl; 
  } 
  cout << endl << "file count: " << filecnt << "\tdir cnt: " << dircnt << "\tlink cnt: " << lnkcnt << endl;
  cout << "space used: " << space_used << endl;
  return 0; 
}  
