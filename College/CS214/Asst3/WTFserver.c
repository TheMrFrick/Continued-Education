#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <zlib.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/sendfile.h>

struct stat st = {0};

char client_message[2000];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *socketThread(void *arg);
int decompress_one_file(char *infilename, char *outfilename);
int compress_one_file(char *infilename, char *outfilename);
int remove_directory(const char *path);
void send_file(char *proj, char *file, int socket);
void sendDirectory(char *dirName, int socket);

void sendDirectory(char *dirName, int socket)
{
    DIR *d = opendir(dirName);
    size_t path_len = strlen(dirName);
    int r = -1;

    if (d)
    {
        struct dirent *p;
        r = 0;
        while ((p = readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", dirName, p->d_name);
                printf("Looking at: %s\n", buf);
                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        printf("Directory: %s\n", buf);
                        send(socket, buf, len, 0);
                        sleep(1);
                        sendDirectory(buf, socket);
                    }
                    else
                    {
                        //project
                        send(socket, "file", sizeof("file"), 0);
                        sleep(1);
                        send(socket, buf, len, 0 );
                        send_file(dirName, buf, socket);
                    }
                }
                free(buf);
            }
        }
        closedir(d);
    }
}

int remove_directory(const char *path)
{
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p = readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_directory(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r)
    {
        r = rmdir(path);
    }
    return r;
}

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

    // printf("Read %ld bytes, wrote %ld bytes, Compression factor %4.2f%%\n", total_read, file_size(outfilename), (1.0 - file_size(outfilename) * 1.0 / total_read) * 100.0);
}

