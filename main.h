char *get_current_dir(char s[], int size);
void echo_out(const char *command, int firstSpaceIndex);
void delete_file(char command[], int firstSpaceIndex);
void copyFile(const char *src, const char *dest);
void displayFiles(const char* dirname);
void change_directory(const char* dirname, char* prev_dir);
int findFirstSpace(const char* cmd);
const char *findCMD(const char* cmd,int firstSpaceIndex);