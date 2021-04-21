/* Tema 2 SO - Mihalcea Adrian 334CA */

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "so_stdio.h"

#define BUFSIZ 4096

/*
 * Structure that holds information about an open file, counterpart to
 * the FILE structure from stdio.h
 */
struct _so_file {
	int fd;
	char BUF[BUFSIZ];
	int buf_end;
	int buf_cursor;
	int last_op_write;
	int error;
	int eof_found;
	int pid;
};

/*
 * Function to return the file descriptor of the open file
 */
int so_fileno(SO_FILE *stream)
{
	return stream->fd;
}

/*
 * Structure to open the specified file in the specified mode
 */
SO_FILE *so_fopen(const char *pathname, const char *mode)
{
	int fd;

	if (strcmp(mode, "r") == 0)
		fd = open(pathname, O_RDONLY);
	else if (strcmp(mode, "r+") == 0)
		fd = open(pathname, O_RDWR);
	else if (strcmp(mode, "w") == 0)
		fd = open(pathname, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (strcmp(mode, "w+") == 0)
		fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else if (strcmp(mode, "a") == 0)
		fd = open(pathname, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (strcmp(mode, "a+") == 0)
		fd = open(pathname, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		return 0;

	if (fd < 0)
		return 0;

	SO_FILE *ret = calloc(1, sizeof(SO_FILE));
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
	ok = close(stream->fd);
	free(stream);
	if (ok < 0)
		return ok;
	return 0;
}

/*
 * Function to flush contents of write buffer to file
 */
int so_fflush(SO_FILE *stream)
{
	int toWrite, written, wr;

	toWrite = stream->buf_end; written = 0;
	while (written < toWrite) {
		wr = write(stream->fd, stream->BUF + written,
		toWrite - written);
		if (wr < 0)
			return SO_EOF;
		written += wr;
	}

	stream->buf_cursor = 0;
	stream->buf_end = 0;
	return 0;
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
		stream->buf_cursor = stream->buf_end = 0;
	}
	ok = lseek(stream->fd, offset, whence);
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
		return lseek(stream->fd, 0, SEEK_CUR) + stream->buf_end;
	else
		return lseek(stream->fd, 0, SEEK_CUR) - buffered;
}

/*
 * Function to read a chunk of data of nmemb members of size bytes from file
 */
size_t so_fread(void *ptr, size_t size, size_t nmemb, SO_FILE *stream)
{
	int tmp;
	size_t i, j;
	char *to_read;

	for (i = 0; i < nmemb; ++i) {
		to_read = ptr + i * size;
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
	int tmp;
	size_t i, j;
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
	int rd;

	if (!stream->last_op_write && stream->buf_cursor < stream->buf_end)
		return stream->BUF[stream->buf_cursor++];
	stream->last_op_write = 0;
	rd = read(stream->fd, stream->BUF, BUFSIZ);
	if (rd <= 0) {
		if (rd < 0)
			stream->error = 1;
		else
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
	int ok;

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
	stream->last_op_write = stream->buf_cursor = stream->buf_end = 1;
	stream->BUF[0] = c;

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

/*
 * Opens the command specified as a stream of input/output
 */
SO_FILE *so_popen(const char *command, const char *type)
{
	pid_t pid;
	int fd[2], rdwr;
	SO_FILE *ret;

	ret = calloc(1, sizeof(SO_FILE));
	if (ret == 0)
		return 0;
	pipe(fd);
	rdwr = strcmp(type, "r") != 0;

	pid = fork();
	switch (pid) {
	case -1:  /* Fork failed */
		free(ret);
		return 0;
	case 0:  /* Child process */
		close(fd[rdwr]);
		dup2(fd[!rdwr], !rdwr);
		close(fd[!rdwr]);
		fd[!rdwr] = STDOUT_FILENO;
		execlp("/bin/sh", "sh", "-c", command, (char *)0);
		free(ret);
		return 0;
	default:
		break;
	}

	// Parent process
	close(fd[!rdwr]);
	ret->fd = fd[rdwr];
	ret->pid = pid;

	return ret;
}

/*
 * Function that closes the stream associated to a command
 */
int so_pclose(SO_FILE *stream)
{
	int pid, ok, status;

	pid = stream->pid;
	ok = so_fclose(stream);
	if (ok < 0)
		return ok;

	ok = waitpid(pid, &status, 0);
	if (ok < 0)
		return -1;

	return status;
}
