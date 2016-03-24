
/***************

	Just for C raw socket programming practice

**************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <byteswap.h>

#define BACKLOG 10
#define SIZE_CHAR 16
#define SIZE_INT 4
#define AMOUNT_NUM 4
#define ui unsigned int
#define SIZE_BUFFER 100

//   collect uint from 4 bytes right than arr
ui get_num(unsigned char* arr)
{
	ui res = 0;
	res |= arr[0];
	res |= (arr[1] << 8);
	res |= (arr[2] << 16);
	res |= (arr[3] << 24);
	return res;
}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}


int main()
{
	// char c_arr[AMOUNT_NUM];
	// scanf("%x%x%x%x", c_arr, c_arr+1, c_arr+2, c_arr+3);
	// printf("%x\n", get_num(c_arr));

	struct addrinfo hints, *res;
	int sockfd, len_bytes, i, total_read_bytes = 0;
	char arr[SIZE_BUFFER];
	char* p_arr = arr;
	ui sum = 0;

	memset(arr, 0, SIZE_BUFFER);
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo("vortex.labs.overthewire.org", "5842", &hints, &res);
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	connect(sockfd, res->ai_addr, res->ai_addrlen);
	
	while (total_read_bytes < 16)
	{
		len_bytes = recv(sockfd, p_arr, SIZE_BUFFER, 0);
		p_arr += len_bytes;
		total_read_bytes += len_bytes;	
	}	

	printf("total bytes:%i\n", total_read_bytes);
	for(i = 0; i < total_read_bytes; i++)
	{
		printf("%hhx %s\n", arr[i], byte_to_binary(arr[i]));
	}

	ui arr_ui[AMOUNT_NUM];
	for (i = 0; i < AMOUNT_NUM; ++i)
	{
		arr_ui[i] = get_num(arr +  4 * i);	
		sum += arr_ui[i];
	}

	puts("\n\n");


	for (i = 0; i < AMOUNT_NUM; ++i) 
	{
		printf("%x %s %s %s %s\n", arr_ui[i], byte_to_binary(arr_ui[i] >> 24),
		byte_to_binary((arr_ui[i] & 0xff0000) >> 16),
		byte_to_binary((arr_ui[i] & 0xff00) >> 8),
		byte_to_binary(arr_ui[i] & 0xff));
	}

	//sum = ntohl(sum);
	send(sockfd, &sum, sizeof(ui), 0);
	memset(arr, 0, SIZE_BUFFER);
	puts("receiving answer...\n");
	recv(sockfd, arr, SIZE_BUFFER, 0);
	puts(arr);


	close(sockfd);
	return 0;
}
