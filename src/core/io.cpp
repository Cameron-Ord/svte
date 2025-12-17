#include "../util.hpp"
const char NEWLINE = '\n';

vec_2d_ptr text_io::read_text_file(const std::string &file_path, vec_2d_ptr buf) {
  vec_2d_ptr copy = std::make_shared<vec_2d>(*buf);
  std::ifstream file(file_path);
  if (!file) {
    return copy;
  }

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> rawbuf(size);
  if (!file.read(rawbuf.data(), size)) {
    file.close();
    return copy;
  }

  size_t i = 0, k = 0;
  while (i < rawbuf.size()) {
    const char bytecount = utf_handler::utf8_byte_count(rawbuf[i]);
    std::vector<char> bytes(bytecount);
    for (char j = 0; j < bytecount && (i + j) < rawbuf.size(); j++) {
      bytes[j] = rawbuf[i + j];
    }

    const uint32_t cp = utf_handler::decode_utf8(bytes);
    switch (cp) {
    default:
      (*copy)[k].push_back(cp);
      break;
    case NEWLINE:
      copy->push_back(std::vector<uint32_t>(0));
      k++;
      break;
    }

    i += bytecount;
  }

  file.close();
  return copy;
}

size_t text_io::write_text_file(const std::string &file_path, vec_2d_ptr buf) {
  std::ofstream file(file_path);
  if (!file) {
    return 0;
  }

  size_t written = 0;
  for (size_t i = 0; i < buf->size(); i++) {
    for (size_t j = 0; j < (*buf)[i].size(); j++) {
      std::vector<char> bytes = utf_handler::encode_utf8((*buf)[i][j]);
      for (size_t k = 0; k < bytes.size(); k++) {
        file << bytes[k];
        written++;
      }
    }
    file << NEWLINE;
    written++;
  }

  return written;
}
