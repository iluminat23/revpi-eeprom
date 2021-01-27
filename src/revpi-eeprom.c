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
	struct stat st;
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

	return 0;
}