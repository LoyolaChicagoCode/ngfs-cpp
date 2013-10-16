// hello.cpp
#include "ngfs.hpp"

#include <string>
#include <sstream>
#include <ctime>

using namespace std;

string itos(int i) {
  stringstream s;
  s << i;
  return s.str();
}

// Constructor
NumguessFS::NumguessFS () :
  m_strNumguessFS ("asdf"),
  m_strPath ("/" + itos(rand() % 100 + 1)) {
        // all we're doing is initialize the member variables
}

int NumguessFS::getattr (const char *path, struct stat *stbuf) {
        int res = 0;
        string strPath (path); // convert the path into a std::string

        // Zero out the file stat buffer
        memset (stbuf, 0, sizeof (struct stat));
        if (strPath == "/") {
                stbuf->st_mode = S_IFDIR | 0755;
                stbuf->st_nlink = 2;
        }
        // Compare the private variable of the NumguessFS object to the
        // passed-in path parameter from fuse
        else if ("/1" <= strPath && strPath <= "/99") {
                stbuf->st_mode = S_IFREG | 0444; // read-only
                stbuf->st_nlink = 1;
                stbuf->st_size = 50;
        }
        else {
                res = -ENOENT;
        }

        return res;
}

int NumguessFS::readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info *fi) {
        string strPath (path);
        (void) offset;
        (void) fi;

        if(strPath != "/")
                return -ENOENT;

        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        // self->m_strPath contains the std::string "/hello"
        // Applying .c_str() returns a const char * containing "/hello"
        // Adding 1 to that pointer skips the '/' character and thus outputs
        // just "hello"
        filler(buf, self->m_strPath.c_str() + 1, NULL, 0);

        return 0;
}

int NumguessFS::open(const char *path, struct fuse_file_info *fi) {
        string strPath (path);

        // The only file that exists is "/hello"
        if(strPath < "/1" || "/99" < strPath)
                return -ENOENT;

        // Only allow read-only access
        if((fi->flags & 3) != O_RDONLY)
                return -EACCES;

        return 0;
}

int NumguessFS::read(const char *path, char *buf, size_t size, off_t offset,
                struct fuse_file_info *fi) {
        string strPath (path);
        size_t len;
        (void) fi;

	const string tooLow ("try higher\n");
	const string tooHigh ("try lower\n");
	const string correct ("you got it\n");

	string content (correct);
	
        // The only file that exists is "/hello"
        if(strPath < "/1" || "/99" < strPath)
                return -ENOENT;

	if("/1" <= strPath && strPath < self->m_strPath)
	  content = tooLow;
	else if(self->m_strPath < strPath && strPath <= "/99")
	  content = tooHigh;

        len = content.length ();
        if (offset < len) {
                if (offset + size > len)
                        size = len - offset;
                memcpy (buf, content.c_str () + offset, size);
        } else
                size = 0;

        return size;
}
