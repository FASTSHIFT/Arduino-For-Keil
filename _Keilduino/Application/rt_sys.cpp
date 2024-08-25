/*
 * MIT License
 * Copyright (c) 2023 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <rt_sys.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "Arduino.h"

#pragma import(__use_no_semihosting)

enum
{
    STDIN,
    STDOUT,
    STDERR,
    STDMAX
};
#define IS_STD_FILEHANDLE(fh) ((fh) >= 0 && (fh) < STDMAX)

/*
 * These names are used during library initialization as the
 * file names opened for stdin, stdout, and stderr.
 * As we define _sys_open() to always return the same file handle,
 * these can be left as their default values.
 */
const char __stdin_name[] =  "__stdin";
const char __stdout_name[] =  "__stdout";
const char __stderr_name[] =  "__stderr";
/*
 * Open a file. May return -1 if the file failed to open.
 */
FILEHANDLE _sys_open(const char *name, int openmode)
{
    if (name == __stdin_name)
    {
        return STDIN;
    }
    else if (name == __stdout_name)
    {
        return STDOUT;
    }
    else if (name == __stderr_name)
    {
        return STDERR;
    }

    return (FILEHANDLE) - 1;
}

/*
 * Close a file. Should return 0 on success or a negative value on error.
 */
int _sys_close(FILEHANDLE fh)
{
    if(IS_STD_FILEHANDLE(fh))
    {
        return 0;
    }

    return  -1;
}

/*
 * Write to a file. Returns 0 on success, negative on error, and
 * the number of characters _not_ written on partial success.
 * `mode' exists for historical reasons and must be ignored.
 */
int _sys_write(FILEHANDLE fh, const unsigned char *buf,
               unsigned len, int mode)
{
    if(IS_STD_FILEHANDLE(fh))
    {
        if(fh == STDOUT || fh == STDERR)
        {
            Serial.write(buf, len);
        }
        return len;
    }

    return -1;
}

/*
 * Read from a file. Can return:
 *  - zero if the read was completely successful
 *  - the number of bytes _not_ read, if the read was partially successful
 *  - the number of bytes not read, plus the top bit set (0x80000000), if
 *    the read was partially successful due to end of file
 *  - -1 if some error other than EOF occurred
 *
 * It is also legal to signal EOF by returning no data but
 * signalling no error (i.e. the top-bit-set mechanism need never
 * be used).
 *
 * So if (for example) the user is trying to read 8 bytes at a time
 * from a file in which only 5 remain, this routine can do three
 * equally valid things:
 *
 *  - it can return 0x80000003 (3 bytes not read due to EOF)
 *  - OR it can return 3 (3 bytes not read), and then return
 *    0x80000008 (8 bytes not read due to EOF) on the next attempt
 *  - OR it can return 3 (3 bytes not read), and then return
 *    8 (8 bytes not read, meaning 0 read, meaning EOF) on the next
 *    attempt
 *
 * `mode' exists for historical reasons and must be ignored.
 */
int _sys_read(FILEHANDLE fh, unsigned char *buf,
              unsigned len, int mode)
{
    if(IS_STD_FILEHANDLE(fh))
    {
        return 0;
    }

    return -1;
}

/*
 * Write a character to the output channel. This function is used
 * for last-resort error message output.
 */
void _ttywrch(int ch)
{
}

/*
 * Return non-zero if the argument file is connected to a terminal.
 */
int _sys_istty(FILEHANDLE fh)
{
    return IS_STD_FILEHANDLE(fh);
}

/*
 * Move the file position to a given offset from the file start.
 * Returns >=0 on success, <0 on failure.
 */
int _sys_seek(FILEHANDLE fh, long pos)
{
    return -1;
}

/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 */
int _sys_ensure(FILEHANDLE fh)
{
    return -1;
}

/*
 * Return the current length of a file, or <0 if an error occurred.
 * _sys_flen is allowed to reposition the file pointer (so Unix can
 * implement it with a single lseek, for example), since it is only
 * called when processing SEEK_END relative fseeks, and therefore a
 * call to _sys_flen is always followed by a call to _sys_seek.
 */
long _sys_flen(FILEHANDLE fh)
{
    return -1;
}

/*
 * Return the name for temporary file number sig in the buffer
 * name. Returns 0 on failure. maxlen is the maximum name length
 * allowed.
 */
int _sys_tmpnam(char *name, int sig, unsigned maxlen)
{
    return 0;
}

/*
 * Terminate the program, passing a return code back to the user.
 * This function may not return.
 */
void _sys_exit(int status)
{
    /*main return*/
    while(1);
}

/*
 * Return a pointer to the command line used to invoke the program.
 * The supplied buffer may be used to store the string, but need
 * not be.
 */
char *_sys_command_string(char *cmd, int len)
{
    return cmd;
}

extern "C" {
    time_t time(time_t *time)
    {
        return millis();
    }

    void exit(int status)
    {
        while(1);
    }

    int system(const char *string)
    {
        return 0;
    }

    int remove(const char *filename)
    {
        return 0;
    }

    clock_t clock()
    {
        return millis();
    }
}
