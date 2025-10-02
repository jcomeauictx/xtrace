/* implement raw output */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <unistd.h>
#include <string.h>

int BUFFERSIZE = 128;

int dumpraw(int buffersize, unsigned char *buffer) {
	unsigned char byte;
	for (int i = 0; i < buffersize; i++) {
		byte = buffer[i];
		if (byte < 127 && byte >= 32) printf("%c", byte);
		else printf("\\x%02x", byte);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	int bytes, count;
	unsigned char buffer[BUFFERSIZE];
	unsigned char *pointer = buffer;
	fprintf(stderr, "args: %d ", argc);
	if (argc == 1) {  /* get data from stdin */
		if (ioctl(STDIN_FILENO, FIONREAD, &bytes) != 0) {
			perror("failed getting byte count");
			return 1;
		}
		if (count = read(
			STDIN_FILENO,
			buffer,
			MIN(BUFFERSIZE, bytes)
		) != bytes)
			fprintf(stderr, "read %d out of %d bytes\n",
				count, bytes);
	} else {
		/* copy args into buffer with nulls between */
		bytes = 0;
		for (int i = 1; i < argc; i++) {
			count = strlen(argv[i]);
			bytes += count;
			pointer = strncpy(
				pointer,
				argv[i],
				MIN(pointer - buffer, count + 1)
			);
		}
		bytes += argc - 2;  /* number of nulls between args */
	}
	dumpraw(bytes, buffer);
}
