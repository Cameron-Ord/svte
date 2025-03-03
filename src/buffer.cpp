#include "../inc/editor.hpp"

FILE *Buffers::read_file(void) {}
int Buffers::write_file(void) {}

Buffers::Buffers(void) : f(NULL), buffer_count(0) { buffers.clear(); }
