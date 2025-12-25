#include "core/buffer.hpp"
#include "events/events.hpp"
#include "renderer/renderer.hpp"
#include "util/util.hpp"
#include "window/window.hpp"

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <random>
#include <unordered_set>

std::unordered_set<int> used_ids;

static int random_num(void);
static std::shared_ptr<buffer> create_buffer(int argc, char **argv);
static int init_buffer_map(std::unordered_map<int, std::shared_ptr<buffer>> &bufmap, int argc, char **argv);

static bool sdl_init(void);

static bool sdl_init(void) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    return false;
  }

  if (!TTF_Init()) {
    return false;
  }

  return true;
}

uint32_t read_text_in(void);

std::vector<uint32_t> read_text_in(const char *text) {
  std::vector<uint32_t> retrieved(0);
  const size_t text_len = strlen(text);
  const char *p = text;
  const char *end = p + text_len;
  while (p < end) {
    int seq_len = utf_handler::utf8_byte_count(static_cast<char>(*p));
    if (seq_len <= 0) {
      ++p;
      continue;
    }

    if (p + seq_len > end) {
      break;
    }

    std::vector<char> bytes(seq_len);
    for (size_t i = 0; i < bytes.size() && p < end; i++) {
      if (p + i > end) {
        break;
      }
      bytes[i] = p[i];
    }

    const uint32_t cp = utf_handler::decode_utf8(bytes);
    if (cp != 0) {
      retrieved.push_back(cp);
    }
    p += seq_len;
  }
  return retrieved;
}

int main(int argc, char *argv[]) {
  if (!sdl_init()) {
    logger::log("Failed to initialize SDL");
    return 1;
  }

  // Maybe implement a keyboard grab that can be switched on and off later?
  window_container window = window_container("SVTE", 400, 300);
  if (!(window.init_window(SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE))) {
    SDL_Quit();
    return 1;
  }

  // Eventually want to have multiple font options, but just regular is fine for
  // dev
  renderer_container renderer = renderer_container("IosevkaNerdFont-Regular.ttf", 12);
  if (!renderer.init_renderer(window.get_window())) {
    SDL_DestroyWindow(window.get_window());
    SDL_Quit();
    return 1;
  }

  if (!renderer.get_font_container().font_open()) {
    SDL_DestroyWindow(window.get_window());
    SDL_Quit();
    return 1;
  }

  renderer.get_font_container().get_font_map().map_insert_defaults(renderer.get_font_container().get_font(),
                                                                   renderer.get_renderer());

  std::unordered_map<int, std::shared_ptr<buffer>> bufmap;
  int current_id = init_buffer_map(bufmap, argc, argv);
  if (current_id < 0) {
    return 1;
  }
  std::shared_ptr<buffer> current = bufmap[current_id];

  input_tree input = input_tree(SDLK_Q, SDLK_ESCAPE, SDLK_R, SDLK_S, SDL_KMOD_ALT);
  bool running = true;
  const uint32_t dtime = 1000 / 30;

  SDL_StartTextInput(window.get_window());
  SDL_ShowWindow(window.get_window());
  while (running) {
    const uint32_t start = SDL_GetTicks();
    renderer.set_col(0, 0, 0, 255);
    renderer.clear();

    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      default:
        break;

      case SDL_EVENT_TEXT_INPUT: {
        if (current) {
          std::vector<uint32_t> ret = read_text_in(ev.text.text);
          for (size_t i = 0; i < ret.size(); i++) {
            input.send_insert_request(ret[i], current);
          }
        }
      } break;

      case SDL_EVENT_KEY_DOWN: {
        unsigned int keysym = ev.key.key;
        unsigned int keymod = ev.key.mod;
        input.parse_input(keysym, keymod, current);
      } break;

      case SDL_EVENT_WINDOW_RESIZED:
        break;

      case SDL_EVENT_QUIT:
        running = false;
        break;
      }
    }

    if (current) {
      renderer.draw_text(current->const_buf());
      renderer.draw_cursor(current);
    }

    renderer.present();
    const uint32_t frametime = SDL_GetTicks() - start;
    if (frametime < dtime) {
      SDL_Delay(dtime - frametime);
    }
  }

  // ⏻ ☹ 😄
  return 0;
}

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
      logger::log_int("Generated ID: ", generated);
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

  char_mat_ptr contents = std::make_shared<char_mat>();
  contents = text_io::read_text_file(fp, contents);
  std::shared_ptr<buffer> buf = std::make_shared<buffer>(id, fp, contents);

  return buf;
}

static int init_buffer_map(std::unordered_map<int, std::shared_ptr<buffer>> &bufmap, int argc, char **argv) {
  std::shared_ptr<buffer> buf = create_buffer(argc, argv);
  if (!buf) {
    logger::log("Failed to allocate buffer");
    return -1;
  }
  const int id = buf->get_id();
  bufmap.insert({id, buf});
  return id;
}
