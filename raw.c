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
	int bytes, count, available;
	unsigned char buffer[BUFFERSIZE];
	unsigned char *pointer = buffer;
	if (argc == 1) {  /* get data from stdin */
		if (ioctl(STDIN_FILENO, FIONREAD, &bytes) != 0) {
			perror("failed getting byte count");
			return 1;
		} else if (bytes == 0) {
			fprintf(stderr, "no bytes found in stdin\n");
			return 1;
		} else {
			fprintf(stderr, "reading stdin into buffer\n");
		}
		if ((count = fread(
			buffer,
			1,
			MIN(BUFFERSIZE, bytes),
			stdin
		)) != bytes)
			fprintf(stderr, "read %d out of %d bytes\n",
				count, bytes);
		else fprintf(stderr, "got %d bytes from stdin\n", count);
	} else {
		/* copy args into buffer with nulls between */
		bytes = 0;
		for (int i = 1; i < argc; i++) {
			count = strlen(argv[i]);
			bytes += count;
			available = BUFFERSIZE - (pointer - buffer);
			fprintf(stderr,
				"count: %d, buffer bytes available: %d\n",
				count, available);
			pointer = stpncpy(
				pointer,
				argv[i],
				MIN(available, count + 1)
			) + 1;
		}
		bytes += argc - 2;  /* number of nulls between args */
	}
	dumpraw(bytes, buffer);
}
