#pragma once

#include "ResourceManager.h"
#include "File.h"

class TextFileManager : public ResourceManager<TextFileManager, std::string> {

    friend CRSP<TextFileManager>;
    using Base = ResourceManager<TextFileManager, std::string>;

public:
    void start_up() override {

    }

    void shut_down() override {
        Base::shut_down();
    }
private:
    std::string* load_resource(const std::string& filename) override {

        assert(file_exists(filename.c_str()));

        auto content = new std::string(read_from_file(filename.c_str()));

        return content;
    }

};