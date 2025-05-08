#include "../inc/editor.hpp"
#include "../inc/defines.hpp"
#include <fstream>
#include <iostream>

const int PER_FILE_LIMIT = (250 * 1024 * 1024); // 250 MiB
const int DEFAULT_SIZE = 1;

static std::string get_delimiter(void)
{
#ifdef WIN32
    return "\\";
#endif

#ifdef __linux__
    return "/";
#endif

    return "invalid";
}

Buffer::Buffer(std::string fn, std::string sp, const int identifier)
{
    buffer.clear();
    is_valid = BUF_STATE_VALID, id = identifier, filename_change_flag = 0, row = 0;
    path_delimiter = get_delimiter(), fullpath = "", filename = fn, subpath = sp;

    std::cout << "internal buffer ID: " << id << std::endl;
    if (id == FILE_ID_BROKEN) {
        is_valid = BUF_STATE_ERR;
    }

    if (buf_concat_paths() == STR_CONCAT_NO_FN) {
        filename_change_flag = 1;
    }

    if (buf_open_file() == FILE_RET_ERR) {
        is_valid = BUF_STATE_ERR;
    }

    if (buffer.size() == 0) {
        //Initialize as size of DEFAULT_SIZE and ensure empty strings.
        buffer.resize(DEFAULT_SIZE);
        for (size_t j = 0; j < DEFAULT_SIZE; j++) {
            Line line = {"", 0};
            buffer[j] = line;
        }
    }

    std::cout << "Buffer state: " << is_valid << std::endl;
}
int Buffer::buf_get_valid(void) { return is_valid; }
const std::vector<Line> *Buffer::buf_get_buffer(void) { return &buffer; }
const int *Buffer::buf_get_row(void) { return &row; }
const int *Buffer::buf_get_col(void)
{
    const int ssize = buffer.size();
    if (row >= 0 && row < ssize) {
        return &buffer.at(row).col;
    }
    std::cerr << "Get col returned NULL! Row :" << row << std::endl;
    return NULL;
}

//Remove char from row
int Buffer::buf_row_remove_char(void)
{
    const int buf_ssize = buffer.size();
    if(row >= 0 && row < buf_ssize){
        Line *line = &buffer.at(row);
        const int str_ssize = line->str.size();
 
        if(line->col > 0 && line->col <= str_ssize){
            const int implicit_col = line->col - 1;
            line->str.erase(line->str.begin() + implicit_col); 
            line->col--;
            return RMV_OK;
        
        } else if(line->col == 0 && line->str.empty() && buf_ssize > 1){
            buffer.erase(buffer.begin() + row);
            if(row > 0){
                row--;
            }
            return RMV_OK;
        }
    }
    return BAD_DEL;
}

//Append from row
int Buffer::buf_row_append(void)
{
    if (row >= 0 && buffer.begin() + row != buffer.end()) {
        const int next = row + 1;
        Line line = {std::string(""), 0};
        buffer.insert(buffer.begin() + next, line);
        row++;
        return ROW_OK;
    }
    return BAD_APND;
}

//Insert at cursor
int Buffer::buf_row_insert_char(const char c)
{
    const int buf_ssize = buffer.size();
    if (row >= 0 && row < buf_ssize) {

        Line *line = &buffer.at(row);
        const int str_ssize = line->str.size();
        
        if (line->col >= 0 && line->col <= str_ssize) {
            line->str.insert(line->str.begin() + line->col, c);
            line->col++;
            return INS_OK;
        }
    }
    return BAD_INS;
}

//Insert after cursor
int Buffer::buf_row_append_char(const char c)
{
    const int buf_ssize = buffer.size();
    if (row >= 0 && row < buf_ssize) {
        Line *line = &buffer.at(row);
        const int str_ssize = line->str.size();
        
        if (line->col >= 0 && line->col < str_ssize) {
            const int implicit_col = line->col + 1;
            line->str.insert(line->str.begin() + implicit_col, c);
            line->col++;
            return INS_OK;
        
        } else if (line->col - 1 >= -1 && line->col == str_ssize) {
            const int implicit_col = (line->col - 1) + 1;
            line->str.insert(line->str.begin() + implicit_col, c);
            line->col++;
            return INS_OK;
        }
    }
    return BAD_INS;
}

