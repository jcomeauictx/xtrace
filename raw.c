/* implement raw output */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int BUFFERSIZE = 128;

int dumpraw(int buffersize, unsigned char *buffer, bool newline) {
	unsigned char byte;
	for (int i = 0; i < buffersize; i++) {
		byte = buffer[i];
		if (byte < 127 && byte >= 32) printf("%c", byte);
		else printf("\\x%02x", byte);
	}
	if (newline) printf("\n");
}

int main(int argc, char **argv) {
	int bytes, count, available;
	unsigned char buffer[BUFFERSIZE];
	unsigned char *pointer = buffer;
	if (argc == 1) {  /* get data from stdin */
		fprintf(stderr, "attempting to read raw data from stdin\n");
		count = fread(buffer, 1, BUFFERSIZE, stdin);
		fprintf(stderr, "read %d out of %d bytes\n",
			count, BUFFERSIZE);
		bytes = count;
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
	dumpraw(bytes, buffer, true);
}
