#include "../inc/editor.hpp"
#include "../inc/globaldef.hpp"
#include <cerrno>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>

const int PER_FILE_LIMIT = (250 * 1024 * 1024); // 250 MiB
const int DEFAULT_SIZE = 1;


Buffer::Buffer(char *fn, char *sp){
    buffer.clear();
    file = NULL;
    valid_buffer = 1;
    raw_buffer = NULL, filename = NULL, subpath = NULL, fullpath = NULL;
    buffer_size = 0, buffer_size_max = PER_FILE_LIMIT;
    file_size_at_open = 0;
    cursor[0] = 0, cursor[1] = 0;
    filename_str_len = 0, subpath_str_len = 0;
    filename_change_flag = 0;

    // Copy using strdup, wherever the original came from it must be freed after the buffer is constructed. (if necessary)
    // these will need to be freed whenever the deconstructor is called.
    if(!buf_dupe_paths(fn, sp)){
        valid_buffer = 0;
    }
    
    if(!buf_concat_path(filename && subpath)){
        valid_buffer = 0;
    }
    //Determine the size.
    if(buf_open_file() == FILE_RET_ERR){
        valid_buffer = 0;
    }

    if(!buf_raw_allocate((size_t)file_size_at_open)){
        valid_buffer = 0;
    }

    if(buf_raw_read() == BUF_READ_ERR){
        valid_buffer = 0;
    }

}

int Buffer::buf_raw_read(void){
    if(!raw_buffer){
        std::cerr << "No valid buffer!" << std::endl;
        return BUF_READ_ERR;
    }

    if(buffer_size != (size_t)file_size_at_open){
        std::cerr << "Size mismatch!" << std::endl;
        return BUF_READ_ERR;
    }

    if(file){
        const int read = fread(raw_buffer, 1, file_size_at_open, file);
        if(!read){
            std::cerr << "fread() failed!" << std::endl;
            return BUF_READ_ERR;
        }
    }

    return BUF_READ_OK;
}

int Buffer::buf_open_file(void){
    if(!fullpath){
        return FILE_RET_ERR;
    }
    if(!(file = fopen(fullpath, "r"))){
        if(errno == ENOENT){
            return FILE_RET_NOEXIST;
        }
        std::cerr << 
            "Failed to open: " << 
            fullpath << 
            " " << 
            strerror(errno) << 
            std::endl;
        return FILE_RET_ERR;
    }
    fseek(file, 0, SEEK_END);
    file_size_at_open = ftell(file);
    return FILE_RET_OK;
}

int Buffer::buf_raw_allocate(const size_t bsize){
    if(bsize >= buffer_size_max){
        std::cerr << "Buffer exceeds limit!" << std::endl;
        return 0;
    }
    
    raw_buffer = (char*)malloc(sizeof(char) * bsize + 1);
    if(!raw_buffer){
        std::cerr << "malloc() failed!" << std::endl;
        return 0;
    }

    raw_buffer[bsize] = NULLCHAR;
    buffer_size = bsize;
    return 1;
}

int Buffer::buf_dupe_paths(char *fn, char *sp){
    if(fn){
        filename = strdup(fn);
        filename_str_len = strlen(filename);
    } else if (!fn) {
        filename_change_flag = 1;
    }

    if(sp){
        subpath = strdup(sp);
        subpath_str_len = strlen(subpath);
    } 

    if(subpath && filename || !filename && subpath){
        return 1;
    } else {
        return 0;
    }
}

