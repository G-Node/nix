//
// Created by jan on 7/30/15.
//

#include "nix/file/AttributesFS.hpp"

using namespace nix::file;
using namespace boost::filesystem;
using namespace YAML;

AttributesFS::AttributesFS() { }


AttributesFS::AttributesFS(const std::string &file_path) {
    path temp(file_path.c_str());
    if (exists(temp)) {
        this->loc = temp;
        this->open_or_create();
   }
}


void AttributesFS::open_or_create() {
    path attr(".attributes");
    path temp = this->location() /  attr;
    if (!exists(temp)) {
        std::ofstream ofs;
        ofs.open(this->location().string() + "/.attributes", std::ofstream::out | std::ofstream::app);
        ofs.close();
    }
    this->node = LoadFile(this->location().string() + "/.attributes");
}


bool AttributesFS::has(const std::string &name) {
    this->open_or_create();
    return (this->node.size() > 0) && (this->node[name]);
}


void AttributesFS::flush() {
    std::ofstream ofs;
    ofs.open(this->location().string() + "/.attributes", std::ofstream::trunc);
    if(ofs.is_open())
        ofs << this->node << std::endl;
    else
        std::cerr << "Failure!!!" << std::endl;
    ofs.close();
}

path AttributesFS::location() const {
    return this->loc;
}

nix::ndsize_t AttributesFS::attributeCount() {
    this->open_or_create();
    return this->node.size();
}

void AttributesFS::remove(const std::string &name) {
    this->open_or_create();
    if (this->node[name]) {
        this->node.remove(name);
    }
    this->flush();
}
