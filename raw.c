/* implement raw output */
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "raw.h"

int BUFFERSIZE = 4096;

void dumpraw(int buffersize, unsigned char *buffer, FILE *out, bool newline) {
	unsigned char byte;
	for (int i = 0; i < buffersize; i++) {
		byte = buffer[i];
		if (byte < 127 && byte >= 32) fprintf(out, "%c", byte);
		else fprintf(out, "\\x%02x", byte);
	}
	if (newline) fprintf(out, "\n");
}

int convertraw(int buffersize, unsigned char *inbuffer, char *outbuffer) {
	/* outbuffer must be at least 4 times the size of inbuffer, because
         * any nonprintable byte 0xab will be shown as "\xab". */
	unsigned char byte;
	char stringbuffer[5], *pointer = outbuffer;
	for (int i = 0; i < buffersize; i++) {
		byte = inbuffer[i];
		if (byte < 127 && byte >= 32) {
			pointer = mempcpy(pointer, &inbuffer[i], 1);
		} else {
			sprintf(stringbuffer, "\\x%02x", byte);
			pointer = mempcpy(pointer, stringbuffer, 4);
		}
	}
	return pointer - outbuffer;
}

#ifdef TESTRAW
int main(int argc, char **argv) {
	int bytes, count, available;
	unsigned char buffer[BUFFERSIZE];
	unsigned char *pointer = buffer;
	char *translated;
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
	//dumpraw(bytes, buffer, stdout, true);
	translated = malloc(BUFFERSIZE * 4);
	count = convertraw(bytes, buffer, translated);
	fwrite(translated, 1, count, stdout);
	free(translated);
	putc('\n', stdout);
}
#endif
