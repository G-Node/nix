#ifndef NIX_FILE_FS_HPP
#define NIX_FILE_FS_HPP

#include <nix/base/IFile.hpp>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>
#include <nix/filesys/FilesystemAttribute.hpp>

namespace nix {
namespace filesys {

class FileFS : public base::IFile {

private:
    boost::filesystem::path root_path, data_path, metadata_path;
    unsigned int file_mode;
    YAML::Node attributes;
    FilesystemAttribute attribute;

    void open_or_create();

public:
    FileFS(const std::string &name, const FileMode mode = FileMode::ReadWrite);


    ndsize_t blockCount() const;


    bool hasBlock(const std::string &name_or_id) const;


    std::shared_ptr<base::IBlock> getBlock(const std::string &name_or_id) const;


    std::shared_ptr<base::IBlock> getBlock(ndsize_t index) const;


    std::shared_ptr<base::IBlock> createBlock(const std::string &name, const std::string &type);


    bool deleteBlock(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    bool hasSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(ndsize_t index) const;


    ndsize_t sectionCount() const;


    std::shared_ptr<base::ISection> createSection(const std::string &name, const std::string &type);


    bool deleteSection(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods for filesys attribute access.
    //--------------------------------------------------


    std::vector<int> version() const;


    std::string format() const;


    std::string location() const;


    time_t createdAt() const;


    time_t updatedAt() const;


    void setUpdatedAt();


    void forceUpdatedAt();


    void setCreatedAt();


    void forceCreatedAt(time_t t);


    void close() override;


    bool isOpen() const;


    bool operator==(const FileFS &other) const;


    bool operator!=(const FileFS &other) const;


    virtual ~FileFS();

    private:

    std::shared_ptr<base::IFile> file() const;

    // check for existence
    bool fileExists(const std::string &name) const;

    // check if the header of the filesys is valid
    bool checkHeader() const;

};

} // namespace filesys
} // namespace nix



#endif //NIX_FILE_FS_H
