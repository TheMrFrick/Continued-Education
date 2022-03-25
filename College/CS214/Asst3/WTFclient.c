#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <dirent.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <sys/sendfile.h>

struct stat st = {0};

int decompress_one_file(char *infilename, char *outfilename);
int compress_one_file(char *infilename, char *outfilename);
void error(const char *msg);
char *concat(char *str1, char buf);
char *listdir(char *dir, char *file_name);
char *checkForExistence(char *dirName, char *file_name, int ispath);
int check_file_existence(char *file_name);
unsigned char *hash_file(char *file);
char *hash_to_str(unsigned char *hash);
char *protocol(char *command, char *proj_name, char *ver);
void *clientThread(void *arg, int argc, char **argv);

int decompress_one_file(char *infilename, char *outfilename)
{
    gzFile infile = gzopen(infilename, "rb");
    int out = open(outfilename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    // FILE *outfile = fopen(outfilename, "wb");
    if (!infile)
    {
        return -1;
    }

    char buffer[2];
    int num_read = 0;
    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0)
    {
        // fwrite(buffer, 1, num_read, outfile);
        write(out, buffer, sizeof(buffer));
    }

    gzclose(infile);
    close(out);
}

int compress_one_file(char *infilename, char *outfilename)
{
    int fd = open(infilename, O_RDONLY);
    // FILE *infile = fopen(infilename, "rb");
    // int out = open(outfilename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    gzFile outfile = gzopen(outfilename, "wb");
    if (!outfile)
    {
        return -1;
    }
    char inbuffer[2];
    int num_read = 0;
    unsigned long total_read = 0, total_wrote = 0;
    while ((num_read = read(fd, inbuffer, sizeof(inbuffer))) > 0)
    {
        total_read += num_read;
        gzwrite(outfile, inbuffer, num_read);
    }
    close(fd);
    gzclose(outfile);

}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

char *concat(char *str1, char buf)
{
    int length = strlen(str1);
    char *str2 = (char *)malloc(length + 1);
    strcpy(str2, str1);
    str2[length] = buf;
    str2[length + 1] = '\0';
    str1 = str2;
    return str1;
}

//Checks to see if the file existing on the client
char *listdir(char *dir, char *file_name)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char *subdir;
    dp = opendir(dir);

    if (dp == NULL)
    {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return NULL;
    }

    while ((entry = readdir(dp)) != NULL)
    {

        int lstat_int;
        //puts("here1");
        //printf("%s/%s\n", dir, entry->d_name);
        char *temp;
        if (strcmp(".Manifest", entry->d_name) != 0 || strcmp(".Update", entry->d_name) != 0 || strcmp(".", entry->d_name) != 0 || strcmp("..", entry->d_name) != 0)
        {
            temp = (char *)malloc(sizeof(char) * strlen(dir) + strlen(entry->d_name) + 10);
            strcat(temp, dir);
            strcat(temp, "/");
            strcat(temp, entry->d_name);
            strcat(temp, "/");
            lstat_int = lstat(temp, &statbuf);
        }
        else
        {
            lstat_int = lstat(entry->d_name, &statbuf);
        }

        if (lstat_int == 0)
        {
            //puts("here2");
            if (statbuf.st_mode & S_IFDIR)
            {
                /* Found a directory, but ignore . and .. */
                if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                {
                    continue;
                }
                //puts("here3?");

                // allocate space for parent directory, "/", subdir, plus NULL terminator
                subdir = malloc(strlen(dir) + strlen(entry->d_name) + 2);
                // Concatenate directory name
                strcpy(subdir, dir);
                strcat(subdir, "/");
                strcat(subdir, entry->d_name);

                char *temp2 = (char *)malloc(strlen(subdir) + strlen(file_name) + 1);
                strcat(temp2, subdir);
                strcat(temp2, "/");
                strcat(temp2, file_name);

                if (check_file_existence(temp2))
                {
                    printf("%s\n", temp2);
                    //free(temp2);
                    //free(dir);
                    //closedir(dp);
                    return temp2;
                }
                else
                {
                    free(temp2);
                }

                /* Recurse at a new indent level */
                listdir(subdir, file_name);
                free(subdir);
            }
        }
    }

    closedir(dp);
    return NULL;
}

int checkDirExistence(char *dirName)
{
    char *temp = (char *)(malloc(strlen(dirName) + 8));
    strcat(temp, "server/");
    strcat(temp, dirName);
    if (stat(temp, &st) == -1)
    {
        free(temp);
        return 0;
    }
    else
    {
        free(temp);
        return 1;
    }
}

int checkClientExistence(char *dirName)
{
    char *temp = (char *)(malloc(strlen(dirName) + 8));
    strcat(temp, "client/");
    strcat(temp, dirName);
    if (stat(temp, &st) == -1)
    {
        free(temp);
        return 0;
    }
    else
    {
        free(temp);
        return 1;
    }
}

char *checkForExistence(char *dirName, char *file_name, int ispath)
{
    char *temp = (char *)(malloc(strlen(dirName) + 8));
    strcat(temp, "client/");
    strcat(temp, dirName);
    if (stat(temp, &st) == -1)
    {
        free(temp);
        return NULL;
    }
    else
    {
        if (!ispath)
        {
            char *temp2 = (char *)malloc(strlen(temp) + strlen(file_name) + 1);
            strcat(temp2, temp);
            strcat(temp2, "/");
            strcat(temp2, file_name);

            if (check_file_existence(temp2))
            {
                //free(temp);
                //free(temp2);
                return temp2;
            }
            else
            {
                char *file_path = listdir(temp, file_name);

                if (file_path == NULL)
                {
                    return NULL;
                }
                else
                {
                    return file_path;
                }
            }
        }
        else
        {
            if (check_file_existence(file_name))
            {
                return file_name;
            }
            else
            {
                return NULL;
            }
        }
    }
}

