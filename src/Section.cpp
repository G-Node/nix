// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Section.hpp>

#include <list>
#include <algorithm>
#include <iterator>
#include <nix/Block.hpp>
#include <nix/File.hpp>
#include <nix/DataArray.hpp>
#include <nix/util/util.hpp>

using namespace nix;

Section::Section()
    : NamedEntity()
{
}


Section::Section(std::nullptr_t ptr)
    : NamedEntity()
{
}


Section::Section(const Section &other)
    : NamedEntity(other.impl())
{
}


Section::Section(const std::shared_ptr<base::ISection> &p_impl)
    : NamedEntity(p_impl)
{
}


Section::Section(std::shared_ptr<base::ISection> &&ptr)
    : NamedEntity(std::move(ptr))
{
}


void Section::link(const Section &link) {
    if (!link) {
        backend()->link(none);
    } else {
        backend()->link(link.id());
    }
}

//-----------------------------------------------------
// Methods concerning child sections
//-----------------------------------------------------


bool Section::hasSection(const Section &section) const {
    if (!util::checkEntityInput(section, false)) {
        return false;
    }
    return backend()->hasSection(section.id());
}


bool Section::deleteSection(const Section &section) {
    if (!util::checkEntityInput(section, false)) {
        return false;
    }
    return backend()->deleteSection(section.id());
}

/*
 * Helper struct for {@link findSections}.
 */
struct SectionCont {
    SectionCont(Section s, size_t d = 0)
    : entity(s), depth(d)
    {}

    Section entity;
    size_t depth;
};


std::vector<Section> Section::sections(const util::Filter<Section>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getSection(i); };
    return getEntities<Section>(f, sectionCount(), filter);
}


std::vector<Section> Section::findSections(const util::Filter<Section>::type &filter,
                                           size_t max_depth) const {
    std::vector<Section>  results;
    std::list<SectionCont> todo;
    bool first_iter = true;

    SectionCont current(*this);
    while (todo.size() > 0 || first_iter) {
        if(!first_iter) {
            current = todo.front();
            todo.pop_front();
            if (filter(current.entity)) {
                results.push_back(current.entity);
            }
        } else {
            first_iter = false;
        }
        if (current.depth < max_depth) {
            size_t next_depth = current.depth + 1;
            for (const auto &s : current.entity.sections()) {
                todo.push_back(SectionCont(s, next_depth));
            }
        }
    }

    return results;
}

static inline auto erase_section_with_id(std::vector<Section> &sections, const std::string &my_id)
    -> decltype(sections.size())
{
    sections.erase(remove_if(sections.begin(),
                             sections.end(),
                             [&my_id](const Section &section) {
                                 return my_id == section.id();
                             }),
                   sections.end());

    return sections.size();
}

std::vector<Section> Section::findRelated(const util::Filter<Section>::type &filter) const
{
    std::vector<Section> results = findDownstream(filter);
    const std::string &my_id = id();

    //This checking of results can be removed if we decide not to include this in findSection
    auto results_size = erase_section_with_id(results, my_id);

    if (results_size == 0) {
        results = findUpstream(filter);
    }
    //This checking of results can be removed if we decide not to include this in findSection
    results_size = erase_section_with_id(results, my_id);

    if (results_size == 0) {
        results = findSideways(filter, id());
    }
    return results;
}


//-----------------------------------------------------
// Methods for property access
//-----------------------------------------------------

bool Section::hasProperty(const Property &property) const {
    if (property == none || !property.isValidEntity()) {
        return false;
    }
    return backend()->hasProperty(property.id());
}

std::vector<Property> Section::properties(const util::Filter<Property>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getProperty(i); };
    return getEntities<Property>(f,
            propertyCount(),
            filter);
}

bool Section::deleteProperty(const Property &property) {
    if (property == none || !property.isValidEntity()) {
        return false;
    }
    return backend()->deleteProperty(property.id());
}

std::vector<Property> Section::inheritedProperties() const {

    std::vector<Property> own = properties();

    if (link() == none)
        return own;

    const std::vector<Property> linked = link().properties();

    copy_if (linked.begin(), linked.end(),
             back_inserter(own),
             [&own](const Property &linked_prop) {
                 return find_if (own.begin(), own.end(),
                                [&linked_prop](const Property &own_prop) {
                                    return linked_prop.name() == own_prop.name();
                                }) == own.end();
             });

    return own;
}


//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------

size_t Section::tree_depth() const{
  const std::vector<Section> children = sections();
  size_t depth = 0;
  if (children.size() > 0) {
      for (auto &child : children) {
          depth = std::max(depth, child.tree_depth());
      }
      depth += 1;
  }
  return depth;
}


std::vector<Section> Section::findDownstream(const std::function<bool(Section)> &filter) const{
    std::vector<Section> results;
    size_t max_depth = tree_depth();
    size_t actual_depth = 1;
    while (results.size() == 0 && actual_depth <= max_depth) {
        results = findSections(filter, actual_depth);
        actual_depth += 1;
    }
    return results;
}


