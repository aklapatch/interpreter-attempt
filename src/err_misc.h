
#define PERROR(msg) printf("%s %u %s\n", __FILE__, __LINE__, msg); perror("Error: ")

#define ERROR(msg) printf("%s %u %s\n", __FILE__, __LINE__, msg)
