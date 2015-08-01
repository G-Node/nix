#include <nix/file/FileFS.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/SectionHDF5.hpp>
#include <nix/util/util.hpp>

using namespace std;
using namespace nix;

namespace nix {
namespace file {

    // Format definition
#define FILE_VERSION std::vector<int>{1, 0, 0}
#define FILE_FORMAT  std::string("nix")

static unsigned int map_file_mode(FileMode mode) {
    switch (mode) {
        case FileMode::ReadWrite:
            return ios::in | ios::out;

        case FileMode::ReadOnly:
            return ios::out;

        case FileMode::Overwrite:
            return ios::trunc;

        default:
            return ios::out;
    }
}


FileFS::FileFS(const string &name, FileMode mode) {
    boost::filesystem::path p(name.c_str());
    if (!boost::filesystem::exists(p)) {
        mode = FileMode::Overwrite;
    }

    this->file_mode = map_file_mode(mode);
    this->root_path = p;
    this->open_or_create();
    setCreatedAt();
    setUpdatedAt();

    if (!checkHeader()) {
        throw std::runtime_error("Invalid file header: either file format or file version not correct");
    }
}

void FileFS::open_or_create() {
    //TODO respect the file mode
    boost::filesystem::path data("data");
    boost::filesystem::path metadata("metadata");

    this->data_path = this->root_path / data;
    this->metadata_path = this->root_path / metadata;
    if (!boost::filesystem::exists(this->root_path)) {
        boost::filesystem::create_directory(this->root_path);
        boost::filesystem::create_directories(this->data_path);
        boost::filesystem::create_directories(this->metadata_path);
    } else {
        if (!boost::filesystem::exists(this->data_path)) {
            boost::filesystem::create_directories(this->data_path);
        }
        if (!boost::filesystem::exists(this->metadata_path)) {
            boost::filesystem::create_directories(this->metadata_path);
        }
    }
    this->attributes = AttributesFS(this->location());
}


ndsize_t FileFS::blockCount() const {
    ndsize_t count = 0;
    boost::filesystem::directory_iterator end;
    if (boost::filesystem::exists(this->data_path) && boost::filesystem::is_directory(this->data_path)) {
        boost::filesystem::directory_iterator di(this->data_path);
        while (di != end) {
            count ++;
        }
    }
    return count;
}

bool FileFS::hasBlock(const std::string &name_or_id) const  {
    return false;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(const std::string &name_or_id) const {
    shared_ptr<nix::hdf5::BlockHDF5> block;
    /*
    boost::optional<Group> group = data.findGroupByNameOrAttribute("entity_id", name_or_id);
    if (group)
       block = make_shared<BlockHDF5>(file(), *group);
    */
    return block;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(ndsize_t index) const {
    shared_ptr<nix::hdf5::BlockHDF5> block;
    return block;
}


std::shared_ptr<base::IBlock> FileFS::createBlock(const std::string &name, const std::string &type) {
    shared_ptr<nix::hdf5::BlockHDF5> block;
    return block;
}


bool FileFS::deleteBlock(const std::string &name_or_id) {
    return false;
}

//--------------------------------------------------
// Methods concerning sections
//--------------------------------------------------

bool FileFS::hasSection(const std::string &name_or_id) const {
    return false;
}


std::shared_ptr<base::ISection> FileFS::getSection(const std::string &name_or_id) const {
    shared_ptr<nix::hdf5::SectionHDF5> block;
    return block;
}


std::shared_ptr<base::ISection> FileFS::getSection(ndsize_t index) const {
    shared_ptr<nix::hdf5::SectionHDF5> block;
    return block;
}


ndsize_t FileFS::sectionCount() const {
    ndsize_t count = 0;
    boost::filesystem::directory_iterator end;
    if (boost::filesystem::exists(this->metadata_path) && boost::filesystem::is_directory(this->metadata_path)) {
        boost::filesystem::directory_iterator di(this->metadata_path);
        while (di != end) {
            count ++;
        }
    }
    return count;
}


std::shared_ptr<base::ISection> FileFS::createSection(const std::string &name, const std::string &type) {
    shared_ptr<hdf5::SectionHDF5> block;
    return block;
}


bool FileFS::deleteSection(const std::string &name_or_id) {
    return  false;
}

//--------------------------------------------------
// Methods for file attribute access.
//--------------------------------------------------


std::vector<int> FileFS::version() const {
    vector<int> version;
    return  version;
}


std::string FileFS::format() const {
    return "nix";
}


std::string FileFS::location() const {
    return boost::filesystem::absolute(this->root_path).string();
}


time_t FileFS::createdAt() const {
    return util::strToTime("00:00.00");
}


time_t FileFS::updatedAt() const {
    return util::strToTime("00:00.00");
}


void FileFS::setUpdatedAt(){
    if (!this->attributes.has("updated_at")) {
        time_t t = time(NULL);
        this->attributes.set("updated_at", util::timeToStr(t));
    }
}


void FileFS::forceUpdatedAt() {
    time_t t = time(NULL);
    this->attributes.set("updated_at", util::timeToStr(t));
}


void FileFS::setCreatedAt() {
    if (!this->attributes.has("created_at")) {
        time_t t = time(NULL);
        this->attributes.set("created_at", util::timeToStr(t));
    }
}


void FileFS::forceCreatedAt(time_t t) {
    this->attributes.set("created_at", util::timeToStr(t));
}


void FileFS::close() {}


bool FileFS::isOpen() const {
    return false;
}


bool FileFS::operator==(const FileFS &other) const {
    return false;
}


bool FileFS::operator!=(const FileFS &other) const {
    return false;
}

FileFS::~FileFS() {

}

bool FileFS::checkHeader() {
    bool check = true;
    vector<int> version;
    string str;
    // check format
    if (this->attributes.has("format")) {
        this->attributes.get("format", str);
        if (str != FILE_FORMAT) {
            check = false;
        }
    } else {
        this->attributes.set("format", FILE_FORMAT);
    }
    // check version
    if (this->attributes.has("version")) {
        this->attributes.get("version", version);
        if (version != FILE_VERSION) {
            check = false;
        }
    } else {
        this->attributes.set("version", FILE_VERSION);
    }
    return check;
}

} // namespace file
} // namespace nix