int Buffer::buf_concat_path(const int valid){
    if(valid){
        const size_t fpsize = subpath_str_len + filename_str_len + 1;
        fullpath = (char *)malloc(sizeof(char) * fpsize);
        if(!fullpath){
            std::cerr << "malloc() for fullpath failed!" << std::endl;
            return 0;
        }

        if(!strncpy(fullpath, subpath, subpath_str_len)){
            std::cerr << "strncpy() failed!" << std::endl;
            free(fullpath);
            return 0; 
        }

        if(!strncat(fullpath, filename, filename_str_len)){
            std::cerr << "strncpy() failed!" << std::endl;
            free(fullpath);
            return 0; 
        }

        return 1;
    }
    return 0;
}

 //
 //
 //static size_t len_add(const size_t *lengths, const size_t size)
 //{
     //size_t accumulator = 0;
     //for (size_t i = 0; i < size; i++) {
         //accumulator += lengths[i];
     //}
     //return accumulator;
 //}
 //
 //static File_Info fsize(FILE *fd)
 //{
     //if (!fd) {
         //File_Info f = {0, NULL};
         //return f;
     //}
     //fseek(fd, 0, SEEK_END);
     //File_Info f = {ftell(fd), fd};
     //return f;
 //}
 //
 //static FILE *file_open(const char *path)
 //{
     //FILE *f = NULL;
     //if (!(f = fopen(path, "r"))) {
         //std::cerr << "Failed to open path: " << path << std::endl;
         //return NULL;
     //}
     //return f;
 //}
 //
 ////void Buffers::calculate_buffer_height(const int i){
 ////    return;
 ////}
 //
 ////void Buffers::calculate_cursor_positions(const int i){
  ////   return;
 ////}
 //
 //
 //int Buffers::match_buffer(const char *key)
 //{
     //for (size_t i = 0; i < buf_count; i++) {
         //const char *fn = buffers[i].bchar.fn;
         //if (strcmp(fn, key) == 0) {
             //return i;
         //}
     //}
     //return -1;
 //}
 //
 //
 //size_t Buffers::buffer_count(void) { return buf_count; }
 //
 //void Buffers::buf_replace_at(const int i, const unsigned char c)
 //{
     //if (buf_bounds(i) && buffers[i].bcurs.pos < (int)buffers[i].bchar.size)
         //buffers[i].bchar.buf[buffers[i].bcurs.pos] = c;
 //}
 //
 //int Buffers::pos_bounds(const int i, const int pos)
 //{
     //const size_t size = buffers[i].bchar.size;
     //if (pos > (int)size) {
         //return 0;
     //}
 //
     //if (pos < 0) {
         //return 0;
     //}
 //
     //return 1;
 //}
 //
 //int Buffers::buf_bounds(const int i)
 //{
     //if (i < 0) {
         //return 0;
     //}
 //
     //if (i >= (int)buf_count) {
         //return 0;
     //}
 //
     //return 1;
 //}
 //
 //int static next_delimit_pos(const char *buf, const int pos, const int ssize,
                             //const int direction)
 //{
     //switch (direction) {
     //default:
         //return pos;
     //case -1:
     //{
         //for (int i = pos; i >= 0; i--) {
             //if (buf[i] == NEWLINE || buf[i] == SPACECHAR) {
                 //return i;
             //}
         //}
     //} break;
     //case 1:
     //{
         //for (int i = pos; i <= ssize; i++) {
             //if (buf[i] == NEWLINE || buf[i] == SPACECHAR) {
                 //return i;
             //}
         //}
     //} break;
     //}
 //
     //return pos;
 //}
 //
 //int Buffers::find_word(const int i, const int direction)
 //{
     //if (buf_bounds(i) && pos_bounds(i, buffers[i].bcurs.pos)) {
 //
         //switch (direction) {
         //default:
             //return buffers[i].bcurs.pos;
         //case 1:
         //{
             //int j = next_delimit_pos(buffers[i].bchar.buf, buffers[i].bcurs.pos, buffers[i].bchar.size, 1);
             //const int ssize = (int)buffers[i].bchar.size;
             //while (j < ssize && (buffers[i].bchar.buf[j] == NEWLINE || buffers[i].bchar.buf[j] == SPACECHAR)) {
                 //j++;
             //}
             //return j;
         //} break;
         //case -1:
         //{
             //int j = next_delimit_pos(buffers[i].bchar.buf, buffers[i].bcurs.pos, buffers[i].bchar.size, -1);
             //while (j > 0 && (buffers[i].bchar.buf[j] == NEWLINE || buffers[i].bchar.buf[j] == SPACECHAR)) {
                 //j--;
             //}
             //while (j > 0 &&
                    //!(buffers[i].bchar.buf[j - 1] == NEWLINE || buffers[i].bchar.buf[j - 1] == SPACECHAR)) {
                 //j--;
             //}
             //return j;
         //} break;
         //}
     //} else {
         //return 0;
     //}
 //}
 //
 //int Buffers::find_line(const int i, const int direction)
 //{
     //if (buf_bounds(i) && pos_bounds(i, buffers[i].bcurs.pos)) {
 //
         //switch (direction) {
         //default:
             //return 0;
         //case 1:
         //{
             //const int cond = buffers[i].bchar.buf[buffers[i].bcurs.pos] == NEWLINE; //|| buffers[i].bchar.buf[buffers[i].bcurs.pos] == NULLCHAR;
             //if (cond && pos_bounds(i, buffers[i].bcurs.pos + 1)) {
                 //buffers[i].bcurs.pos++;
             //}
 //
             //for (int j = buffers[i].bcurs.pos; j <= (int)buffers[i].bchar.size; j++) {
                 //const char c = buffers[i].bchar.buf[j];
                 //if (c == NEWLINE || c == NULLCHAR) {
                     //return j;
                 //}
             //}
             //return (int)buffers[i].bchar.size;
         //} break;
 //
         //case -1:
         //{
             //const int cond = buffers[i].bchar.buf[buffers[i].bcurs.pos] == NEWLINE || buffers[i].bchar.buf[buffers[i].bcurs.pos] == NULLCHAR;
             //if (cond && pos_bounds(i, buffers[i].bcurs.pos - 1)) {
                 //buffers[i].bcurs.pos--;
             //}
 //
             //for (int j = buffers[i].bcurs.pos; j >= 0; j--) {
                 //const char c = buffers[i].bchar.buf[j];
                 //if (c == NEWLINE || c == NULLCHAR) {
                     //return j;
                 //}
             //}
             //return 0;
         //} break;
         //}
     //}
     //return 0;
 //}
 //
 //void Buffers::buf_mv_pos(const int i, const int operation)
 //{
     //switch (operation) {
     //default:
         //return;
 //
     //case MV_RIGHT:
     //{
         //if (buf_bounds(i) && pos_bounds(i, buffers[i].bcurs.pos + 1)) {
             //buffers[i].bcurs.pos += 1;
         //} else if (buf_bounds(i) && !pos_bounds(i, buffers[i].bcurs.pos + 1)) {
             //buffers[i].bcurs.pos = buffers[i].bchar.size;
         //}
     //} break;
 //
     //case MV_LEFT:
     //{
         //if (buf_bounds(i) && pos_bounds(i, buffers[i].bcurs.pos - 1)) {
             //buffers[i].bcurs.pos -= 1;
         //} else if (buf_bounds(i) && !pos_bounds(i, buffers[i].bcurs.pos - 1)) {
             //buffers[i].bcurs.pos = 0;
         //}
     //} break;
 //
     //// Words are seperated by spaces, so just find the first char after a space,
     //// can add skip to last position later
     //case NEXT_WORD:
     //{
         //if (buf_bounds(i)) {
             //buffers[i].bcurs.pos = find_word(i, 1);
         //}
     //} break;
     //case PREV_WORD:
     //{
         //if (buf_bounds(i)) {
             //buffers[i].bcurs.pos = find_word(i, -1);
         //}
     //} break;
     //case PREV_LINE:
     //{
         //if (buf_bounds(i)) {
             //buffers[i].bcurs.pos = find_line(i, -1);
         //}
     //} break;
     //case NEXT_LINE:
     //{
         //if (buf_bounds(i)) {
             //buffers[i].bcurs.pos = find_line(i, 1);
         //}
     //} break;
     //}
 //}
 //
 //void Buffers::buf_pos_bw(const int i)
 //{
     //if (buf_bounds(i) && pos_bounds(i, buffers[i].bcurs.pos - 1)) {
         //buffers[i].bcurs.pos--;
     //} else if (buf_bounds(i) && !pos_bounds(i, buffers[i].bcurs.pos - 1)) {
         //buffers[i].bcurs.pos = 0;
     //}
 //}
 //
 //void Buffers::buf_insert(const int i, const char *c)
 //{
     //if (buf_bounds(i) && (size_t)buffers[i].bcurs.pos < buffers[i].bchar.size) {
         //memcpy(&buffers[i].bchar.buf[buffers[i].bcurs.pos], c, 1);
         //buffers[i].bcurs.pos++;
     //}
 //}
 //
 //void Buffers::shift_buffer(const int direction, const int i)
 //{
     //if (!buf_bounds(i)) {
         //return;
     //}
 //
     //switch (direction) {
     //case INS:
     //{ // insert
         //memmove(&buffers[i].bchar.buf[buffers[i].bcurs.pos + 1], &buffers[i].bchar.buf[buffers[i].bcurs.pos],
                 //buffers[i].bchar.size - buffers[i].bcurs.pos - 1);
     //} break;
 //
     //case DEL:
     //{ // delete
         //memmove(&buffers[i].bchar.buf[buffers[i].bcurs.pos], &buffers[i].bchar.buf[buffers[i].bcurs.pos + 1],
                 //buffers[i].bchar.size - buffers[i].bcurs.pos - 1);
     //} break;
 //
     //case RMV:
     //{ // backspace/remove
         //memmove(&buffers[i].bchar.buf[buffers[i].bcurs.pos - 1], &buffers[i].bchar.buf[buffers[i].bcurs.pos], buffers[i].bchar.size - buffers[i].bcurs.pos);
     //} break;
     //}
 //}
 //
 //// Get buffer by idx
 //const Buf *Buffers::get_buf(const int i)
 //{
     //if (!buf_bounds(i)) {
         //return NULL;
     //}
     //return &buffers[i];
 //}
 //
 //void Buffers::print_file(const int i)
 //{
     //if (!buffers[i].bchar.buf) {
         //return;
     //}
     //printf("%s\n", buffers[i].bchar.buf);
 //}
 //
 //void Buffers::delete_buffer(const int i)
 //{
     //if (buffers[i].bchar.buf) {
         //free(buffers[i].bchar.buf);
     //}
     //// kinda not safe
     //memmove(&buffers[i], &buffers[i + 1], buf_count - i - 1);
 //}
 //
 //size_t Buffers::buf_realloc(const int i, const size_t new_size)
 //{
     //assert(buf_bounds(i) && new_size + 1 >= DEFAULT_SIZE);
     //char *tmp = (char *)realloc(buffers[i].bchar.buf, new_size + 1);
     //if (!tmp) {
         //std::cerr << "Failed to reallocate buffer!" << std::endl;
         //return 0;
     //}
     //buffers[i].bchar.buf = tmp;
 //
     //if (new_size == 0) {
         //for (size_t j = 0; j < new_size + 1; j++) {
             //buffers[i].bchar.buf[j] = NULLCHAR;
         //}
 //
         //buffers[i].bchar.size = new_size;
         //return new_size;
     //}
 //
     //buffers[i].bchar.buf[new_size] = NULLCHAR;
     //buffers[i].bchar.size = new_size;
     //return new_size;
 //}
 //
 //int Buffers::realloc_buffer_list(const int direction)
 //{
     //const int tmp_size = (int)buf_count + direction;
     //assert(tmp_size >= 1);
     //Buf *tmp = (Buf *)realloc(buffers, tmp_size * sizeof(Buf));
     //if (!tmp) {
         //std::cout << "realloc for buffer list failed!" << std::endl;
         //return 0;
     //}
     //buffers = tmp;
     //buf_count = tmp_size;
     //return 1;
 //}
 //
 //int Buffers::allocate_buffer_list(void)
 //{
     //buffers = (Buf *)malloc(sizeof(Buf) * 1);
     //if (!buffers) {
         //std::cout << "Could not allocate char pointer array!" << std::endl;
         //return 0;
     //}
     //buf_count = 1;
     //return 1;
 //}
 //// Append the initial filename. If it is empty, it will just append as empty
 //// and the file names vector will be empty, so this will need to be handled
 //// later if no initial file name was provided
 //Buffers::Buffers(char *wp, char *str_arg)
 //{
     //if (allocate_buffer_list()) {
         //const int gate = (str_arg && strlen(str_arg) > 0) ? 1 : 0;
 //
         //switch (gate) {
         //case 0:
         //{
             //append_buffer(wp, random_fn(), 1);
             //buf_malloc(0, 0);
         //} break;
 //
         //case 1:
         //{
             //append_buffer(wp, str_arg, 0);
             //read_file(wp, str_arg);
         //} break;
         //}
     //}
 //}
 //
 //void Buffers::append_buffer(char *wp, char *fn, const int fn_needs_change)
 //{
     //assert(buf_count >= 1);
     //buffers[buf_count - 1] = (Buf){
         //(Buffer_Char){fn, wp, NULL, fn_needs_change, 0},
         //(Buffer_Cursor){0, 0, 0, 0},
         //(Buffer_Dims){0, 0, 0, 0, 0}
     //};
 //}
 //
 //char *Buffers::random_fn(void)
 //{
     //srand(time(NULL));
 //
     //const size_t str_length = 24;
     //const size_t ASCII_RAND_MAX = 128 - 32;
 //
     //char *str = (char *)malloc(str_length + 1);
     //if (!str) {
         //std::cout << "Could not allocate char pointer! - random_fn(void)" << std::endl;
         //return NULL;
     //}
     //for (size_t i = 0; i < str_length; i++) {
         //const unsigned char r = rand() % ASCII_RAND_MAX;
         //str[i] = r;
     //}
     //str[str_length] = '\0';
     //return str;
 //}
 //
 //size_t Buffers::read_file(const char *wp, const char *fn)
 //{
     //std::cout << fn << std::endl;
     //std::cout << wp << std::endl;
 //
     //const char *s = "/";
     //const size_t lengths[] = {strlen(fn), strlen(wp), strlen(s)};
     //char *path = (char *)malloc(len_add(lengths, 3) + 1);
 //
     //strncpy(path, wp, lengths[1] + 1);
     //strncat(path, s, lengths[2] + 1);
     //strncat(path, fn, lengths[0] + 1);
 //
     //File_Info fi = fsize(file_open(path));
     //const int i = match_buffer(fn);
     //size_t read = 0;
 //
     //if (fi.f && fi.fs > 0) {
         //rewind(fi.f);
         //const int cond = fi.fs > PER_FILE_LIMIT;
 //
         //switch (cond) {
         //case 0:
         //{
             //if (buf_malloc(i, (size_t)fi.fs)) {
                 //read = fread(buffers[i].bchar.buf, 1, fi.fs, fi.f);
             //}
         //} break;
         //case 1:
         //{
             //std::cerr << "File too large!" << std::endl;
         //} break;
         //}
 //
     //} else if (!fi.f) {
         //buf_malloc(i, 0);
     //}
 //
     //free(path);
     //return read;
 //}
 //
 //size_t Buffers::buf_malloc(const int i, const size_t size)
 //{
     //assert(buf_bounds(i) && size + 1 >= DEFAULT_SIZE);
     //buffers[i].bchar.buf = (char *)malloc(size + 1);
     //if (!buffers[i].bchar.buf) {
         //std::cerr << "Failed to allocate buffer!" << std::endl;
         //return 0;
     //}
 //
     //buffers[i].bchar.size = size;
     //return 1;
 //}
 //
 //int Buffers::write_buffer(const char *wp, const char *fn)
 //{
     //const int i = match_buffer(fn);
     //assert(buf_bounds(i));
 //
     //const char *s = "/";
     //const size_t lengths[] = {strlen(fn), strlen(wp), strlen(s)};
     //char *path = (char *)malloc(len_add(lengths, 3) + 1);
 //
     //strncpy(path, wp, lengths[1] + 1);
     //strncat(path, s, lengths[2] + 1);
     //strncat(path, fn, lengths[0] + 1);
 //
     //FILE *f = fopen(path, "w");
     //if (!f) {
         //std::cerr << "Failed to open file!" << std::endl;
         //return 0;
     //}
 //
     //const size_t written = fwrite(buffers[i].bchar.buf, 1, buffers[i].bchar.size, f);
     //if (written != buffers[i].bchar.size) {
         //std::cerr << "Failed to write to file!" << std::endl;
         //return 0;
     //}
 //
     //fclose(f);
     //free(path);
     //return written;
 //}
 //
 //void Buffers::update_cursor_dimensions(const int i, const Vec2i *dims){
     //if(buf_bounds(i)){
         //buffers[i].bcurs.w = dims->x;
         //buffers[i].bcurs.h = dims->y;
     //}
 //}
