#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* we use 32k eeproms */
#define EEPROM_SIZE (32 * 1024)

#define EEPROM_PATH "/sys/bus/i2c/devices/0-0050/eeprom"


int main(int argc, char *argv[])
{
	int ret;
	int eeprom_fd;
	int image_fd;
	struct stat st;
	size_t image_size;
	/* FIXME: Use O_WRONLY, as we should not need to read the eeprom. */
	eeprom_fd = open(EEPROM_PATH, O_RDWR | O_SYNC);
	if (eeprom_fd < 0) {
		fprintf(stderr, "ERROR: Can't open eeprom `%s': %s\n", EEPROM_PATH,
		        strerror(errno));
		exit(EXIT_FAILURE);
	}

	ret = fstat(eeprom_fd, &st);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Can't get file status `%s': %s\n", EEPROM_PATH,
		        strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (st.st_size != EEPROM_SIZE) {
		fprintf(stderr, "ERROR: EEPROM size missmatch, expacted: %d; got: %d\n",
		        EEPROM_SIZE, st.st_size);
		exit(EXIT_FAILURE);
	}

	if (argc < 1) {
		fprintf(stderr, "ERROR: No image file specified.\n");
		exit(EXIT_FAILURE);
	}
	image_fd = open(argv[1], O_RDONLY);
	if (image_fd < 0) {
		fprintf(stderr, "ERROR: Can't open image file `%s': %s\n", argv[1],
		        strerror(errno));
		exit(EXIT_FAILURE);
	}
	ret = fstat(image_fd, &st);
	if (ret < 0) {
		fprintf(stderr, "ERROR: Can't get image file status `%s': %s\n",
		        argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}
	image_size = st.st_size;
	if (image_size > EEPROM_SIZE) {
		fprintf(stderr, "ERROR: Image size (%d) exceeds EEPROM size (%d).\n",
		        image_size, EEPROM_SIZE);
		exit(EXIT_FAILURE);
	}

	return 0;
}