#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
 
#define TIME_BUF_SIZE 64
 

static void print_usage(const char *prog_name)
{
    fprintf(stderr, "Usage: %s <file_path>\n", prog_name);
}
 
static const char *get_file_type(mode_t mode)
{
    if (S_ISREG(mode)) {
        return "Regular File";
    }
    if (S_ISDIR(mode)) {
        return "Directory";
    }
    if (S_ISLNK(mode)) {
        return "Symbolic Link";
    }

    return "Unknown";
}
 

static int format_time(time_t t, char *buf, size_t buf_size)
{
    struct tm tm_info;
 
    if (localtime_r(&t, &tm_info) == NULL) {
        return -1;
    }
    if (strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S", &tm_info) == 0) {
        return -1;
    }
    return 0;
}
 
int main(int argc, char *argv[])
{
    struct stat file_info;
    char time_str[TIME_BUF_SIZE];
    const char *path;
 
    if (argc != 2) {
        if (argc > 2) {
            fprintf(stderr, "Error: too many arguments (got %d, expected 1)\n",
                    argc - 1);
        } else {
            fprintf(stderr, "Error: missing file path argument\n");
        }
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
 
    path = argv[1];
 
    if (lstat(path, &file_info) == -1) {
        perror(path);
        return EXIT_FAILURE;
    }
 
    if (format_time(file_info.st_mtime, time_str, sizeof(time_str)) == -1) {
        fprintf(stderr, "Error: cannot convert modification time\n");
        return EXIT_FAILURE;
    }
 
    printf("File Path     : %s\n", path);
    printf("File Type     : %s\n", get_file_type(file_info.st_mode));
    printf("Size          : %lld bytes\n", (long long)file_info.st_size);
    printf("Last Modified : %s\n", time_str);
 
    return EXIT_SUCCESS;
}