int check_file_existence(char *file_name)
{

    if (access(file_name, F_OK) != -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Method to hash a file */
unsigned char *hash_file(char *file)
{

    char *hash_message = 0;
    long length;
    int fd = open(file, O_RDONLY);
	length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, 0);
	hash_message = malloc(sizeof(char) * length);
	
	int status = 0;
        int bytes;
        bytes = 0;
        char buff;
        char *temp;
        temp = "";
        
        do
        {
            status = read(fd, &buff, 1);

            //file did not exist in the .Manifest file
            if (bytes == length)
            {
              	hash_message = temp;
                break;
            }
            bytes++;


            temp = concat(temp, buff);
        } while (status > 0);
        close(fd);
	    
    //printf("contents: %s\n", hash_message);

    //Hash the contents
    unsigned char *hash;
    hash = (char *)malloc(sizeof(char) * 256);
    SHA256(hash_message, strlen(hash_message), hash);

    //Printing out the hash code
    /*int i;
    for (i = 0; i < strlen(hash); i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
	*/
    return hash;
}

/* Convert the hash value to a string*/
char *hash_to_str(unsigned char *hash)
{

    char **temp_hash_string = (char **)malloc(sizeof(char *) * 256); //used to conver the SHA256 Hash
    int i;
    for (i = 0; i < 256; i++)
    {
        temp_hash_string[i] = (char *)malloc(sizeof(char));
    }

    //Convert SHA has
    for (i = 0; i < strlen(hash); i++)
    {
        sprintf(temp_hash_string[i], "%02x", hash[i]);
    }
    

    //Copy the converted hash to a string
    char *hash_string = (char *)malloc(sizeof(char) * 257);
    for (i = 0; i < 32; i++)
    {
        strcat(hash_string, temp_hash_string[i]);
    }
    hash_string[256] = '\0';
    
    for (i = 0; i < 256; i++)
    {
        free(temp_hash_string[i]);
    }
    free(temp_hash_string);

    //printf("hash_string: %s\n", hash_string);

    return hash_string;
}

/* Produces protocol message to send to server */
char *protocol(char *command, char *proj_name, char *ver)
{
    //Determines how many colons there will be in the message
    int colons = 2;
    if (atoi(ver) > -1)
    {
        colons += 1;
    }

    //Convert project length to a string
    char *proj_len = (char *)malloc(2);
    sprintf(proj_len, "%ld", strlen(proj_name));
    strcat(proj_len, "\0");

    //Proto contains the protocol message
    char *proto = (char *)malloc(strlen(command) + strlen(proj_name) + colons + strlen(proj_len) + strlen(ver));

    strcpy(proto, command);
    strcat(proto, ":");
    strcat(proto, proj_len); //number of bytes
    strcat(proto, ":");
    strcat(proto, proj_name);

    if (colons == 3)
    {
        proto = strcat(proto, ":");
        proto = strcat(proto, ver);
    }

    //printf("protocol: %s\n", proto);
    free(proj_len);
    return proto;
}

void receive_message(int sock_fd)
{
    int bytes_receive;
    char buffer[BUFSIZ];
    recv(sock_fd, buffer, BUFSIZ, 0);
    printf("%s\n", buffer);
    bytes_receive = atoi(buffer) - 1;
    printf("%d\n", bytes_receive);

    char buf[bytes_receive + 1];
    recv(sock_fd, &buf, (bytes_receive), 0);
    buf[sizeof(buf)] = '\0';
    printf("%s\n", buf);
}

/*Method for receiving a file*/
void receive_file(int sock_fd, char *proj, int op)
{

    char buffer[BUFSIZ];
    int file_size;
    int received_file;
    int remain_data = 0;
    int len;

    //Receiving file name
    recv(sock_fd, buffer, BUFSIZ, 0);
    printf("file name: %s\n", buffer);

    char *file_name = (char *)malloc(sizeof(char) * strlen(buffer));
    strcpy(file_name, buffer);
    puts(file_name);

    char *received_file_path;
    if (op == 1)
    { //put into the same directory
        //Create the file path for the file
        received_file_path = (char *)malloc(sizeof(char) * strlen(file_name) + 12 + strlen(proj));
        strcpy(received_file_path, "./client/");
        strcat(received_file_path, proj);
        strcat(received_file_path, "/");
        strcat(received_file_path, file_name);
        strcat(received_file_path, ".z");
    }
    else
    { //don't put into the same directory
        received_file_path = (char *)malloc(sizeof(char) * strlen(file_name) + 4);
        strcpy(received_file_path, "./");
        strcat(received_file_path, file_name);
        strcat(received_file_path, ".z");
    }
    received_file = open(received_file_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    puts(received_file_path);

    //Receiving file size
    recv(sock_fd, buffer, BUFSIZ, 0);
    file_size = atoi(buffer);
    remain_data = file_size;

    //While there is data to be obtained
    while (remain_data > 0)
    {
        len = recv(sock_fd, buffer, BUFSIZ, 0);
        if (len > 0)
        {
            write(received_file, buffer, len);
            remain_data -= len;
        }
        else
        {
            printf("len < 0\n");
            break;
        }
        sleep(2);
    }

    close(received_file);

    char *decompress_file = (char *)malloc(sizeof(char) * strlen(received_file_path) + 3);

    if (op == 1)
    { //same directory
        decompress_file = (char *)malloc(sizeof(char) * strlen(received_file_path) + 3);
        strcpy(decompress_file, "./client/");
        strcat(decompress_file, proj);
        strcat(decompress_file, "/");
        strcat(decompress_file, file_name);
    }
    else
    {
        decompress_file = (char *)malloc(sizeof(char) * strlen(received_file_path) + 2);
        strcpy(decompress_file, "./");
        strcat(decompress_file, file_name);
    }

    puts(received_file_path);
    decompress_one_file(received_file_path, decompress_file);

    int status = remove(received_file_path);
    if (status == 0)
    {
        puts("file removed");
    }

    free(file_name);
    free(received_file_path);
    free(decompress_file);
}

/*Method for the commands add and remove*/
void add_remove(char *arg1, char *arg2, int op)
{

    //op = 1 --> add
    //op = 2 --> remove

    char *file_path;
    int ispath = 0;

    if (strstr(arg2, "/") != NULL)
    {

        //checking to see if "client/" is in the path or not
        if (strstr(arg2, "client/") == NULL)
        {
            char *temp = (char *)malloc(sizeof(char) * (strlen(arg1) + strlen(arg2) + 8));
            strcat(temp, "client/");
            strcat(temp, arg1);
            strcat(temp, "/");
            strcat(temp, arg2);
            arg2 = (char *)malloc(sizeof(char) * strlen(temp));
            strcat(arg2, temp);
            free(temp);
            puts(arg2);
        }

        ispath = 1;
    }

    if ((file_path = checkForExistence(arg1, arg2, ispath)) != NULL)
    { //filename has no path, need to find
        puts(file_path);
        puts("exists");

        char *name;
        name = (char *)malloc(sizeof(char) * (10 + 7 + strlen(arg1)));
        strcat(name, "client/");
        strcat(name, arg1);
        strcat(name, "/.Manifest");
        puts(name);
        int fd = open(name, O_RDWR);
        int end; //how many bytes in file, know where to end at

        //file descriptor -- grabbing the end bytes
        struct stat fpStats;
        if (stat(name, &fpStats) == -1)
        {
            printf("Error: .Manifest file failed to open.\n");
            exit(0);
        }
        else
        {
            end = fpStats.st_size;
        }

        //variables used for reading the file
        int status = 0;
        int bytes;
        bytes = 0;
        char buff;
        char *temp;
        temp = "";
        int bytes_holder[4];
        int counter = 1;
        char *ver_no;
        ver_no = (char *)malloc(sizeof(char) * 1);

        /* Bytes holder:

		 0 -- version num
		 1 -- symbolic symbol (lol)
		 2 -- path
		 3 -- hash*/

        unsigned char *new_file_hash = hash_file(file_path); //new hash value for the file
        char *hash_string = malloc(sizeof(char) * 256);
        hash_string = hash_to_str(new_file_hash); //the string version of the hash
        free(new_file_hash);
        do
        {
            status = read(fd, &buff, 1);

            //file did not exist in the .Manifest file
            if (bytes == end)
            {
                write(fd, "1 + ", strlen("1 + "));
                write(fd, file_path, strlen(file_path));
                write(fd, " ", strlen(" "));
                write(fd, hash_string, strlen(hash_string));
                write(fd, "\n", strlen("\n"));
                free(hash_string);
                break;
            }
            bytes++;

            if (buff == '\n' || buff == ' ')
            {
            
            	if(bytes == 2){ //version number, don't want
            		continue;
            	}

                if (bytes != 1)
                { //ignores version number at top, so byte can't equal 1

                    //Assigns where the bytes are for the version, symbol, path, etc.
                    if (counter == 1)
                    {
                        bytes_holder[0] = bytes - 2;
                        counter++;
                    }
                    else if (counter == 2)
                    {
                        bytes_holder[1] = bytes - 2;
                        counter++;
                    }
                    else if (counter == 3)
                    {
                        bytes_holder[2] = bytes - 2;
                        counter++;
                    }
                    else if (counter == 4)
                    {
                        bytes_holder[3] = bytes - 2;
                        counter = 1; //reset
                    }
                }

                //check to see if we are at the file path
                if (strcmp(file_path, temp) == 0)
                {

                    if (op == 1)
                    { //add file
                        printf("Warning: This file has already been added. Rewriting over the current hash.\n");

                        //increment version number of file
                        if (lseek(fd, bytes_holder[0], SEEK_SET) == -1)
                        {
                            printf("Error performing lseek\n");
                            break;
                        }
                        else
                        {
                            read(fd, &buff, 1);
                            int verno = buff - '0';
                            verno++;
							char* ver;
							ver = (char*) malloc(sizeof(char) * 4);
							sprintf(ver, "%d", verno);
                            lseek(fd, bytes_holder[0], SEEK_SET);
                            write(fd, ver, strlen(ver));
                            free(ver);
                        }
                        
                        //change symbol to +
                        if (lseek(fd, bytes_holder[1], SEEK_SET) == -1)
                        {
                            printf("Error performing lseek\n");
                            break;
                        }
                        else
                        {
                        	lseek(fd, bytes_holder[1], SEEK_SET);
                            write(fd, "+", strlen("+"));
                        }

                        //write over hashcode
                        if (lseek(fd, bytes, SEEK_SET) == -1)
                        {
                            printf("Error performing lseek\n");
                            break;
                        }
                        else
                        {
                            //goes to the location and writes the new hash code
                            lseek(fd, bytes, SEEK_SET);
                            write(fd, hash_string, strlen(hash_string));
                            break;
                        }
                    }
                    else if (op == 2)
                    {
                        if (lseek(fd, bytes_holder[1], SEEK_SET) == -1)
                        {
                            printf("Error performing lseek\n");
                            break;
                        }
                        else
                        {
                            //goes to the location and writes the new hash code
                            write(fd, "!", strlen("!"));
                            break;
                        }
                    }
                }
                else
                {
                    temp = "";
                    continue;
                }
            }
            temp = concat(temp, buff);
        } while (status > 0);
        close(fd);
        return;
    }
    else
    {
        printf("Error: Project or file does not exists locally.\n");
        return;
    }
}

void *clientThread(void *arg, int argc, char **argv)
{
    int sock_fd;         //socket fd
    int portno;          //port number
    int con;             //connection
    int x;               //holds write info
    char *command;       //holds the command passed in
    char *arg1;          //either project name or ip address (if configure command)
    char *arg2;          //either file name, version, or port
    char *proto_message; //holds protocol message
    char buffer[256];
    struct sockaddr_in server_addr;
    struct hostent *server;

    // Defintion of a struct hostent:
    struct hostent
    {
        char *h_name;       //name of the host
        char **h_aliases;   //alias list
        int h_addrtype;     //host address type
        int h_length;       //length of the addr
        char **h_addr_list; //list of addresses from name server
#define h_addr h_addr_list[0]
    };

    //checking usage
    if (argc < 3)
    {
        printf("Error: not enough arguments.\nUsage: hostname/ip address port\n");
        exit(0);
    }

    //grab client command
    command = (char *)malloc(sizeof(argv[1]));
    command = argv[1];
    //command[strlen(command)] = '\0';

    //only project name commands
    //command arg & project name
    if (argc == 3)
    {
        arg1 = (char *)malloc(sizeof(argv[2]));
        arg1 = argv[2];
    }
    else if (argc == 4)
    { //two args after command
        arg1 = (char *)malloc(sizeof(argv[2]));
        arg1 = argv[2];

        arg2 = (char *)malloc(sizeof(argv[3]));
        arg2 = argv[3];
    }

    //These commands do not require a connection to the server
    if (strcmp(command, "add") == 0)
    {

        add_remove(arg1, arg2, 1);
        exit(0);
    }
    else if (strcmp(command, "remove") == 0)
    {

        add_remove(arg1, arg2, 2);
        exit(0);
    }

    //configure - first command for client
    if (strcmp(command, "configure") == 0)
    {
        if (check_file_existence("./.configure"))
        {
            int status = remove("./.configure");
            if (status == 0)
            {
                puts("file removed");
            }
        }
        int configure;
        char *name;
        name = (char *)malloc(sizeof(char) * 12);
        name = strcpy(name, "./.configure");
        //create the ./.configure file
        configure = open(name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        write(configure, arg1, strlen(arg1));
        write(configure, "\n", strlen("\n"));
        write(configure, arg2, strlen(arg2));
        free(name);
        close(configure);
        puts("Configure file successfully created.");
        pthread_exit(NULL);
        // exit(1); //end program after configure
    }

    //go through configure file to initalize connection to server
    char *name;
    name = (char *)malloc(sizeof(char) * 12);
    name = strcpy(name, "./.configure");
    int fd = open(name, O_RDONLY); //opening configure file
    int end;                       //how many bytes in file, know where to end at

    //file descriptor -- grabbing the end bytes
    struct stat fpStats;
    if (stat(name, &fpStats) == -1)
    {
        printf("Error: configure file failed to open. Please run configure before using other commands.\n");
        exit(0);
    }
    else
    {
        end = fpStats.st_size;
    }

    //variables used for reading the file
    int status;
    status = 0;
    int bytes;
    bytes = 0;
    char buff;
    char *temp;
    temp = "";

    do
    {
        status = read(fd, &buff, 1);
        //printf("buffer is: %c\n", buff);
        if (bytes == end)
        {
            arg2 = temp;
            //printf("arg2: %s\n", arg2);
            break;
        }
        bytes++;

        if (buff == '\n')
        {
            if (arg1 != NULL)
            {
                arg1 = temp;
                //printf("arg1: %s\n", arg1);
                temp = "";
                continue;
            }
        }
        temp = concat(temp, buff);

    } while (status > 0);

    portno = atoi(arg2);
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("Error: socket failed to open\n");
    }

    server = gethostbyname(arg1);
    if (server == NULL)
    {
        printf("Error: no such host\n");
        exit(0);
    }

    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr,
          server->h_length);
    server_addr.sin_port = htons(portno);

    while (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        puts("Connecting...");
        sleep(3);
    }

    char message[strlen("Connected to Server") + 1];
    int in = recv(sock_fd, &message, sizeof(message), 0);
    message[strlen(message)] = '\0';
    printf("%s\n", message);
    // send(sock_fd, message, strlen(message), 0);
    // close(sock_fd);

    //reassign
    if (argc == 3)
    {
        //free(arg1);
        arg1 = (char *)malloc(sizeof(argv[2]));
        arg1 = argv[2];
    }
    { //two args after command
        //free(arg1);
       // free(arg2);
        arg1 = (char *)malloc(sizeof(argv[2]));
        arg1 = argv[2];

        arg2 = (char *)malloc(sizeof(argv[3]));
        arg2 = argv[3];
    }

    if (strcmp(command, "checkout") == 0)
    {
        /*
            The checkout command will fail:
            - If the project name doesn't exist on the server
            - The client can't communicate with the server.............check
            - If the project name already exists on the client side....check
            - Configure not run on client side.........................check
        */

        //Check to see if the project name exists on the client already
        if (checkClientExistence(arg1))
        {
            printf("ERROR:\n");
            printf("%s already exists on the client.\n", arg1);
            //    exit(1);
        }
        else
        {

            proto_message = protocol(command, arg1, "-1");

            //Send protocol message to server
            if ((send(sock_fd, proto_message, strlen(proto_message), 0)) == -1)
            {
                printf("Error: Failed to send message to server.\n");
                pthread_exit(NULL);
                exit(1);
            }
            else
            {
                printf("Client protocol message has been sent.\n");
                // receive_message(sock_fd);
                char *dir = (char *)malloc(strlen("client/") + strlen(arg1));
                strcpy(dir, "client/");
                strcat(dir, arg1);
                mkdir(dir, 0700);
                free(dir);
                char buffer[BUFSIZ];
                memset(buffer, 0, BUFSIZ);
                int f = 0; // false
                do
                {
                    memset(buffer, 0, BUFSIZ);
                    recv(sock_fd, &buffer, BUFSIZ, 0);
                    printf("%s\n", buffer);
                    if (f)
                    {
                        char delim[2] = "/";
                        char *token;
                        char *rest = NULL;
                        int last = 0;
                        int file_size;
                        int received_file;
                        int remain_data = 0;
                        int len;
                        char *rest_1;

                        // proj[0] = "";
                        token = strtok_r(buffer, delim, &rest); // token gets server -> rest gets the file path
                        // proj[last] = '\0';
                        printf("project name: %s\n", rest);
                        rest_1 = (char *) malloc(strlen(rest) + 1);
                        strcpy(rest_1, rest);
                        rest_1[strlen(rest_1)] = '\0';
                        // receive_file(sock_fd, proj, 1);
                        char *received_file_path = (char *)malloc(strlen(rest) + 13);
                        strcpy(received_file_path, "./client/");
                        strcat(received_file_path, rest);
                        // printf("Rest after cat: %s\n", rest);
                        strcat(received_file_path, ".z\0");
                        printf("received file path: %s\n", received_file_path);
                        received_file = open(received_file_path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
                        memset(buffer, 0, BUFSIZ);
                        recv(sock_fd, buffer, BUFSIZ, 0);
                        memset(buffer, 0, BUFSIZ);
                        recv(sock_fd, buffer, BUFSIZ, 0);
                        file_size = atoi(buffer);
                        printf("File size: %d\n", file_size);
                        remain_data = file_size;
                        //While there is data to be obtained
                        while (remain_data > 0)
                        {
                            memset(buffer, 0, BUFSIZ);
                            len = recv(sock_fd, buffer, BUFSIZ, 0);
                            if (len > 0)
                            {
                                write(received_file, buffer, len);
                                remain_data -= len;
                            }
                            else
                            {
                                puts("No information sent from server");
                                break;
                            }
                        }

                        close(received_file);
                        char *decompress_file = (char *)malloc(strlen(rest_1) + 11);
                        strcpy(decompress_file, "./client/");
                        strcat(decompress_file, rest_1);
                        strcat(decompress_file, "\0");
                        decompress_one_file(received_file_path, decompress_file);

                        int status = remove(received_file_path);
                        if (status == 0)
                        {
                            puts("file removed");
                        }
                        free(received_file_path);
                        free(decompress_file);
                        f = 0;
                    }
                    else if (strcmp(buffer, "file") == 0)
                    {
                        f = 1;
                    }
                    else
                    {
                        f = 0;
                        char delim[2] = "/";
                        char *token;
                        char *rest = NULL;
                        char *dtemp = (char *)malloc(strlen(buffer) + strlen("client/"));
                        token = strtok_r(buffer, delim, &rest);
                        strcpy(dtemp, "client/");
                        strcat(dtemp, rest);
                        dtemp[strlen(rest) + strlen("client/") + 1] = '\0';
                        mkdir(dtemp, 0700);
                        free(dtemp);
                    }

                    // sleep(1);
                } while (strcmp(buffer, "-1") != 0);
            }
        }
    }
    else if (strcmp(command, "update") == 0)
    {
    
        proto_message = protocol(command, arg1, "-1");

        //Send protocol message to server
        if ((send(sock_fd, proto_message, strlen(proto_message), 0)) == -1)
        {
            printf("Error: Failed to send message to server.\n");
            pthread_exit(NULL);
            exit(1);
        }
        else
        {
            printf("Client protocol message has been sent.\n");
        }

        //Receive the .Manifest file
        receive_file(sock_fd, arg1, 0);
        
        char* man_file = (char*) malloc(sizeof(char) * strlen("./.Manifest"));
	    strcpy(man_file, "./.Manifest");
	    
	    int fd = open(man_file, O_RDONLY);
	    int end = lseek(fd, 0, SEEK_END);
	    lseek(fd, 0, 0);
	    int status = 0;
		int bytes = 0;
		char buff;
		char *temp;
		temp = "";
		int counter = 1;
		
		char * serv_ver_num;
		serv_ver_num = (char*)malloc(sizeof(char) * 2);
		
		//for struct
		char* serv_file_ver;
		serv_file_ver = (char*)malloc(sizeof(char) * 2);
		int removed = 1; //1 == removed, 0 == not removed
		char* path;
		
		struct Node {
			char* path;
			char* file_ver;
			int removed;
			char hash[256];
			char* live_hash;
			char update;
			struct Node *next;
		};
		
		struct Node *serv_man;
		serv_man = NULL;
		//read through server's manifest
		puts("go through server manifest");
		
		do
		{
		    status = read(fd, &buff, 1);
		    
		    //file did not exist in the .Manifest file
		    if (bytes == end)
		    {
		        break;
		    }
		    bytes++;

	    	if (buff == '\n' || buff == ' ')
		    {
		    	
		    	//Gets current version of project
		    	if(bytes == 2){
					strcpy(serv_ver_num, temp);
		    		temp = "";
		    		continue;
		    	}
		    	
	            if (counter == 1)
	            {
					strcpy(serv_file_ver, temp);
	                counter++;
	            }
	            else if (counter == 2)
	            {
	                if(strcmp(temp, "!") == 0){
	                	removed = 1;
	                } else {
	                	removed = 0;
	                }
	                counter++;
	            }
	            else if (counter == 3)
	            {
	                path = (char*) malloc(sizeof(char) * strlen(temp));
	                strcpy(path, temp);
	                counter++;
	            }
	            else if (counter == 4)
	            {
	            	if(serv_man == NULL){
	            		serv_man = (struct Node*) malloc(sizeof(struct Node));
	            		serv_man->path = (char*) malloc(sizeof(char) * strlen(path));
	            		serv_man->file_ver = (char*) malloc(sizeof(char) * strlen(serv_file_ver));
	            		serv_man->next = NULL;
	            		strcpy(serv_man->path, path);
	            		strcpy(serv_man->file_ver, serv_file_ver);
	            		serv_man->removed = removed;
	            		strcpy(serv_man->hash, temp);
	            	} else {
	            		struct Node * tempNode = (struct Node*) malloc(sizeof(struct Node));
	            		tempNode->path = (char*) malloc(sizeof(char) * strlen(path));
	            		tempNode->file_ver = (char*) malloc(sizeof(char) * strlen(serv_file_ver));
	            		strcpy(tempNode->path, path);
	            		strcpy(tempNode->file_ver, serv_file_ver);
	            		tempNode->removed = removed;
	            		strcpy(tempNode->hash, temp);
	            		tempNode->next = serv_man;
	            		serv_man = tempNode;
	            	}
	            	free(path);
	                counter = 1; //reset
	            }
	        	temp = ""; //reset
				continue;			    
			}
			temp = concat(temp, buff);
		} while (status > 0);
	    
        status = remove(man_file);
		if (status == 0)
		{
			puts("file removed");
		}
		
		/*
		struct Node * ptr5;
		ptr5 = serv_man;
		while(ptr5 != NULL){
			printf("%d\n", ptr5->removed);
			ptr5 = ptr5->next;
		}*/
		
		//free(temp);
		close(fd);
		free(man_file);

		temp = "";		
		char* cli_man_path = (char*) malloc(sizeof(char) * strlen(arg1) + 18);
		strcpy(cli_man_path, "client/");
		strcat(cli_man_path, arg1);
		strcat(cli_man_path, "/.Manifest");
		fd = open(cli_man_path, O_RDONLY);
		
		if(fd == -1){
			printf("Error: cannot open .Manifest file\n");
			exit(0);
		}
		
		//reset variables for reading
	    end = lseek(fd, 0, SEEK_END);
	    lseek(fd, 0, 0);
	    bytes = 0;
		counter = 1;
		status = 0;
		removed = 1;
		
		char * cli_ver_num;
		cli_ver_num = (char*)malloc(sizeof(char) * 2);
		char* cli_file_ver;
		cli_file_ver = (char*)malloc(sizeof(char) * 2);
		struct Node *cli_man;
		cli_man = NULL;
		
		//read through client's manifest
		do
		{
		    status = read(fd, &buff, 1);
		    
		    //file did not exist in the .Manifest file
		    if (bytes == end)
		    {
		        break;
		    }
		    bytes++;
	    	if (buff == '\n' || buff == ' ')
		    {
		    	//printf("temp %s\n", temp);
		    	//Gets current version of project
		    	if(bytes == 2){
					strcpy(cli_ver_num, temp);
		    		temp = "";
		    		continue;
		    	}
		    	
	            if (counter == 1)
	            {
					strcpy(cli_file_ver, temp);
	                counter++;
	            }
	            else if (counter == 2)
	            {
	                if(strcmp(temp, "!") == 0){
	                	removed = 1;
	                } else {
	                	removed = 0;
	                }
	                counter++;
	            }
	            else if (counter == 3)
	            {
	                path = (char*) malloc(sizeof(char) * strlen(temp));
	                strcpy(path, temp);
	                counter++;
	            }
	            else if (counter == 4)
	            {
	            	if(cli_man == NULL){
	            		cli_man = (struct Node*) malloc(sizeof(struct Node));
	            		cli_man->path = (char*) malloc(sizeof(char) * strlen(path));
	            		cli_man->file_ver = (char*) malloc(sizeof(char) * strlen(cli_file_ver));
	            		cli_man->next = NULL;
	            		strcpy(cli_man->path, path);
	            		strcpy(cli_man->file_ver, cli_file_ver);
	            		cli_man->removed = removed;
	            		strcpy(cli_man->hash, temp);
	            		//live hash	   
						if(removed == 0){
							unsigned char *new_file_hash = hash_file(path); //new hash value for the file
							char *hash_string = (char*) malloc(sizeof(char) * 257);
							hash_string = hash_to_str(new_file_hash); //the string version of the hash
							free(new_file_hash);
							cli_man->live_hash = (char*) malloc(sizeof(char) * 257);
							strcpy(cli_man->live_hash, hash_string);
							free(hash_string);
						}     
	            		
	            	} else {
	            		struct Node * tempNode = (struct Node*) malloc(sizeof(struct Node));
	            		tempNode->path = (char*) malloc(sizeof(char) * strlen(path));
	            		tempNode->file_ver = (char*) malloc(sizeof(char) * strlen(cli_file_ver));
	            		strcpy(tempNode->path, path);
	            		strcpy(tempNode->file_ver, cli_file_ver);
	            		tempNode->removed = removed;
	            		strcpy(tempNode->hash, temp);
	            		
	            		//live hash
	            		if(removed == 0){
			        		unsigned char *new_file_hash = hash_file(path); //new hash value for the file
							char *hash_string = (char*) malloc(sizeof(char) * 257);
							hash_string = hash_to_str(new_file_hash); //the string version of the hash
							free(new_file_hash);
							tempNode->live_hash = (char*) malloc(sizeof(char) * 257);
							puts(hash_string);
							strcpy(tempNode->live_hash, hash_string);
							puts(tempNode->live_hash);
							free(hash_string);
	            		}
	            		
	            		tempNode->next = cli_man;
	            		cli_man = tempNode;
	            		
	            	}
	            	free(path);
	                counter = 1; //reset
	            }
	        	temp = ""; //reset
				continue;			    
			}
			temp = concat(temp, buff);
		} while (status > 0);
		
		puts(serv_ver_num);
		puts(cli_ver_num);
			
		//Do comparisons for U, M, A, & D --> Server's nodes
		int conflict = 0; //0 == no conflict, 1 == conflict
		struct Node * ptr;
		ptr = serv_man;
		
		while(ptr != NULL){
		
			struct Node* ptr2 = cli_man;
			
			while(ptr2 != NULL){
				
				if(strcmp(ptr->path, ptr2->path) == 0){
					//U - Upload
					if((ptr->removed == 0 && ptr2->removed == 0) && (strcmp(ptr->hash, ptr2->live_hash) != 0) && (strcmp(serv_ver_num, cli_ver_num) == 0)){
							puts("U");
							ptr2->update = 'U';
							break;
							
						} else if((ptr->removed == 1 && ptr2->removed == 0) && (strcmp(serv_ver_num, cli_ver_num) == 0)){
							puts("U");
							ptr2->update = 'U';
							break;
						}
						
						//M - Modify
						if((ptr->removed == 0 && ptr2->removed == 0) && (strcmp(serv_ver_num, cli_ver_num)!= 0) && (strcmp(ptr->file_ver, ptr2->file_ver) != 0) && (strcmp(ptr2->live_hash, ptr2->hash) == 0)){
							puts("M");
							ptr2->update = 'M';
							break;
						}
						
						//A - Add
						if((ptr->removed == 0 && ptr2->removed == 1) && (strcmp(serv_ver_num, cli_ver_num) != 0)){
							puts("A");
							ptr->update = 'A';
							break;
						}
						
						//D - Delete
						if((ptr->removed == 1 && ptr2->removed == 0) && (strcmp(serv_ver_num, cli_ver_num) != 0)){
							puts("D");
							ptr2->update = 'D';
							break;
						}
						
						//N - No action
						if((ptr->removed == 0 && ptr2->removed == 0) && (strcmp(ptr->file_ver, ptr2->file_ver) == 0)){
							puts("N");
							ptr->update = 'N';							
							ptr2->update = 'N';
						} else if((ptr->removed == 1 && ptr2->removed == 1) && (strcmp(ptr->file_ver, ptr2->file_ver) == 0)){
							puts("N");
							ptr->update = 'N';	
							ptr2->update = 'N';
						}
					}
					
					//At the end of client's .Manifest
					if(ptr2->next == NULL){
						
						//Went through entire client's .Manifest file
						//Not present in .Manifest AT ALL
						//A - Add
						if(ptr2->update != 'N'){
						
							if(strcmp(serv_ver_num, cli_ver_num) != 0){
								puts("Aa");
								ptr->update = 'A';
							} else {
								//Server has a path that is not in Client's .Manifest
								//Server's version # != client's
								//No fitting condition --> conflict
								puts("C");
								ptr->update = 'C';
								conflict = 1;
							}
						}
					}
					
				
				ptr2 = ptr2->next;
			} //end inner while
		
			ptr = ptr->next;
			
		}
		
		struct Node * c_ptr;
		c_ptr = cli_man;
		struct Node * s_ptr;
		
		while(c_ptr != NULL){
		
			s_ptr = serv_man;
			
			while(s_ptr != NULL){
				
				//Defensive checking
				if(c_ptr->update == 'N' || c_ptr->update == 'A' || c_ptr->update == 'U' || c_ptr->update == 'M' || c_ptr->update == 'D'){
					break;
				}
			
				//At the end of the server's file
				if(s_ptr->next == NULL){
					//Files not present in the server's .Manifest at all
					
					//Upload
					if((strcmp(serv_ver_num, cli_ver_num) == 0)){
						puts("U");
						c_ptr->update = 'U';
						break;
					} else if(strcmp(serv_ver_num, cli_ver_num) != 0){ //Delete
						puts("D");
						c_ptr->update = 'D';
						break;
					} else {
						puts("C");
						c_ptr->update = 'C';
						conflict = 1;
						break;
					}
					
				}
				
				//Server contains the file, goes next
				if(strcmp(s_ptr->path, c_ptr->path)){
					
					//Defensive checking
					if(c_ptr->update == 'N' || c_ptr->update == 'A' || c_ptr->update == 'U' || c_ptr->update == 'M' || c_ptr->update == 'D'){
						break;
					}
					
					if((c_ptr->removed == 0 && s_ptr->removed == 1) && (strcmp(serv_ver_num, cli_ver_num) != 0)){
						puts("D");
						c_ptr->update = 'D';
						break;
					}
					s_ptr = s_ptr->next;
					continue;
				}
			
				s_ptr = s_ptr->next;
			}	
			
			c_ptr = c_ptr->next;
			
		}
		
		s_ptr = serv_man;
		c_ptr = cli_man;
		
		//Conflict occurred
		if(conflict == 1){
			printf("Conflicts on server:\n");
			while(s_ptr != NULL){
				if(s_ptr->update == 'C'){
				
					printf("%s\n", s_ptr->path);
				
				}
				s_ptr = s_ptr->next;
			}
		
			printf("Conflict on client:\n");
			while(c_ptr != NULL){
				if(c_ptr->update == 'C'){
				
					printf("%s\n", c_ptr->path);
				
				}
				c_ptr = c_ptr->next;
			}
			
			printf("Please resolve these conflicts before updating again.\n");
		
		} else {
		
			printf("Update:\n");


			int update_fd;
		    char *name;
		    name = (char *)malloc(sizeof(char) * 18 + strlen(arg1));
		    name = strcpy(name, "./client/");
		    name = strcat(name, arg1);
		    name = strcat(name, "/.Update");
		    //create the ./.configure file
		    update_fd = open(name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
		    if(fd == -1){
		    	printf("Error: failed to create file\n");
		    	exit(0);
		    }
		    free(name);
		    
		    int written = 0;
		
			while(c_ptr != NULL){
				if(c_ptr->update == 'A' || c_ptr->update == 'D' || c_ptr->update == 'M'){
					
					//STDOUT
					printf("%c %s %s %s\n", c_ptr->update, c_ptr->file_ver, c_ptr->path, c_ptr->live_hash);
					
					//Write to .Update
					char* up_temp = (char*)malloc(sizeof(char)*2);
					up_temp[0] = c_ptr->update;
					up_temp[1] = '\0';
					write(update_fd, up_temp, strlen(up_temp));
					write(update_fd, " ", strlen(" "));
					write(update_fd, c_ptr->file_ver, strlen(c_ptr->file_ver));
					write(update_fd, " ", strlen(" "));
					write(update_fd, c_ptr->path, strlen(c_ptr->path));
					write(update_fd, " ", strlen(" "));
					write(update_fd, c_ptr->live_hash, strlen(c_ptr->live_hash));
					write(update_fd, "\n", strlen("\n"));
					free(up_temp);
					written = 1;
				}
				
				c_ptr = c_ptr->next;
			}
			
			while(s_ptr != NULL){
				if(s_ptr->update == 'A' || s_ptr->update == 'D'  || s_ptr->update == 'M'){
				
					printf("%c %s %s %s\n", s_ptr->update, s_ptr->file_ver, s_ptr->path, s_ptr->hash);
					
					puts("here");
					char* up_temp = (char*)malloc(sizeof(char)*2);
					up_temp[0] = s_ptr->update;
					up_temp[1] = '\0';
					write(update_fd, up_temp, strlen(up_temp));
					write(update_fd, " ", strlen(" "));
					write(update_fd, s_ptr->file_ver, strlen(s_ptr->file_ver));
					write(update_fd, " ", strlen(" "));
					write(update_fd, s_ptr->path, strlen(s_ptr->path));
					write(update_fd, " ", strlen(" "));
					write(update_fd, s_ptr->hash, strlen(s_ptr->hash));
					write(update_fd, "\n", strlen("\n"));
					free(up_temp);
					written = 1;
				}
				s_ptr = s_ptr->next;
			}
			if(!written){
				printf("Up to date\n");
			}
			close(update_fd);
		
		}
		
	
		free(serv_file_ver);
		free(serv_ver_num);
		free(cli_ver_num);
        
    }
    else if (strcmp(command, "upgrade") == 0)
    {
        proto_message = protocol(command, arg1, "-1");
    }
    else if (strcmp(command, "commit") == 0)
    {
        proto_message = protocol(command, arg1, "-1");
    }
    else if (strcmp(command, "push") == 0)
    {
        proto_message = protocol(command, arg1, "-1");
    }
    else if (strcmp(command, "create") == 0)
    {


        proto_message = protocol(command, arg1, "-1");

        //Send protocol message to server
        if ((send(sock_fd, proto_message, strlen(proto_message), 0)) == -1)
        {
            printf("Error: Failed to send message to server.\n");
            pthread_exit(NULL);
            exit(1);
        }
        else
        {
            printf("Client protocol message has been sent.\n");
        }

        //Receive .Manifest file
        int messageLength = 300;
        char message[messageLength + 1];
        int in = 0;
        int index = 0;
        int limit = messageLength;
        //in = recv(clisock, &message[index], messageLength, 0);
        //printf("in: %d\n", in);

        /*repo created?*/
        in = recv(sock_fd, &message[index], messageLength, 0);
        if (!(in > 0))
        {
            printf("Error: failed to receive message from server.\n");
        }
        else
        {
            message[strlen(message)] = '\0';

            char *temp = (char *)(malloc(strlen(arg1) + 8));
            strcpy(temp, "client/");
            strcat(temp, arg1);
            printf("Creating project....%s\n", arg1);
            if (mkdir(temp, 0700) == -1)
            {
                printf("Failed to create directory\n");
                return 0;
            }
        }

        receive_file(sock_fd, arg1, 1);
    }
    else if (strcmp(command, "destroy") == 0)
    {
        proto_message = protocol(command, arg1, "-1");

        //Send protocol message to server
        if ((send(sock_fd, proto_message, strlen(proto_message), 0)) == -1)
        {
            printf("Error: Failed to send message to server.\n");
            pthread_exit(NULL);
            exit(1);
        }
        else
        {
            printf("Client protocol message has been sent.\n");
            char message[255];
            recv(sock_fd, &message, sizeof(message), 0);
        }
    }
    else if (strcmp(command, "currentversion") == 0)
    {
        proto_message = protocol(command, arg1, "-1");
        puts(proto_message);

        //Send protocol message to server
        if ((send(sock_fd, proto_message, strlen(proto_message), 0)) == -1)
        {
            printf("Error: Failed to send message to server.\n");
            pthread_exit(NULL);
            exit(1);
        }
        else
        {
            printf("Client protocol message has been sent.\n");
        }

        int messageLength = 300;
        char message[messageLength + 1];
        int in = 0;
        int index = 0;
        int limit = messageLength;

        receive_file(sock_fd, arg1, 0);

        char *man_file = (char *)malloc(sizeof(char) * strlen("./.Manifest"));
        strcpy(man_file, "./.Manifest");

        int fd = open(man_file, O_RDONLY);
        int end = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, 0);

        int status = 0;
        int bytes = 0;
        char buff;
        char *temp;
        temp = "";
        int counter = 1;
        char *ver_num;
        ver_num = (char *)malloc(sizeof(char) * 2);
        int removed = 1; //1 == removed, 0 == not removed

        do
        {
            status = read(fd, &buff, 1);

            //file did not exist in the .Manifest file
            if (bytes == end)
            {
                break;
            }
            bytes++;

            if (buff == '\n' || buff == ' ')
            {

                //Gets current version of project
                if (bytes == 2)
                {
                    printf("Current project version: %s\n", temp);
                    temp = "";
                    continue;
                }

                if (counter == 1)
                {
                    strcpy(ver_num, temp);
                    counter++;
                }
                else if (counter == 2)
                {
                    if (strcmp(temp, "!") == 0)
                    {
                        removed = 1;
                    }
                    else
                    {
                        removed = 0;
                    }
                    counter++;
                }
                else if (counter == 3)
                {
                    if (removed == 0)
                    {
                        printf("%s %s\n", ver_num, temp);
                    }
                    counter++;
                }
                else if (counter == 4)
                {
                    counter = 1; //reset
                }

                temp = ""; //reset
                continue;
            }

            temp = concat(temp, buff);
        } while (status > 0);

        status = remove(man_file);
        if (status == 0)
        {
            puts("file removed");
        }

        free(man_file);
        free(ver_num);
        free(proto_message);
    }
    else if (strcmp(command, "history") == 0)
    {
        proto_message = protocol(command, arg1, "-1");
    }
    else if (strcmp(command, "rollback") == 0)
    {
        printf("protocol: %s\n", protocol(command, arg1, arg2));
    }

    close(sock_fd);
    pthread_exit(NULL);
    exit(0);
}

int main(int argc, char **argv)
{
    int i = 0;
    pthread_t tid;
    if (pthread_create(&tid, NULL, clientThread(NULL, argc, argv), NULL) != 0)
    {
        printf("failed to create thread\n");
    }
    sleep(20);
    pthread_join(tid, NULL);
}