std::vector<Section> Section::findUpstream(const std::function<bool(Section)> &filter) const{
    std::vector<Section> results;
    Section p = parent();

    if (p != none) {
        results = p.findSections(filter,1);
        if (results.size() > 0) {
            return results;
        }
        return p.findUpstream(filter);
    }
    return results;
}


std::vector<Section> Section::findSideways(const std::function<bool(Section)> &filter, const std::string &caller_id) const{
    std::vector<Section> results;
    Section p = parent();
    if (p != none) {
        results = p.findSections(filter,1);
        if (results.size() > 0) {

            results.erase(remove_if(results.begin(),
                                    results.end(),
                                    [&caller_id](const Section &section) {
                                        return section.id() == caller_id;
                                    }),
                          results.end());

            return results;
        }
        return p.findSideways(filter, caller_id);
    }
    return results;
}


std::ostream& nix::operator<<(std::ostream &out, const Section &ent) {
    out << "Section: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

void Section::repository(const std::string &repository) {
    util::checkEmptyString(repository, "repository");
    backend()->repository(repository);
}

void Section::link(const std::string &id) {
    util::checkEmptyString(id, "link");
    backend()->link(id);
}

void Section::mapping(const std::string &mapping) {
    util::checkEmptyString(mapping, "mapping");
    backend()->mapping(mapping);
}

Section Section::createSection(const std::string &name, const std::string &type) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasSection(name)) {
        throw DuplicateName("createSection");
    }
    return backend()->createSection(name, type);
}

Property Section::createProperty(const std::string &name, const DataType &dtype) {
    util::checkEntityName(name);
    if (backend()->hasProperty(name)) {
        throw DuplicateName("hasProperty");
    }
    return backend()->createProperty(name, dtype);
}

Property Section::createProperty(const std::string &name, const std::vector<Value> &values) {
    if (values.size() < 1)
        throw std::runtime_error("Trying to create a property without a value!");
    util::checkEntityName(name);
    if (backend()->hasProperty(name)) {
        throw DuplicateName("hasProperty");
    }
    return backend()->createProperty(name, values);
}

Property Section::createProperty(const std::string &name, const Value &value) {
    util::checkEntityName(name);
    if (backend()->hasProperty(name)){
        throw DuplicateName("Property with that name already exists!");
    }
    return backend()->createProperty(name, value);
}

Section Section::getSection(ndsize_t index) const {
    if (index >= backend()->sectionCount()) {
        throw OutOfBounds("Section::getSection: index is out of bounds!");
    }
    return backend()->getSection(index);
}


std::vector<nix::DataArray> Section::referringDataArrays() const {
    std::vector<nix::DataArray> arrays;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::DataArray> temp = referringDataArrays(b);
        arrays.insert(arrays.end(), temp.begin(), temp.end());
    }
    return arrays;
}


std::vector<nix::DataArray> Section::referringDataArrays(const Block &b) const {
    std::vector<nix::DataArray> arrays;
    if (b) {
        arrays = b.dataArrays(nix::util::MetadataFilter<nix::DataArray>(id()));
    }
    return arrays;
}


std::vector<nix::Tag> Section::referringTags() const {
    std::vector<nix::Tag> tags;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::Tag> temp = referringTags(b);
        tags.insert(tags.end(), temp.begin(), temp.end());
    }
    return tags;
}


std::vector<nix::Tag> Section::referringTags(const Block &b) const {
    std::vector<nix::Tag> tags;
    if (b) {
        tags = b.tags(nix::util::MetadataFilter<nix::Tag>(id()));
    }
    return tags;
}


std::vector<nix::MultiTag> Section::referringMultiTags() const {
    std::vector<nix::MultiTag> tags;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::MultiTag> temp = referringMultiTags(b);
        tags.insert(tags.end(), temp.begin(), temp.end());
    }
    return tags;
}


std::vector<nix::MultiTag> Section::referringMultiTags(const Block &b) const {
    std::vector<nix::MultiTag> tags;
    if (b) {
        tags = b.multiTags(nix::util::MetadataFilter<nix::MultiTag>(id()));
    }
    return tags;
}


std::vector<nix::Source> Section::referringSources() const {
    std::vector<nix::Source> srcs;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::Source> temp = referringSources(b);
        srcs.insert(srcs.end(), temp.begin(), temp.end());
    }
    return srcs;
}


std::vector<nix::Source> Section::referringSources(const Block &b) const {
    std::vector<nix::Source> srcs;
    if (b) {
        srcs = b.findSources(nix::util::MetadataFilter<nix::Source>(id()));
    }
    return srcs;
}


std::vector<nix::Block> Section::referringBlocks() const {
    nix::File f = backend()->parentFile();
    return f.blocks(nix::util::MetadataFilter<nix::Block>(id()));
}
