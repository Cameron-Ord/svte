#include "buffer.hpp"
#include "input.hpp"
#include "util.hpp"

#include <cstdlib>
#include <random>
#include <unordered_set>

static const int MIN_BUFFER_SIZE = 1;
std::unordered_set<int> used_ids;
std::vector<unsigned int> ids(0);

static std::vector<std::string> parse_args(int argc, char *argv[]) {
  std::vector<std::string> argbuf(argc - 1);
  for (int i = 0; i < argc - 1; i++) {
    argbuf[i] = argv[i + 1];
  }
  return argbuf;
}

static int random_num(void) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());

  const int attempts = 50;
  for (int i = 0; i < attempts; i++) {
    const int generated = dist(gen);
    if (used_ids.insert(generated).second) {
      logger::log_var("Generated ID: ", generated);
      return generated;
    }
  }
  return -1;
}

static std::shared_ptr<buffer> create_buffer(int argc, char **argv) {
  int id = random_num();
  if (id < 0) {
    logger::log("Failed to generate an ID");
    return nullptr;
  }

  std::vector<std::string> argbuf = parse_args(argc, argv);
  std::string fp;

  if (argbuf.size() > 0) {
    fp = argbuf[0];
  }
  return std::make_shared<buffer>(id, fp, text_io::read_text_file(fp));
}

static int
init_buffer_map(std::unordered_map<int, std::shared_ptr<buffer>> &bufmap,
                int argc, char **argv) {
  std::shared_ptr<buffer> buf = nullptr;
  if (!(buf = create_buffer(argc, argv))) {
    logger::log("Failed to allocate buffer");
    return -1;
  }
  bufmap[buf->get_id()] = buf;
  ids.insert(ids.end(), buf->get_id());
  return buf->get_id();
}

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "");
  std::unordered_map<int, std::shared_ptr<buffer>> bufmap;
  int current_id = init_buffer_map(bufmap, argc, argv);
  if (current_id < 0) {
    return 1;
  }

  input_tree input = input_tree();

  // ⏻ ☹ 😄
  return 0;
}
