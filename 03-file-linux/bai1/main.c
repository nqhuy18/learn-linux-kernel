#include <stdio.h>
#include <dirent.h>
int main() {
   DIR *dir;
   struct dirent *entry;
   // Open the directory
   dir = opendir("/home/nqh/Downloads/training_linux_embedded-master");
   if (dir == NULL) {
       perror("opendir");
       return 1;
   }
   // Read entries from the directory
   while ((entry = readdir(dir)) != NULL) {
       printf("Name: %s, size: %zu\n", entry->d_name, entry->d_reclen);
   }
   // Close the directory
   closedir(dir);
   return 0;
}