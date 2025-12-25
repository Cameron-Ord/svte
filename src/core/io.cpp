#include "../util/util.hpp"

char_mat_ptr text_io::read_text_file(const std::string &file_path, char_mat_ptr buf) {
  std::ifstream file(file_path);
  if (!file) {
    return buf;
  }

  file.seekg(0, std::ios::end);
  long size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> rawbuf(static_cast<size_t>(size));
  if (!file.read(rawbuf.data(), size)) {
    file.close();
    return buf;
  }

  size_t i = 0;
  while (i < rawbuf.size()) {
    const u8 bytecount = utf_handler::utf8_byte_count(rawbuf[i]);
    std::vector<char> bytes(bytecount);
    for (u8 j = 0; j < bytecount && (i + j) < rawbuf.size(); j++) {
      bytes[j] = rawbuf[i + j];
    }

    buf->push_back(utf_handler::decode_utf8(bytes));
    i += bytecount;
  }

  logger::log_int_unsigned("Read bytes: ", i);
  file.close();
  return buf;
}

size_t text_io::write_text_file(const std::string &file_path, char_mat_ptr buf) {
  std::ofstream file(file_path);
  if (!file) {
    return 0;
  }

  size_t written = 0;
  for (size_t i = 0; i < buf->size(); i++) {
    std::vector<char> bytes = utf_handler::encode_utf8((*buf)[i]);
    for (size_t k = 0; k < bytes.size(); k++) {
      file << bytes[k];
      written++;
    }
    written++;
  }

  return written;
}
