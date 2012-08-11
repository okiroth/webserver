/*
 * FileParser.c
 *
 *  Created on: 08/08/2012
 *      Author: ivan
 */

#include "FileParser.h"

#define KEY_DELIMITER '='
#define END_OF_LINE '\n'

int setKey(entry_t *entry, char *line);
int setValue(entry_t *value, char *line, int keylen);

/**
 * Reads a FILE	line by line and saves it into a map.
 * In case the line has 'key=value' format,
 * splits the line into key:value into the map.
 * Otherwise 'key=\0' and the line goes to 'value'.
 */
int readTextFile(char *file, map_t *buffer) {
	FILE *fp;
	char line[VALUE_MAX_SIZE];

	fp = fopen(file, "rt");

	int lines = 0;
	while (feof(fp) == false) {
		fgets(line, VALUE_MAX_SIZE, fp);

		entry_t *entry = buffer->entries + lines;
		setValue(entry, line, setKey(entry, line));

		lines++;
	}

	buffer->size = lines;

	fclose(fp);

	return lines;
}

/**
 * Reads a FILE as binary data
 */
bytelist_t* readBinaryFile(char *name) {
	FILE *file;
	fsize_t fsize;

	//Open file
	file = fopen(name, "rb");
	if (!file) {
		fprintf(stderr, "Unable to open file %s", name);
	}

	//Get file length
	fseek(file, 0, SEEK_END);
	fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Read file contents into buffer
	bytelist_t *buffer = newBytelist(fsize);
	fread(buffer->list, fsize, 1, file);

	fclose(file);

	return buffer;
}

/**
 * Splits the line by the '=' delimiter
 * and saves the first part into the 'entry->key'
 */
int setKey(entry_t *entry, char *line) {
	int i = 0;
	char c;

	while ((c = *(line + i)) != KEY_DELIMITER) {
		entry->key[i] = c;
		i++;

		//In case there is no key
		if (c == END_OF_LINE) {
			entry->key[0] = '\0';
			return 0;
		}
	}
	entry->key[i] = '\0';

	return i + 1;
}

/**
 * Splits the line by the '=' delimiter
 * and saves the second part into the 'entry->value'
 */
int setValue(entry_t *entry, char *line, int keylen) {
	int i = 0;
	char c;

	while ((c = *(line + keylen + i)) != END_OF_LINE) {
		entry->value[i] = c;
		i++;
	}
	entry->value[i] = '\0';

	return i;
}