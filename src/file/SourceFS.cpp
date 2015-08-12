// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/SourceFS.hpp>
#include <nix/util/util.hpp>
#include <nix/Source.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


SourceFS::SourceFS(const std::shared_ptr<IFile> &file, const std::string &loc)
    : EntityWithMetadataFS(file, loc)
{
    createSubFolders(file);
}


SourceFS::SourceFS(const shared_ptr<IFile> &file, const string &loc, const std::string &id, const string &type, const string &name)
    : SourceFS(file, loc, id, type, name, util::getTime())
{
}


SourceFS::SourceFS(const shared_ptr<IFile> &file, const string &loc, const std::string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time)
{
    createSubFolders(file);
}


void SourceFS::createSubFolders(const shared_ptr<IFile> &file) {
    boost::filesystem::path s("sources"), p(location());
    sources_dir = Directory(p / s, file->fileMode());
}

bool SourceFS::hasSource(const string &name_or_id) const {
    return getSource(name_or_id) != nullptr;
}


shared_ptr<ISource> SourceFS::getSource(const string &name_or_id) const {
    shared_ptr<SourceFS> source;
    boost::optional<boost::filesystem::path> p = sources_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (p) {
        source = make_shared<SourceFS>(file(), p->string());
    }

    return source;
}


shared_ptr<ISource> SourceFS::getSource(ndsize_t index) const {
    if (index >= sourceCount()) {
        throw OutOfBounds("Trying to access source.source with invalid index.", index);
    }
    boost::filesystem::path p = sources_dir.sub_dir_by_index(index);
    shared_ptr<SourceFS> source;
    source = make_shared<SourceFS>(file(), p.string());
    return source;
}


ndsize_t SourceFS::sourceCount() const {
    return sources_dir.subdirCount();
}


shared_ptr<ISource> SourceFS::createSource(const string &name, const string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSource(name)) {
        throw DuplicateName("createSource");
    }
    string id = util::createId();
    SourceFS s(file(), sources_dir.location(), id, type, name);
    return make_shared<SourceFS>(s);
}


bool SourceFS::deleteSource(const string &name_or_id) {
    return sources_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}


SourceFS::~SourceFS() {}

} // ns nix::file
} // ns nix
