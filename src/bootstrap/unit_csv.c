#include "../smkl/io.h"
#include "../smkl/mem.h"
#include "../smkl/type.h"
#include "../utils/io.h"
#include <stdio.h>
#include <string.h>

#define mkdie_and_close(msg) { mkdie (msg); goto exit; }

#define N_LINES 74
#define LENGTH_LINE 80

/* QUESTION: is sizeof (cur_line) = 256 or 14? */
/* ANSWER: 256 */
/* Q: Are those bytes 0s or undefined? */

typedef struct pascal_string
{
    char head[LENGTH_LINE];
} String;

struct CsvEntry
{
    uint16_t position;
    uint64_t id;
    uint16_t enabled;
    char comment[LENGTH_LINE];
};

enum
{
    COL_ID,
    COL_EN,
    COL_COMMENT,
    N_COL
};

/* implementation 1: sentinel per byte */
int
csv_parse_file (char (*lines_p_s)[LENGTH_LINE], const char * filename) /* an array of pointers */
{
    int n_lines = 0;
    FILE *fstream;
    int idx_linelist = 0;
    
    fstream = fopen (filename, "rb");
    if (!fstream) mkdie_and_close ("Couldn't open csv file! Is the file reachable?");

    char cur_line[LENGTH_LINE];
    int len_line = 0;
    char *p_curline = (&cur_line)[0];
    for (;;)
    {
        char cur_char;
        int read = fread (&cur_char, 1, 1, fstream);
        len_line++;
        if (cur_char == '\n' || cur_char == EOF || !read)
        {
            *p_curline = '\0';
            p_curline = &cur_line[0];
            /* work */
            memcpy ((lines_p_s)[idx_linelist], cur_line, LENGTH_LINE);
            printf ("Length is: %d, Line is: %s\n", len_line, (lines_p_s)[idx_linelist]);
            idx_linelist++;
            /* endwork */
            n_lines++;
            len_line = 0;
            if (cur_char == '\n') continue;
            else break;
        }
        *p_curline = cur_char;
        p_curline++;
    }

exit:
    if (fstream) fclose (fstream);
    return n_lines;
}

/* test */
int
csv_parse_alt (const char * filename)
{
    int n_lines = 0;
    int idx_linelist = 0;
    char buffer [N_LINES * LENGTH_LINE];
    char *p_buffer = &buffer[0];

    int result = load_text_file(buffer, filename);
    if (result != 0) mkdie ("Oh no!");
    char cur_line[LENGTH_LINE];
    char *p_curline = (&cur_line)[0];
    int len_line = 0;
    for (char cur_char;;)
    {
        cur_char = *p_buffer++;
        len_line++;
        if (cur_char == '\n' || cur_char == EOF)
        {
            *p_curline = '\0';
            p_curline = &cur_line[0];
            printf ("Length is: %d, Line is: %s\n", len_line, cur_line);
            idx_linelist++;
            n_lines++;
            len_line = 0;
            if (cur_char == '\n') continue;
            else break;
        }
        *p_curline++ = cur_char;
    }

    return n_lines;
}

#define csv_for_each_line(line, buffer) for(;;)
#define csv_for_each_column(col, col_idx, row) for(;;)

int main ()
{
    //char (*buffer)[LENGTH_LINE] = (char (*)[LENGTH_LINE]) mk_malloc (LENGTH_LINE*N_LINES);
    char buffer[N_LINES][LENGTH_LINE];
    // char *buffer_per = (char *) mk_malloc (LENGTH_LINE);
    int number_of_lines = csv_parse_file ((&buffer)[0], "/home/kinten/Garage/mates-vpk/profiles/kinten/addonlist.txt");
    printf("Lines: %d\n", number_of_lines);
    // printf("Here is an example. %s. The fifth character is '%c'.\n", buffer[1], buffer[1][4]);
    printf("Here is an example. %s. The fifth character is '%c'.\n", ((buffer-2)+2)[2], (*(buffer+1))[4] );
    /* p++ # p+1 */
    struct CsvEntry[N_LINES];
    int entry_idx = 0;
    char *line;
    csv_for_each_line (line, buffer)
    {
        char *column_buffer;
        int column_idx;
        csv_for_each_column (column, column_idx, line)
        {
            switch (column_idx)
            {
                case COL_ID:
                    CsvEntry[entry_idx].id = mk_atoi (column_buffer);
                    break;
                case COL_EN:
                    CsvEntry[entry_idx].enabled = mk_atoi (column_buffer);
                    break;
                case COL_COMMENT:
                    CsvEntry[entry_idx].comment = strstrip (column_buffer, '"');
                    break;
                default:
                    mkdie ("Malformed CSV!");
                    break;
            }
            
        }
    }
    return 0;
}