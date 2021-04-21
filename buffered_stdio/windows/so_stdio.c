#include <windows.h>

#include "so_stdio.h"

#define BUFSIZ 4096

/*
 * Structure that holds information about an open file, counterpart to
 * the FILE structure from stdio.h
 */
struct _so_file {
	HANDLE fd;
	char BUF[BUFSIZ];
	int buf_end;
	int buf_cursor;
	int last_op_write;
	int error;
	int eof_found;
};

/*
 * Function to return the file descriptor of the open file
 */
SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	HANDLE fd;
	SO_FILE *ret;

	if (strcmp(mode, "r") == 0)
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	else if (strcmp(mode, "r+") == 0)
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	else if (strcmp(mode, "w") == 0)
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);
	else if (strcmp(mode, "w+") == 0)
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);
	else if (strcmp(mode, "a") == 0) {
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);
		SetFilePointer(fd, 0, 0, FILE_END);
	} else if (strcmp(mode, "a+") == 0) {
		fd = CreateFile(pathname, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 0);
		SetFilePointer(fd, 0, 0, FILE_END);
	} else
		return 0;

	if (fd == INVALID_HANDLE_VALUE)
		return 0;
	ret = calloc(1, sizeof(SO_FILE));
	if (ret == 0)
		return 0;
	ret->fd = fd;

	return ret;
}

/*
 * Function to close the file and free memory
 */
int so_fclose(SO_FILE *stream)
{
	int ok;

	if (stream->last_op_write == 1) {
		ok = so_fflush(stream);
		if (ok < 0) {
			free(stream);
			return ok;
		}
	}
	ok = CloseHandle(stream->fd);
	free(stream);
	if (!ok)
		return SO_EOF;
	return 0;
}

/*
 * Function to flush contents of write buffer to file
 */
int so_fflush(SO_FILE *stream)
{
	int toWrite = stream->buf_end, written, wr, ok;

	written = 0;
	while (written < toWrite) {
		ok = WriteFile(stream->fd, stream->BUF + written,
		toWrite - written, &wr, 0);
		if (!ok)
			return SO_EOF;
		written += wr;
	}

	stream->buf_cursor = stream->buf_end = 0;
	return 0;
}

/*
 * Function to return the file handler of the open file
 */
HANDLE so_fileno(SO_FILE *stream)
{
	return stream->fd;
}

/*
 * Function to move the current file pointer, clearing the current buffer
 */
int so_fseek(SO_FILE *stream, long offset, int whence)
{
	int ok;

	if (stream->last_op_write) {
		ok = so_fflush(stream);
		if (ok < 0) {
			stream->error = 1;
			return SO_EOF;
		}
	} else {
		stream->buf_cursor = 0;
		stream->buf_end = 0;
	}

	ok = SetFilePointer(stream->fd, offset, 0, whence);
	if (ok < 0) {
		stream->error = 1;
		return SO_EOF;
	}

	return 0;
}

/*
 * Function to print current position of the file pointer,
 * taking into account buffered but not processed data
 */
long so_ftell(SO_FILE *stream)
{
	int buffered;

	buffered = stream->buf_end - stream->buf_cursor;
	if (stream->last_op_write)
		return SetFilePointer(stream->fd, 0, 0, FILE_CURRENT)
		+ stream->buf_end;
	else
		return SetFilePointer(stream->fd, 0, 0, FILE_CURRENT)
		- buffered;
}

/*
 * Function to read a chunk of data of nmemb members of size bytes from file
 */
size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int tmp, i, j;
	char *to_read;

	for (i = 0; i < nmemb; ++i) {
		to_read = (char *)ptr + i * size;
		for (j = 0; j < size; ++j) {
			tmp = so_fgetc(stream);
			if (tmp == SO_EOF && stream->error)
				return 0;
			if (tmp == SO_EOF && stream->eof_found)
				return i;
			to_read[j] = tmp;
		}
	}

	return nmemb;
}

/*
 * Function to write a chunk of data of nmemb members of size bytes to file
 */
size_t so_fwrite(const void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int tmp, i, j;
	char *to_write;

	for (i = 0; i < nmemb; ++i) {
		to_write = (char *)ptr + i * size;
		for (j = 0; j < size; ++j) {
			tmp = so_fputc(to_write[j], stream);
			if (tmp != to_write[j])
				return 0;
		}
	}

	return nmemb;
}

/*
 * Function to read a character from file
 */
int so_fgetc(SO_FILE *stream)
{
	int rd, ok;

	if (stream->last_op_write == 0 && stream->buf_cursor < stream->buf_end)
		return stream->BUF[stream->buf_cursor++];
	stream->last_op_write = 0;
	ok = ReadFile(stream->fd, stream->BUF, BUFSIZ, &rd, 0);
	if (ok == 0) {
		stream->error = 1;
		return SO_EOF;
	}

	if (rd == 0) {
		stream->eof_found = 1;
		return SO_EOF;
	}
	stream->buf_end = rd;
	stream->buf_cursor = 0;
	return stream->BUF[stream->buf_cursor++];
}

/*
 * Function to write a character to file
 */
int so_fputc(int c, SO_FILE *stream)
{
	int wr = 0, ok;

	if (stream->buf_end == BUFSIZ) {
		ok = so_fflush(stream);
		if (ok < 0) {
			stream->error = 1;
			return ok;
		}
	}

	if (stream->last_op_write == 1) {
		stream->BUF[stream->buf_cursor++] = c;
		++stream->buf_end;
		return c;
	}
	stream->last_op_write = 1;
	stream->BUF[0] = c;
	stream->buf_cursor = 1;
	stream->buf_end = 1;

	return c;
}

/*
 * Returns 0/1 if eof has been found in the file
 */
int so_feof(SO_FILE *stream)
{
	return stream->eof_found;
}

/*
 * Returns 0/1 if an I/O error has happened so far
 */
int so_ferror(SO_FILE *stream)
{
	return stream->error;
}

SO_FILE *so_popen(const char *command, const char *type)
{
	return 0;
}

int so_pclose(SO_FILE *stream)
{
	return 0;
}