int checkForExistence(char *dirName)
{
    char *temp = (char *)(malloc(strlen(dirName) + 8));
    strcat(temp, "server/");
    strcat(temp, dirName);
    if (stat(temp, &st) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
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

/* Produces protocol message to send to server 
char* protocol(char* command, char* contents, int num_files){

	//Determines how many colons there will be in the message
	int colons = 2;
	if(atoi(ver) > -1){
		colons += 1;
	}
	
	//Convert project length to a string
	char* proj_len = (char*) malloc(12);
	sprintf(proj_len, "%d", strlen(proj_name));

	
	//Proto contains the protocol message
	char* proto = (char*) malloc(strlen(command) + strlen(proj_name) + colons + strlen(proj_len) + strlen(ver));
	
	proto = strcat(proto, command); //command
	proto = strcat(proto, ":");
	proto = strcat(proto, proj_len); //number of bytes
	proto = strcat(proto, ":");
	proto = strcat(proto, proj_name);
	
	if(colons == 3){
		proto = strcat(proto, ":");
		proto = strcat(proto, ver);
	}
	
	//printf("%s\n", proto);

	return proto;
}
*/

/* Method to hash a file */
char *hash_file(char *file)
{

    char *hash_message = 0;
    long length;
    FILE *fp = fopen(file, "rb");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        hash_message = malloc(length);
        if (hash_message)
        {
            fread(hash_message, 1, length, fp);
        }
        fclose(fp);
    }

    //printf("contents: %s\n", hash_message);

    //Hash the contents
    unsigned char *hash;
    hash = (char *)malloc(sizeof(char) * 256);
    SHA256(hash_message, strlen(hash_message), hash);

    //Printing out the hash code
    int i;
    for (i = 0; i < strlen(hash); i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");

    return hash;
}

//Create function
int create(char *proj_name, const char *delim, int clisock)
{

    int proj_len = strlen(proj_name);

    if (checkForExistence(proj_name))
    {
        printf("Error: Project already exists on server\n");
        return 0;
    }
    else
    {
        char *temp = (char *)(malloc(proj_len + 8));
        strcpy(temp, "server/");
        strcat(temp, proj_name);
        puts("Repo doesn't exist");
        printf("Creating repo....%s\n", proj_name);
        if (mkdir(temp, 0700) == -1)
        {
            printf("Failed to create directory\n");
            return 0;
        }
        else
        {
            printf("Successfully created directory\n");
        }

        int manifest;
        char *file_name = (char *)malloc(sizeof(char) * (strlen(temp) + 12));
        strcpy(file_name, temp);
        strcat(file_name, "/");
        strcat(file_name, "./.Manifest");
        manifest = open(file_name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        write(manifest, "0", strlen("0"));
        write(manifest, "\n", strlen("\n"));

        puts(file_name);

        //Send Manifest over
        //need to make protocol first

        free(temp);
        free(file_name);
        close(manifest);
        //write(clisock, "Repo created", strlen("Repo created"));
    }

    return 1;
}

void send_file(char *proj, char *file, int socket)
{

    char *file_path;
    file_path = (char *)malloc(sizeof(char) * (7 + strlen(file) + strlen(proj) + 1));
    strcpy(file_path, "server/");
    strcat(file_path, proj);
    strcat(file_path, "/");
    strcat(file_path, file);

    char *z_file = (char *)malloc(sizeof(char) * strlen(file) + 2);
    strcpy(z_file, file);
    strcat(z_file, ".z");

    //compress the file
    compress_one_file(file_path, z_file);
    free(file_path);

    char *compress_file = (char *)malloc(sizeof(char) * strlen(z_file) + 2);
    strcpy(compress_file, "./");
    strcat(compress_file, z_file);

    puts(compress_file);

    //open compressed file
    int fd = open(compress_file, O_RDONLY);
    if (fd == -1)
    {
        printf("Error: cannot open file\n");
    }

    puts(file);

    //send file name
    if ((send(socket, file, strlen(file), 0)) == -1)
    {
        printf("Error: Failed to send message to server.\n");
    }
    sleep(2); //waits to send next message

    int bytes = lseek(fd, 0, SEEK_END);
    printf("num bytes: %d\n", bytes);
    char file_size[256];
    sprintf(file_size, "%d", bytes);

    close(fd);

    fd = open(compress_file, O_RDONLY);
    if (fd == -1)
    {
        printf("Error: cannot open file\n");
    }

    //send file length
    if ((send(socket, file_size, strlen(file_size), 0)) == -1)
    {
        printf("Error: Failed to send message to server.\n");
    }
    sleep(1);

    // off_t* offset = 0;
    int sent_bytes = 0;
    int remain_data = bytes;

    while (remain_data > 0)
    {
        sent_bytes = sendfile(socket, fd, NULL, BUFSIZ);
        if (sent_bytes > 0)
        {
            remain_data -= sent_bytes;
        }
        else
        {
            printf("sent_bytes is not > 0\n");
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int sock_fd; //file descriptor for socket
    int new_sock_fd;
    int portno; //port number the server accepts connections
    int bd;
    int client_len; //size of the address of the client
    int x;          //returns the value for the read() and write() calls

    char buffer[256];               //buffer, server reads chars from socket
    struct sockaddr_in server_addr; //will contain addr of server
    struct sockaddr_in client_addr; //will contain addr of client

    if (argc < 2)
    {
        error("No port number. Usage: ./server <port number>\n");
    }
    /*
    Defintion of a struct sockaddr_in:

    struct sockaddr_in{
        short sin_family; //must be AF_INET
        u_short sin_port;
        struct in_addr sin_addr;
        char sin_zero[0];
    }
    */

    portno = atoi(argv[1]);

    sock_fd = socket(AF_INET, SOCK_STREAM, 0); //create a socket

    if (sock_fd < 0)
    { //if sock_fd == -1
        printf("Error: failed to open socket\n");
    }

    // portno = atoi(argv[1]); //grab portno from args
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno); //htons converts portno to network byte order

    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    bd = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (bd < 0)
    {
        printf("Error: failed to bind socket to address\n");
    }

    listen(sock_fd, 50); //listen to the socket for connections

    pthread_t tid[60];
    int i = 0;
    while (1)
    {
        int clilen = sizeof(client_addr);
        int clisock = accept(sock_fd, (struct sockaddr *)&client_addr, &clilen);

        if (clisock >= 0)
        {
            //this will send it to socketThread
            if (pthread_create(&tid[i], NULL, socketThread, &clisock) != 0)
            {
                printf("Failed to create thread\n");
            }

            if (i >= 50)
            {
                i = 0;
                while (i < 50)
                {
                    pthread_join(tid[i++], NULL);
                }
                i = 0;
            }
        }
    }
    return 0;
}

void *socketThread(void *arg)
{
    //memset(client_message, 0, sizeof(client_message));
    int newSocket = *((int *)arg);
    send(newSocket, "Connected to server", sizeof("Connected to server"), 0);
    //recieve what the client sent:
    if (recv(newSocket, &client_message, 2000, 0) < 0)
    {
        printf("Error: failed to receive message from client.\n");
    }
    printf("Client sent: %s\n", client_message);
    if (strcmp(client_message, "") == 0)
    {
        puts("message null...exiting");
        sleep(1);
        close(newSocket);
        pthread_exit(NULL);
    }

    const char delim[2] = ":";
    char *token;
    token = strtok(client_message, delim);
    puts(token);

    if (strcmp(token, "create") == 0)
    {
        token = strtok(NULL, delim); // go to next token
        token = strtok(NULL, delim); // skip bytes

        if (create(token, delim, newSocket))
        {
            pthread_mutex_lock(&lock);
            char *return_message = "Repo created successfully\n";
            if ((send(newSocket, return_message, strlen(return_message), 0)) == -1)
            {
                printf("Error: Failed to send message to server.\n");
            }

            puts(token);
            puts("hello???");
            //Create .Manifest
            char *name;
            name = (char *)malloc(sizeof(char) * (7 + 10 + strlen(token)));
            strcpy(name, "server/");
            strcat(name, token);
            strcat(name, "/.Manifest");
            int fd = open(name, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
            write(fd, "1\n", strlen("1\n"));
            close(fd);

            send_file(token, ".Manifest", newSocket);

            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_lock(&lock);
            if ((send(newSocket, "Repo was not created\n", strlen("Repo was not created\n"), 0)) == -1)
            {
                printf("Error: Failed to send message to client.\n");
            }
            pthread_mutex_unlock(&lock);
        }
    } else if(strcmp(token, "currentversion") == 0 || strcmp(token, "update") == 0){
    	
    	token = strtok(NULL, delim); // go to next token
    	char* file_path = malloc(sizeof(char) * atoi(token) + strlen("/version1") + 3);
    	token = strtok(NULL, delim);
    	strcpy(file_path, token);
    	
    	puts(file_path);
    	
    	if(checkForExistence(file_path) == 0){
    		send(newSocket, "Error: project does not exist on server.\n", strlen("Error: project does not exist on server.\n"), 0);
    		exit(0);
    	}
    	
    	strcat(file_path, "/version");
    	puts(file_path);
    	
    	int verno = 1;
    	char* ver;
    	ver = (char*) malloc(sizeof(char) * 4);
    	
    	char* result = strstr(file_path, "/version");
    	int index = result - file_path;
    	printf("position: %d\n", index);
    	
    	//int index = strlen(file_path) - position;
    	
    	//printf("index: %d\n", index);
    	int i;
    	while(1){
    		sprintf(ver, "%d", verno);
    		puts(ver);
    		
    		for(i = 0; i < strlen(ver); i++){
    			file_path[index+8+i] = ver[i];
    		}
    		
    		puts(file_path);
    		
    		if(checkForExistence(file_path)){
    			verno++;	
    		} else {
    			break;	
    		}
    	}

    	//subtract a version number
    	int temp = strlen(ver);
    	int zero_out = 0;
		verno--;
		sprintf(ver, "%d", verno); 	
		
		if(temp > strlen(ver)){ //went from a 2 digit number to 1 digit
			zero_out = 1;
		}
		
    	for(i = 0; i < strlen(ver); i++){
			file_path[index+8+i] = ver[i];
			
			if(zero_out){
				file_path[index+8+i+1] = '\0';
			}
		}
		
    	puts(file_path);
    	send_file(file_path, ".Manifest", newSocket);
    	
    	free(file_path);
    	
    } 
    
    if (strcmp(token, "destroy") == 0)
    {
        token = strtok(NULL, delim); // go to next token -> number
        token = strtok(NULL, delim); // name of the directory/project

        if (checkForExistence(token))
        {
            puts("Directory exists");
            pthread_mutex_lock(&lock);
            char *temp = (char *)(malloc(sizeof(token) + 7));
            strcpy(temp, "server/");
            strcat(temp, token);
            remove_directory(temp);
            
            free(temp);
            send(newSocket, "Project has been successfully removed", strlen("Project has been successfully removed"), 0);
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_lock(&lock);

            send(newSocket, "No project by that name found\n", strlen("No project by that name found\n"), 0);
            pthread_mutex_unlock(&lock);
        }
    }
    if (strcmp(token, "checkout") == 0)
    {
        token = strtok(NULL, delim); //boo
        token = strtok(NULL, delim); //get the project name

        if (checkForExistence(token))
        {
            puts("Project Exists");
            char *temp = (char *)malloc(sizeof(token) + 8);
            strcpy(temp, "server/");
            strcat(temp, token);
            printf("looking at directory: %s\n", temp);
            //send the project and everything in it to the client:
            sendDirectory(temp, newSocket);
            send(newSocket, "-1", 3, 0);
            free(temp);
        }
        else
        {
            pthread_mutex_lock(&lock);
            send(newSocket, "40", 40, 0);
            sleep(.5);
            send(newSocket, "No project by that name found on server", strlen("No project by that name found on server"), 0);
            pthread_mutex_unlock(&lock);
        }
    }

    // pthread_mutex_unlock(&lock);
    sleep(1);
    // send(newSocket, buffer, sizeof(buffer), 0);
    close(newSocket);
    memset(client_message, 0, sizeof(client_message));
    pthread_exit(NULL);
}
