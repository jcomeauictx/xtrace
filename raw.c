/* implement raw output */
#include <stdio.h>
int dumpraw(int buffersize, unsigned char *buffer) {
	unsigned char byte;
	for (int i = 0; i < buffersize; i++) {
		byte = buffer[i];
		if (byte < 127 && byte >= 32) printf("%c", byte);
		else printf("\\x02x", byte);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	fprintf(stderr, "args: %d ", argc);
	for (int i = 0; i < argc; i++) {
		fprintf(stderr, "%s ", argv[i]);
	}
	fprintf(stderr, "\n");
}
