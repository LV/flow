#include <filesystem>
#include <fstream> // ofstream
#include <iomanip> // put_time
#include <iostream> // cerr
#include <string>
#include <toml++/toml.hpp>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp" // border, hbox, vbox


struct Config {
    std::string session_path = "./";
};

std::string current_timestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d-%H%M%S");
    return oss.str();
}


void force_one_newline_at_end(std::string& text) {
    while (!text.empty() && text.back() == '\n') {
        text.pop_back();
    }
    text += '\n';
}


void write_file(const std::string& content, const std::string& filename) {
    if (content == "" || content == "\n") {
        return;
    }

    std::filesystem::path filepath(filename);
    std::filesystem::create_directories(filepath.parent_path());

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    file << content;
    if (!file.good()) {
        throw std::runtime_error("Failed to write to file: " + filename);
    }
}


Config read_config() {
    Config config = Config();

    // Get config path using XDG standard
    const char* xdg = std::getenv("XDG_CONFIG_HOME");
    const char* home = std::getenv("HOME");

    std::string config_path = "";

    if (home && *home) {
        config.session_path = std::filesystem::path(home) / "flow";
        config_path = std::filesystem::path(home) / ".config" / "flow" / "config.toml";
    }

    if (xdg && *xdg) {
        config_path = std::filesystem::path(xdg) / "flow" / "config.toml";
    }


    try {
        toml::table tbl = toml::parse_file(config_path);
        config.session_path = tbl["path"].value_or("./");
    } catch (const toml::parse_error& err) {
        std::cerr << "Warning: Could not load config file in '$(XDG_CONFIG_HOME)/flow/config.toml':\n" 
                  << err << "\nUsing defaults instead.\n";
    }
    return config;
}


void run_application(Config config) {
    std::string buffer_data;
    const std::string session_folder_name = "session-" + current_timestamp();
    const std::string base_path = config.session_path;

    ftxui::Component input_buffer = ftxui::Input(&buffer_data, "Write your thought...");

    // Catch Events for submission
    input_buffer = ftxui::CatchEvent(input_buffer, [&](const ftxui::Event& event) {
        if (event == ftxui::Event::Tab) {
            const std::filesystem::path note_full_path = std::filesystem::path(base_path) / session_folder_name / (current_timestamp() + ".txt");
            force_one_newline_at_end(buffer_data);
            write_file(buffer_data, note_full_path);
            buffer_data.clear();
            return true;
        }
        return false;
    });

    auto component = ftxui::Container::Vertical({
        input_buffer,
    });

    auto renderer = ftxui::Renderer(component, [&] {
        return ftxui::vbox({
            ftxui::hbox(input_buffer->Render()),
        })
        | ftxui::border;
    });

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    screen.Loop(renderer);
}


int main() {
    run_application(read_config());
    return 0;
}
