#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

void copie(int op_source, int op_dest);

int copie_dir(DIR * dir_source,char * source,char * destination);

int cp_main(int argc,char* argv[]);

