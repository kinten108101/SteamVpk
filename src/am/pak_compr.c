#define MAX_FILES 2048
#define SIZE_VPKENTRY 18

static inline uint8
_strcmp (const char* a, const char* b)
{
    while (*a++ == *b++) if (*a=='\0') return 1;
    return 0;
}

static inline uint8
_read_string (FILE* f, char* str ) 
{
    char* cur = (char*)malloc(1);
    uint8 len = 0;
    do
    {
        fread(cur,1,1,f);
        *(str++)=*cur; len++;
    }
    while(*cur);
    len--;
    *str='\0';
    free(cur);
    return len;
}

static inline uint16*
_short_hash (uint8 len, const char* data)
{
    uint16* hash = (uint16*)malloc(20);
    uint16* short_hash = (uint16*)malloc(10);
    uint16* ret = short_hash;
    uint8 i = 0;
    SHA1(data, len, hash);
    // take only 10 bits (5 bytes -> 5 steps)
    while ( (*short_hash++=*hash++) && (i < 4) ) i++;
    return ret;
}

void 
init_hashlist_buffer (hashlist* buffer, FILE* f)
{
    int* list = (int*)malloc(2*MAX_FILES);
    int* list_start = list;
    uint16 len = 0;
    fseek(f, SIZE_VPKHEADER, SEEK_SET);
    while (1)
    {
        char ext [SIZE_FEXT];
        uint8 len_ext = _read_string(f,ext);
        if (strcmp(ext,"")==0) break;
        while (1)
        {
            char dir [SIZE_FDIR];
            uint8 len_dir = _read_string(f,dir);
            if (strcmp(dir,"")==0) break;
            while (1)
            {
                char name [SIZE_FNAME];
                uint8 len_name = _read_string(f,name);
                if (strcmp(name,"")==0) break;

                uint8 len_path = len_ext+len_dir+len_name;
                char path[len_path+1];
                path[0] = '\0';
                path = maki_strcat(path,dir);
                path = maki_strcat(path,name);
                path = maki_strcat(path,ext);
                *list++ = _short_hash(len_path, path[0]);
                len++;
                // FSEEK BY SIZE OF PAKENTRY
                fseek(f,SIZE_VPKENTRY,SEEK_CUR);
            }
        }   
    }
    buffer->len = len;
    buffer->list = list_start;
}

static inline void
cat_hashlist_buffer (hashlist* dest, hashlist* a, hashlist* b)
{
    int* iarr = (int*) malloc (a->len + b->len);
    int* iarr_start = iarr;
    while (*a->iarr) *iarr++=*a->iarr++;
    while (*b->iarr) *iarr++=*b->iarr++;
    dest->len = a->len + b->len;
    dest->iarr = iarr_start;
}

static inline char
_spot_dup (const int len, const int* iarr)
{
    // stop immediately once dup is found. O(1)
    int* a = iarr;
    int* b = iarr; b++;
    int i = 0;
    while (*a++ != *b++) i++;
    if ( i == len - 1) return 0;
    else return 1;
}

void close_hashlist_buffer (hashlist* buffer)
{
    free(buffer->iarr);
    free(buffer);
}

char compare_paks (const char* a, const char* b)
{
    FILE* fa = fopen(a, "rb"); // open file here because we want to select read-only access
    FILE* fb = fopen(b, "rb");

    hashlist* buff_a = (hashlist*)malloc(4+4);
    init_hashlist_buffer(buff_a, fa);
    fclose(fa);
    hashlist* buff_b = (hashlist*)malloc(4+4);
    init_hashlist_buffer(buff_b, fb);
    fclose(fb);
    // version 1
    hashlist* buff_c = (hashlist*)malloc(4+4);
    cat_hashlist_buffer(buff_c, buff_a, buff_b);

    maki_sort(buff_c->len, buff_c->iarr);
    ret = _spot_dup(buff_c->len, buff_c->iarr);

    close_hashlist_buffer(buff_a);
    close_hashlist_buffer(buff_b);
    close_hashlist_buffer(buff_c);
    return ret;
}