//Delete at cursor
int Buffer::buf_row_delete_char(void)
{
    const int buf_ssize = buffer.size();
    if (row >= 0 && row < buf_ssize) {
        Line *line = &buffer.at(row);
        const int str_ssize = line->str.size();

        if (line->col > 0 && line->col < str_ssize) {
            line->str.erase(line->str.begin() + line->col);
            if(line->col == str_ssize){
                line->col--;
            }   
            return DEL_OK;

        } else if (line->col > 0 && line->col == str_ssize) {
            const int implicit_col = line->col - 1;
            line->str.erase(line->str.begin() + implicit_col);
            line->col--;
            return DEL_OK;

        } else if (line->col == 0) {
            if(line->str.empty() && buf_ssize > 1){
                buffer.erase(buffer.begin() + row);
                if(row > 0){
                    row--;
                }
                return DEL_OK;

            } else if(!line->str.empty()) {
                line->str.erase(line->str.begin() + 0);
                return DEL_OK;
            }
        }
    }
    return BAD_DEL;
}

int Buffer::buf_row_first_char(std::string row, const int offset)
{
    for (size_t i = offset; i < row.size(); i++) {
        if (row[i] != SPACECHAR) {
            return i;
        }
    }
    return 0;
}

void Buffer::buf_shift_curs_col(const int d)
{
    const int buf_ssize = buffer.size();
    if (!(row >= 0 && row < buf_ssize)) {
        return;
    }

    Line *line = &buffer.at(row);
    const int string_ssize = line->str.size();
    if (line->col + d >= 0 && line->col + d <= string_ssize) {
        line->col += d;
    }
}

void Buffer::buf_shift_curs_row(const int d)
{
    const int ssize = buffer.size();
    if (!(row + d >= 0 && row + d < ssize)) {
        return;
    }
    row += d;
}

int Buffer::buf_save_file(void){
    if (fullpath.size() == 0 || filename.size() == 0) {
        std::cout << "No provided filename or path" << std::endl;
        return FILE_RET_NO_FN;
    }

    const std::string checkstr = subpath + path_delimiter + filename;
    if (!(fullpath.size() > 0 && fullpath == checkstr)) {
        std::cerr << "Path mismatch!" << std::endl;
        return FILE_RET_ERR;
    }

    std::ofstream file(fullpath, std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Error while opening in write mode" << std::endl;
        return FILE_RET_ERR;
    }

    std::vector<Line>::iterator it;
    size_t accumulator = 0;
    for(it = buffer.begin(); it != buffer.end(); ++it){
        std::string str = it->str;
        accumulator += str.size() * sizeof(str[0]);
        //Just needs a newline, so not using std::endl;
        file << str << '\n';
    }

    std::cout << accumulator << " Bytes written" << std::endl;
    file.close();
    return FILE_RET_OK;
}

int Buffer::buf_open_file(void)
{
    if (fullpath.size() == 0 || filename.size() == 0) {
        std::cout << "No provided filename or path" << std::endl;
        return FILE_RET_NO_FN;
    }

    const std::string checkstr = subpath + path_delimiter + filename;
    if (!(fullpath.size() > 0 && fullpath == checkstr)) {
        std::cerr << "Path mismatch!" << std::endl;
        return FILE_RET_ERR;
    }

    std::ifstream file(fullpath, std::ios::in);
    if (!file.is_open()) {
        if (file.bad()) {
            std::cerr << "Fatal error while opening in read mode" << std::endl;
            return FILE_RET_ERR;
        } else if (file.fail()) {
            return FILE_RET_OK;
        }
    }

    std::string str;
    while (std::getline(file, str)) {
        Line line = {str, 0};
        buffer.push_back(line);
    }
    file.close();
    return FILE_RET_OK;
}

int Buffer::buf_concat_paths(void)
{
    if (filename.size() == 0) {
        return STR_CONCAT_NO_FN;
    }
    fullpath = subpath + path_delimiter + filename;
    return STR_CONCAT_OK;
}