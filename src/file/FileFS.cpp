#include <nix/file/FileFS.hpp>
#include <nix/file/BlockFS.hpp>
#include <nix/file/SectionFS.hpp>

using namespace std;
using namespace nix;

namespace fs = boost::filesystem;

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


FileFS::FileFS(const string &name, FileMode mode)
    : DirectoryWithAttributes(name, mode){
    this->mode = mode;
    setCreatedAt();
    setUpdatedAt();
    create_subfolders();
    if (!checkHeader()) {
        throw std::runtime_error("Invalid file header: either file format or file version not correct");
    }
}

void FileFS::create_subfolders() {
    fs::path data("data");
    fs::path metadata("metadata");
    fs::path p(location());

    data_dir = Directory(p / data, mode);
    metadata_dir = Directory(p / metadata, mode);
}


ndsize_t FileFS::blockCount() const {
    return data_dir.subdirCount();
}

bool FileFS::hasBlock(const std::string &name_or_id) const  {
    boost::optional<fs::path> path = data_dir.findByNameOrAttribute("entity_id", name_or_id);
    return (bool)path;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(const std::string &name_or_id) const {
    shared_ptr<BlockFS> block;
    boost::optional<fs::path> path = data_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        BlockFS b(file(), path->string());
        return make_shared<BlockFS>(b);
    }
    return block;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(ndsize_t index) const {
    if (index >= blockCount()) {
        throw nix::OutOfBounds("TRying to access file.block with invalid index", index);
    }
    fs::path p = data_dir.sub_dir_by_index(index);
    shared_ptr<BlockFS> b;
    b = make_shared<BlockFS>(file(), p.string());
    return b;
}


std::shared_ptr<base::IBlock> FileFS::createBlock(const std::string &name, const std::string &type) {
    if (hasBlock(name)) {
        throw DuplicateName("createBlock"); // FIXME to test
    }
    string id = util::createId();
    BlockFS b(file(), data_dir.location(), id, type, name);
    return make_shared<BlockFS>(b);
}


bool FileFS::deleteBlock(const std::string &name_or_id) {
    return data_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning sections
//--------------------------------------------------

bool FileFS::hasSection(const std::string &name_or_id) const {
    boost::optional<fs::path> path = metadata_dir.findByNameOrAttribute("entity_id", name_or_id);
    return (bool)path;
}


std::shared_ptr<base::ISection> FileFS::getSection(const std::string &name_or_id) const {
    shared_ptr<base::ISection> sec;
    boost::optional<fs::path> path = metadata_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        SectionFS s(file(), path->string());
        return make_shared<SectionFS>(s);
    }
    return sec;
}


std::shared_ptr<base::ISection> FileFS::getSection(ndsize_t index) const {
    if (index >= sectionCount()) {
        throw OutOfBounds("Trying to access file.section with invalid index.", index);
    }
    fs::path p = metadata_dir.sub_dir_by_index(index);
    shared_ptr<SectionFS> sec;
    sec = make_shared<SectionFS>(file(), p.string());
    return sec;
}


ndsize_t FileFS::sectionCount() const {
    return metadata_dir.subdirCount();
}


std::shared_ptr<base::ISection> FileFS::createSection(const std::string &name, const std::string &type) {
    if (hasSection(name)) {
        throw DuplicateName("createSection");
    }
    string id = util::createId();
    SectionFS s(file(), metadata_dir.location(), id, type, name);
    return make_shared<SectionFS>(s);
}


bool FileFS::deleteSection(const std::string &name_or_id) {
    return metadata_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods for file attribute access.
//--------------------------------------------------


std::vector<int> FileFS::version() const {
    vector<int> version;
    getAttr("version", version);
    return  version;
}


std::string FileFS::format() const {
    string format;
    getAttr("format", format);
    return format;
}


std::string FileFS::location() const {
    return boost::filesystem::canonical(Directory::location()).string();
}


time_t FileFS::createdAt() const {
    string temp_t;
    getAttr("created_at", temp_t);
    return util::strToTime(temp_t);
}


time_t FileFS::updatedAt() const {
    string temp_t;
    getAttr("updated_at", temp_t);
    return util::strToTime(temp_t);
}


void FileFS::setUpdatedAt(){
    if (!hasAttr("updated_at")) {
        time_t t = time(NULL);
        setAttr("updated_at", util::timeToStr(t));
    }
}


void FileFS::forceUpdatedAt() {
    time_t t = time(NULL);
    setAttr("updated_at", util::timeToStr(t));
}


void FileFS::setCreatedAt() {
    if (!hasAttr("created_at")) {
        time_t t = time(NULL);
        setAttr("created_at", util::timeToStr(t));
    }
}


void FileFS::forceCreatedAt(time_t t) {
    setAttr("created_at", util::timeToStr(t));
}


void FileFS::close() {} // FIXME not needed?

bool FileFS::isOpen() const { //FIXME not needed?
    return true;
}


bool FileFS::operator==(const FileFS &other) const {
    return location() == other.location();
}


bool FileFS::operator!=(const FileFS &other) const {
    return location() != other.location();
}


bool FileFS::checkHeader() {
    bool check = true;
    vector<int> version;
    string str;
    // check format
    if (hasAttr("format")) {
        getAttr("format", str);
        if (str != FILE_FORMAT) {
            check = false;
        }
    } else {
        setAttr("format", FILE_FORMAT);
    }
    // check version
    if (hasAttr("version")) {
        getAttr("version", version);
        if (version != FILE_VERSION) {
            check = false;
        }
    } else {
        setAttr("version", FILE_VERSION);
    }
    return check;
}

std::shared_ptr<base::IFile> FileFS::file() const {
    return const_pointer_cast<FileFS>(shared_from_this());
}

FileMode FileFS::fileMode() const {
    return mode;
}

FileFS::~FileFS() {}

} // namespace file
} // namespace nix